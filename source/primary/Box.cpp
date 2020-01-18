#include "Box.hpp"

namespace Aether {
    Box::Box(int x, int y, int w, int h, unsigned int b) : Texture(x, y) {
        Texture::setW(w);
        Texture::setH(h);
        this->setBorder(b);
    }

    void Box::redrawTexture() {
        this->setTexture(SDLHelper::renderRect(this->w(), this->h(), this->border()));
    }

    unsigned int Box::border() {
        return this->border_;
    }

    void Box::setBorder(unsigned int b) {
        this->border_ = b;
        this->redrawTexture();
    }

    void Box::setBoxSize(int w, int h) {
        Texture::setW(w);
        Texture::setH(h);
        this->redrawTexture();
    }
};
