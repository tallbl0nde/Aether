#include "Rectangle.hpp"

namespace Aether {
    Rectangle::Rectangle(Element * e, int w, int h, unsigned int r) : Texture(e, nullptr) {
        Texture::setW(w);
        Texture::setH(h);
        this->setCornerRadius(r);
    }

    void Rectangle::renderTexture() {
        if (this->cornerRadius_ > 0) {
            this->setTexture(SDLHelper::renderRoundedRect(this->w(), this->h(), this->cornerRadius_));
        } else {
            this->setTexture(SDLHelper::renderRect(this->w(), this->h()));
        }
    }

    unsigned int Rectangle::cornerRadius() {
        return this->cornerRadius_;
    }

    void Rectangle::setCornerRadius(unsigned int r) {
        this->cornerRadius_ = r;
        this->renderTexture();
    }

    void Rectangle::setW(int w) {
        Texture::setW(w);
        this->renderTexture();
    }

    void Rectangle::setH(int h) {
        Texture::setH(h);
        this->renderTexture();
    }

    Rectangle::~Rectangle() {

    }
};