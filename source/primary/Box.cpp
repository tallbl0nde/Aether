#include "Aether/primary/Box.hpp"

namespace Aether {
    Box::Box(const int x, const int y, const int w, const int h, const unsigned int thick, const unsigned int radius) : Texture(x, y) {
        Texture::setW(w);
        Texture::setH(h);
        this->cornerRadius_ = radius;
        this->setThickness(thick);
    }

    Drawable * Box::renderDrawable() {
        if (this->cornerRadius_ > 0) {
            return this->renderer->renderRoundRectTexture(this->w(), this->h(), this->cornerRadius_, this->border_);
        } else {
            return this->renderer->renderRectTexture(this->w(), this->h(), this->border_);
        }
    }

    unsigned int Box::thickness() {
        return this->border_;
    }

    void Box::setThickness(const unsigned int thick) {
        this->border_ = thick;
        this->destroy();
        this->renderSync();
    }

    void Box::setBoxSize(const int w, const int h) {
        Texture::setW(w);
        Texture::setH(h);
        this->destroy();
        this->renderSync();
    }

    unsigned int Box::cornerRadius() {
        return this->cornerRadius_;
    }

    void Box::setCornerRadius(const unsigned int radius) {
        this->cornerRadius_ = radius;
        this->destroy();
        this->renderSync();
    }
};
