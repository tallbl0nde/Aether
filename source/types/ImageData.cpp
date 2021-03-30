#include "Aether/types/ImageData.hpp"

namespace Aether {
    ImageData::ImageData() {
        this->isBytes = true;
        this->bytes.resize(0);
        this->width_ = 0;
        this->height_ = 0;
        this->channels_ = 0;
    }

    ImageData::ImageData(const std::vector<uint8_t> & data, const size_t width, const size_t height, const uint8_t channels) {
        this->isBytes = true;
        this->bytes = data;
        this->width_ = width;
        this->height_ = height;
        this->channels_ = channels;
    }

    ImageData::ImageData(const std::vector<Colour> & pixels, const size_t width, const size_t height, const uint8_t channels) {
        this->isBytes = false;
        this->pixels = pixels;
        this->width_ = width;
        this->height_ = height;
        this->channels_ = channels;
    }

    bool ImageData::valid() {
        if (this->isBytes) {
            return !this->bytes.empty();
        } else {
            return !this->pixels.empty();
        }
    }

    size_t ImageData::width() {
        return this->width_;
    }

    size_t ImageData::height() {
        return this->height_;
    }

    uint8_t ImageData::channels() {
        return this->channels_;
    }

    std::vector<uint8_t> ImageData::toByteVector() {
        // Simply return copy if same format
        if (this->isBytes) {
            return this->bytes;
        }

        // Otherwise iterate over and convert
        std::vector<uint8_t> bytes(this->pixels.size() * this->channels_);
        for (size_t idx = 0; idx < this->pixels.size(); idx++) {
            size_t byteIdx = idx * this->channels_;
            Colour & pixel = this->pixels[idx];

            bytes[byteIdx + 0] = pixel.r();
            bytes[byteIdx + 1] = pixel.g();
            bytes[byteIdx + 2] = pixel.b();
            if (this->channels_ > 3) {
                bytes[byteIdx + 3] = pixel.a();
            }
        }

        return bytes;
    }

    std::vector<Colour> ImageData::toPixelVector() {
        // Simply return copy if same format
        if (this->isBytes) {
            return this->pixels;
        }

        // Otherwise iterate over and convert
        std::vector<Colour> pixels(this->bytes.size() / this->channels_);
        for (size_t byteIdx = 0; byteIdx < this->bytes.size(); byteIdx += this->channels_) {
            Colour colour = Colour(this->bytes[byteIdx], this->bytes[byteIdx + 1], this->bytes[byteIdx + 2], (this->channels_ > 3 ? this->bytes[byteIdx + 3] : 255));
            pixels[byteIdx/this->channels_] = colour;
        }

        return pixels;
    }
};