#include "Rectangle.hpp"

namespace Aether {
    Rectangle::Rectangle(int x, int y, int w, int h, unsigned int r) : Texture(x, y, RenderType::OnCreate) {
        Texture::setW(w);
        Texture::setH(h);
        this->setCornerRadius(r);
    }

    void Rectangle::generateSurface() {
        // This is safe - see Texture::regenerate()
        if (this->cornerRadius_ > 0) {
            this->texture = SDLHelper::renderFilledRoundRect(this->w(), this->h(), this->cornerRadius_);
        } else {
            this->texture = SDLHelper::renderFilledRect(this->w(), this->h());
        }
    }

    unsigned int Rectangle::cornerRadius() {
        return this->cornerRadius_;
    }

    void Rectangle::setCornerRadius(unsigned int r) {
        this->cornerRadius_ = r;
        this->regenerate();
    }

    void Rectangle::setRectSize(int w, int h) {
        Texture::setW(w);
        Texture::setH(h);
        this->regenerate();
    }
};