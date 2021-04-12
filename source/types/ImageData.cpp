#include "Aether/types/ImageData.hpp"

namespace Aether {
    ImageData::ImageData() {
        this->pixels_.resize(0);
        this->width_ = 0;
        this->height_ = 0;
        this->channels_ = 0;
    }

    ImageData::ImageData(const std::vector<uint8_t> & data, const size_t width, const size_t height, const uint8_t channels) {
        // Convert bytes to pixels
        for (size_t i = 0; i < data.size(); i += channels) {
            this->pixels_.push_back(Colour(data[i], data[i+1], data[i+2], data[i+3]));
        }

        // Pad if necessary with opaque white
        if (this->pixels_.size() < width * height) {
            this->pixels_.resize(width * height, Colour(255, 255, 255, 255));
        }

        this->width_ = width;
        this->height_ = height;
        this->channels_ = channels;
    }

    ImageData::ImageData(const std::vector<Colour> & pixels, const size_t width, const size_t height, const uint8_t channels) {
        this->pixels_ = pixels;

        // Pad if necessary with opaque white
        if (this->pixels_.size() < width * height) {
            this->pixels_.resize(width * height, Colour(255, 255, 255, 255));
        }

        this->width_ = width;
        this->height_ = height;
        this->channels_ = channels;
    }

    bool ImageData::valid() const {
        return !this->pixels_.empty();
    }

    size_t ImageData::width() const {
        return this->width_;
    }

    size_t ImageData::height() const {
        return this->height_;
    }

    uint8_t ImageData::channels() const {
        return this->channels_;
    }

    Colour * ImageData::colourAt(const size_t x, const size_t y) const {
        // Check within range
        size_t idx = (this->width_ * y) + x;
        if (idx >= this->pixels_.size()) {
            return nullptr;
        }

        Colour & colour = const_cast<ImageData *>(this)->pixels_[idx];
        return &colour;
    }

    std::vector<uint8_t> ImageData::toByteVector() const {
        // Convert from pixels to bytes
        std::vector<uint8_t> bytes(this->pixels_.size() * this->channels_);
        for (size_t idx = 0; idx < this->pixels_.size(); idx++) {
            size_t byteIdx = idx * this->channels_;
            Colour pixel = this->pixels_[idx];

            bytes[byteIdx + 0] = pixel.r();
            bytes[byteIdx + 1] = pixel.g();
            bytes[byteIdx + 2] = pixel.b();
            if (this->channels_ > 3) {
                bytes[byteIdx + 3] = pixel.a();
            }
        }

        return bytes;
    }

    std::vector<Colour> ImageData::toColourVector() const {
        return this->pixels_;
    }
};