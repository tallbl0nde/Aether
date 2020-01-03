#include "Image.hpp"

namespace Aether {
    Image::Image(Element * e, std::string p) : Texture(e, nullptr) {
        // this->set
    }

    Image::Image(Element * e, u8 * p, size_t s) : Texture(e, nullptr) {
        this->setTexture(SDLHelper::renderImage(p, s));
    }

    Image::~Image() {

    }
};