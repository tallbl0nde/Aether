#include "Aether/primary/Image.hpp"

namespace Aether {
    Image::Image(int x, int y, std::string p, RenderType t) : Texture(x, y, t) {
        this->type = Type::Path;
        this->path = p;

        // Now check if we need to render immediately
        if (this->renderType == RenderType::OnCreate) {
            this->generateSurface();
            this->convertSurface();
        }
    }

    Image::Image(int x, int y, unsigned char * p, size_t s, RenderType t) : Texture(x, y, t) {
        this->type = Type::Pointer;
        this->ptr = p;
        this->size = s;

        // Now check if we need to render immediately
        if (this->renderType == RenderType::OnCreate) {
            this->generateSurface();
            this->convertSurface();
        }
    }

    void Image::generateSurface() {
        switch (this->type) {
            case Type::Path:
                this->drawable = this->renderer->renderImageSurface(this->path);
                break;

            case Type::Pointer:
                std::vector<unsigned char> buf;
                buf.assign(this->ptr, this->ptr + this->size);
                this->drawable = this->renderer->renderImageSurface(buf);
                break;
        }
    }
};