#include "Image.hpp"

namespace Aether {
    Image::Image(int x, int y, std::string p) : Texture(x, y) {
        this->setTexture(SDLHelper::renderImage(p));
    }

    Image::Image(int x, int y, u8 * p, size_t s, int f1, int f2) : Texture(x, y) {
        this->setTexture(SDLHelper::renderImage(p, s, f1, f2));
    }

    void Image::redrawTexture() {

    }
};