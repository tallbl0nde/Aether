#include "Image.hpp"

namespace Aether {
    Image::Image(int x, int y, std::string p) : Texture(x, y) {
        // this->set
    }

    Image::Image(int x, int y, u8 * p, size_t s) : Texture(x, y) {
        this->setTexture(SDLHelper::renderImage(p, s));
    }

    void Image::redrawTexture() {

    }
};