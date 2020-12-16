#include "Aether/primary/Box.hpp"

namespace Aether {
    Box::Box(int x, int y, int w, int h, unsigned int b, unsigned int r) : Texture(x, y, RenderType::OnCreate) {
        Texture::setW(w);
        Texture::setH(h);
        this->cornerRadius_ = r;
        this->setBorder(b);
    }

    void Box::generateSurface() {
        // This is safe - see Texture::regenerate()
        if (this->cornerRadius_ > 0) {
            this->drawable = this->renderer->renderRoundRectTexture(this->w(), this->h(), this->cornerRadius_, this->border());
        } else {
            this->drawable = this->renderer->renderRectTexture(this->w(), this->h(), this->border());
        }
    }

    unsigned int Box::border() {
        return this->border_;
    }

    void Box::setBorder(unsigned int b) {
        this->border_ = b;
        this->regenerate();
    }

    void Box::setBoxSize(int w, int h) {
        Texture::setW(w);
        Texture::setH(h);
        this->regenerate();
    }

    unsigned int Box::cornerRadius() {
        return this->cornerRadius_;
    }

    void Box::setCornerRadius(unsigned int r) {
        this->cornerRadius_ = r;
        this->regenerate();
    }
};
