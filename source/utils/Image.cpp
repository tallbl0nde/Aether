#include "Aether/ThreadPool.hpp"
#include "Aether/utils/Image.hpp"
#include <array>
#include <cmath>
#include <thread>

namespace Aether::Utils::Image {
    // Calculates the interpolated value using a cubic hermite spline
    static double cubicHermite(const double a, const double b, const double c, const double d, const double t) {
        double A = -(a / 2.0d) + ((3.0d * b) / 2.0d) - ((3.0d * c) / 2.0d) + (d / 2.0d);
        double B = a - ((5.0d * b) / 2.0d) + (2.0d * c) - (d / 2.0d);
        double C = -(a / 2.0d) + (c / 2.0d);
        double D = b;

        return (A * t * t * t) + (B * t * t) + (C * t) + D;
    }

    // Ensures the requested coordinates are bounded within the image
    static std::pair<size_t, size_t> getClampedCoords(const ImageData * image, int x, int y) {
        x = (x > 0 ? (x >= image->width() ? image->width() - 1 : x) : 0);
        y = (y > 0 ? (y >= image->height() ? image->height() - 1 : y) : 0);
        return std::pair<size_t, size_t>(x, y);
    }

    ImageData * scaleBicubic(ImageData * image, const size_t width, const size_t height) {
        // Return copy if same dimensions
        if (image->width() == width && image->height() == height) {
            return new ImageData(image->toColourVector(), image->width(), image->height(), image->channels());;
        }

        // Set up scaled image
        ImageData * resized = new ImageData(std::vector<Colour>(), width, height, 4);

        // Pre-allocate arrays
        std::array<std::array<double,   4>, 4> columnValues;        // Interpolated column values for pixel
        std::array<double,                  4> interpolated;        // Interpolated channels for pixel
        std::array<std::array<Colour *, 4>, 4> samples;             // Pixels which are interpolated

        // Work down each row (x and y are coords)
        for (size_t y = 0; y < resized->height(); y++) {
            double v = y / (double)(resized->height() - 1);

            for (size_t x = 0; x < resized->width(); x++) {
                double u = x / (double)(resized->width() - 1);

                double xPos = (u * image->width() - 0.5d);
                int xPx = (int)xPos;
                double xRem = xPos - floor(xPos);

                double yPos = (v * image->height() - 0.5d);
                int yPx = (int)yPos;
                double yRem = yPos - floor(yPos);

                // Find colours to sample
                for (int row = -1; row <= 2; row++) {
                    for (int col = -1; col <= 2; col++) {
                        std::pair<size_t, size_t> coords = getClampedCoords(image, xPx + col, yPx + row);
                        samples[col + 1][row + 1] = image->colourAt(coords.first, coords.second);
                    }
                }

                // Interpolate vertically first
                for (size_t col = 0; col < 4; col++) {
                    columnValues[col][0] = cubicHermite(samples[0][col]->r(), samples[1][col]->r(), samples[2][col]->r(), samples[3][col]->r(), xRem);
                    columnValues[col][1] = cubicHermite(samples[0][col]->g(), samples[1][col]->g(), samples[2][col]->g(), samples[3][col]->g(), xRem);
                    columnValues[col][2] = cubicHermite(samples[0][col]->b(), samples[1][col]->b(), samples[2][col]->b(), samples[3][col]->b(), xRem);
                    columnValues[col][3] = cubicHermite(samples[0][col]->a(), samples[1][col]->a(), samples[2][col]->a(), samples[3][col]->a(), xRem);
                }

                // Then interpolate horizontally
                for (size_t channel = 0; channel < interpolated.size(); channel++) {
                    double value = cubicHermite(columnValues[0][channel], columnValues[1][channel], columnValues[2][channel], columnValues[3][channel], yRem);
                    value = (value > 0.0d ? (value < 255.0d ? value : 255.0d) : 0.0d);
                    interpolated[channel] = value;
                }

                // Set pixel in resized image
                Colour * pixel = resized->colourAt(x, y);
                pixel->setColour(interpolated[0], interpolated[1], interpolated[2], interpolated[3]);
            }
        }

        return resized;
    }

    ImageData * scaleBoxSampling(ImageData * image, const size_t width, const size_t height) {
        // Set up scaled image
        ImageData * scaled = new ImageData(std::vector<Colour>(), width, height, 4);
        ImageData * tmp    = new ImageData(std::vector<Colour>(), scaled->width(), image->height(), 4);

        // Scale horizontally first
        double rw = image->width() / (double)scaled->width();
        double iw = 1.0d / rw;

        for (size_t y = 0; y < image->height(); y++) {
            for (size_t x = 0; x < scaled->width(); x++) {
                double pos = x * rw;
                int baseIdx = (int)pos;

                std::array<double, 4> channels;
                Colour * pixel = nullptr;
                double val;

                pixel = image->colourAt(baseIdx, y);
                val = (double)baseIdx + 1 - pos;
                channels[0] = pixel->r() * val;
                channels[1] = pixel->g() * val;
                channels[2] = pixel->b() * val;
                channels[3] = pixel->a() * val;

                double end = pos + rw;
                int endI = (int)end;

                val = end - (double)endI;
                if (val > 1e-4f) {
                    pixel = image->colourAt(endI, y);
                    channels[0] += pixel->r() * val;
                    channels[1] += pixel->g() * val;
                    channels[2] += pixel->b() * val;
                    channels[3] += pixel->a() * val;
                }

                for (size_t i = baseIdx+1; i < endI; i++) {
                    pixel = image->colourAt(i, y);
                    channels[0] += pixel->r();
                    channels[1] += pixel->g();
                    channels[2] += pixel->b();
                    channels[3] += pixel->a();
                }

                pixel = tmp->colourAt(x, y);
                pixel->setR((channels[0] * iw) + 0.5d);
                pixel->setG((channels[1] * iw) + 0.5d);
                pixel->setB((channels[2] * iw) + 0.5d);
                pixel->setA((channels[3] * iw) + 0.5d);
            }
        }

        // Then vertically
        double rh = image->height() / (double)scaled->height();
        double ih = 1.0d / rh;

        for (size_t y = 0; y < scaled->height(); y++) {
            for (size_t x = 0; x < scaled->width(); x++) {
                double pos = y * rh;
                int baseIdx = (int)pos;

                std::array<double, 4> channels;
                Colour * pixel = nullptr;
                double val;

                pixel = tmp->colourAt(x, baseIdx);
                val = (double)baseIdx + 1 - pos;
                channels[0] = pixel->r() * val;
                channels[1] = pixel->g() * val;
                channels[2] = pixel->b() * val;
                channels[3] = pixel->a() * val;

                double end = pos + rh;
                int endI = (int)end;

                val = end - (double)endI;
                if (val > 1e-4f) {
                    pixel = tmp->colourAt(x, endI);
                    channels[0] += pixel->r() * val;
                    channels[1] += pixel->g() * val;
                    channels[2] += pixel->b() * val;
                    channels[3] += pixel->a() * val;
                }

                for (size_t i = baseIdx+1; i < endI; i++) {
                    pixel = tmp->colourAt(x, i);
                    channels[0] += pixel->r();
                    channels[1] += pixel->g();
                    channels[2] += pixel->b();
                    channels[3] += pixel->a();
                }

                pixel = scaled->colourAt(x, y);
                pixel->setR((channels[0] * ih) + 0.5d);
                pixel->setG((channels[1] * ih) + 0.5d);
                pixel->setB((channels[2] * ih) + 0.5d);
                pixel->setA((channels[3] * ih) + 0.5d);
            }
        }

        delete tmp;
        return scaled;
    }

    ImageData * scaleOptimal(ImageData * image, const size_t width, const size_t height) {
        // Sanity checks
        if (width == 0 || height == 0 || image == nullptr) {
            return nullptr;

        } else if (width == image->width() && height == image->height()) {
            // Copy even though same image as it has been "scaled"
            return new ImageData(image->toColourVector(), image->width(), image->height(), image->channels());;
        }

        // Use box sampling if one or more dimensions are being scaled down
        ImageData * scaled;
        if (width <= image->width() && height <= image->height()) {
            scaled = scaleBoxSampling(image, width, height);

        // Otherwise default to bicubic interpolation
        } else {
            scaled = scaleBicubic(image, width, height);
        }

        return scaled;
    }
}