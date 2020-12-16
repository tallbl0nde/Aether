#include "Aether/types/GlyphMetrics.hpp"

namespace Aether {
    GlyphMetrics::GlyphMetrics() {
        this->ch_ = 0;
        this->width_ = 0;
        this->height_ = 0;
        this->lineHeight_ = 0;
    }

    GlyphMetrics::GlyphMetrics(const uint16_t ch, const int width, const int height, const int lineHeight) {
        this->ch_ = ch;
        this->width_ = width;
        this->height_ = height;
        this->lineHeight_ = lineHeight;
    }

    uint16_t GlyphMetrics::character() {
        return this->ch_;
    }

    int GlyphMetrics::width() {
        return this->width_;
    }

    int GlyphMetrics::height() {
        return this->height_;
    }

    int GlyphMetrics::lineHeight() {
        return this->lineHeight_;
    }
};