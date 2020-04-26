#include "Image.hpp"

namespace Aether {
    Image::Image(int x, int y, std::string p, int f1, int f2, RenderType rt) : Texture(x, y) {
        if (rt != RenderType::Deferred) {
            this->setTexture(SDLHelper::renderImage(p, f1, f2));
        }
    }

    Image::Image(int x, int y, u8 * p, size_t s, int f1, int f2, RenderType rt) : Texture(x, y) {
        if (rt != RenderType::Deferred) {
            this->setTexture(SDLHelper::renderImage(p, s, f1, f2));
        }
    }

    void Image::redrawTexture() {

    }
};