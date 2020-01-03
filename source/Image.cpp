#include "Image.hpp"

namespace Aether {
    Image::Image(Element * e, std::string p) : Texture(e) {
        // this->set
    }

    Image::Image(Element * e, u8 * p, size_t s) : Texture(e) {
        this->setTexture(SDLHelper::renderImage(p, s));
    }

    void Image::redrawTexture() {

    }

    Image::~Image() {

    }
};