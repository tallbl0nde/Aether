#include "Aether/primary/Image.hpp"

namespace Aether {
    Image::Image(const int x, const int y, unsigned char * data, const size_t size, const Render type) : Texture(x, y) {
        this->mem.ptr = data;
        this->mem.size = size;
        this->type = Type::Pointer;
        this->performRender(type);
    }

    Image::Image(const int x, const int y, const std::string & path, const Render type) : Texture(x, y) {
        this->path = path;
        this->type = Type::File;
        this->performRender(type);
    }

    Image::Image(const int x, const int y, const std::vector<unsigned char> & data, const Render type) : Texture(x, y) {
        this->copy = data;
        this->type = Type::Vector;
        this->performRender(type);
    }

    Drawable * Image::renderDrawable() {
        switch (this->type) {
            case Type::File:
                return this->renderer->renderImageSurface(this->path);

            case Type::Pointer: {
                std::vector<unsigned char> buf;
                buf.assign(this->mem.ptr, this->mem.ptr + this->mem.size);
                return this->renderer->renderImageSurface(buf);
            }

            case Type::Vector:
                return this->renderer->renderImageSurface(this->copy);
        }
    }

    void Image::performRender(const Render type) {
        if (type == Render::Sync) {
            this->renderSync();

        } else if (type == Render::Async) {
            this->renderAsync();
        }
    }

    Image::~Image() {

    }
};