#include "Image.hpp"

namespace Aether {
    Image::Image(std::string p) : Texture() {
        // this->set
    }

    Image::Image(u8 * p, size_t s) : Texture() {
        this->setTexture(SDLHelper::renderImage(p, s));
    }

    void Image::redrawTexture() {

    }
};