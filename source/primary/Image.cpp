#include "Image.hpp"

namespace Aether {
    Image::Image(int x, int y, std::string p, int xF, int yF, RenderType t) : Texture(x, y, t) {
        this->type = Type::Path;
        this->path = p;
        this->xF = xF;
        this->yF = yF;

        // Now check if we need to render immediately
        if (this->renderType == RenderType::OnCreate) {
            this->generateSurface();
            this->convertSurface();
        }
    }

    Image::Image(int x, int y, u8 * p, size_t s, int xF, int yF, RenderType t) : Texture(x, y, t) {
        this->type = Type::Pointer;
        this->ptr = p;
        this->size = s;
        this->xF = xF;
        this->yF = yF;

        // Now check if we need to render immediately
        if (this->renderType == RenderType::OnCreate) {
            this->generateSurface();
            this->convertSurface();
        }
    }

    void Image::generateSurface() {
        switch (this->type) {
            case Type::Path:
                this->surface = SDLHelper::renderImageS(this->path, this->xF, this->yF);
                break;

            case Type::Pointer:
                this->surface = SDLHelper::renderImageS(this->ptr, this->size, this->xF, this->yF);
                break;
        }
    }
};