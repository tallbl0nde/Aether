#include "Aether/primary/Image.hpp"

namespace Aether {
    Image::Image(const int x, const int y, unsigned char * data, const size_t size, const Render type) : Texture(x, y) {
        this->scaleWidth_ = 0;
        this->scaleHeight_ = 0;
        this->mem.ptr = data;
        this->mem.size = size;
        this->type = Type::Pointer;
        this->performRender(type);
    }

    Image::Image(const int x, const int y, const std::string & path, const Render type) : Texture(x, y) {
        this->scaleWidth_ = 0;
        this->scaleHeight_ = 0;
        this->path = path;
        this->type = Type::File;
        this->performRender(type);
    }

    Image::Image(const int x, const int y, const std::vector<unsigned char> & data, const Render type) : Texture(x, y) {
        this->scaleWidth_ = 0;
        this->scaleHeight_ = 0;
        this->copy = data;
        this->type = Type::Vector;
        this->performRender(type);
    }

    Drawable * Image::renderDrawable() {
        switch (this->type) {
            case Type::File:
                return this->renderer->renderImageSurface(this->path, this->scaleWidth_, this->scaleHeight_);

            case Type::Pointer: {
                std::vector<unsigned char> buf;
                buf.assign(this->mem.ptr, this->mem.ptr + this->mem.size);
                return this->renderer->renderImageSurface(buf, this->scaleWidth_, this->scaleHeight_);
            }

            case Type::Vector:
                return this->renderer->renderImageSurface(this->copy, this->scaleWidth_, this->scaleHeight_);
        }
    }

    void Image::performRender(const Render type) {
        if (type == Render::Sync) {
            this->renderSync();

        } else if (type == Render::Async) {
            this->renderAsync();
        }
    }

    bool Image::setScaleDimensions(const size_t width, const size_t height) {
        // We can't render images over 8000px
        if (width >= 8000 || height >= 8000) {
            return false;
        }

        this->scaleWidth_ = width;
        this->scaleHeight_ = height;
        return true;
    }

    size_t Image::scaleWidth() {
        return this->scaleWidth_;
    }

    size_t Image::scaleHeight() {
        return this->scaleHeight_;
    }

    Image::~Image() {

    }
};