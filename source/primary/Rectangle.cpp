#include "Aether/primary/Rectangle.hpp"

namespace Aether {
    Rectangle::Rectangle(int x, int y, int w, int h, unsigned int r) : Texture(x, y) {
        Texture::setW(w);
        Texture::setH(h);
        this->setCornerRadius(r);
    }

    Drawable * Rectangle::renderDrawable() {
        if (this->cornerRadius_ > 0) {
            return this->renderer->renderFilledRoundRectTexture(this->w(), this->h(), this->cornerRadius_);
        } else {
            return this->renderer->renderFilledRectTexture(this->w(), this->h());
        }
    }

    unsigned int Rectangle::cornerRadius() {
        return this->cornerRadius_;
    }

    void Rectangle::setCornerRadius(unsigned int r) {
        this->cornerRadius_ = r;
        this->destroy();
        this->renderSync();
    }

    void Rectangle::setRectSize(int w, int h) {
        Texture::setW(w);
        Texture::setH(h);
        this->destroy();
        this->renderSync();
    }
};