#include "Aether/primary/Ellipse.hpp"

namespace Aether {
    Ellipse::Ellipse(const int x, const int y, const unsigned int xd, const unsigned int yd) : Texture(x, y) {
        this->xDiameter_ = xd;
        this->yDiameter_ = ((yd == 0) ? xd : yd);
        this->renderSync();
    }

    Drawable * Ellipse::renderDrawable() {
        return this->renderer->renderFilledEllipseTexture(this->xDiameter_/2, this->yDiameter_/2);
    }

    unsigned int Ellipse::xDiameter() {
        return this->xDiameter_;
    }

    void Ellipse::setXDiameter(const unsigned int xd) {
        this->xDiameter_ = xd;
        this->destroy();
        this->renderSync();
    }

    unsigned int Ellipse::yDiameter() {
        return this->yDiameter_;
    }

    void Ellipse::setYDiameter(const unsigned int yd) {
        this->yDiameter_ = yd;
        this->destroy();
        this->renderSync();
    }

    Drawable * Ellipse::renderHighlightBG() {
        return this->renderer->renderFilledEllipseTexture(this->w()/2, this->h()/2);
    }

    Drawable * Ellipse::renderHighlight() {
        return this->renderer->renderEllipseTexture(this->w()/2, this->h()/2, this->hiSize);
    }

    Drawable * Ellipse::renderSelection() {
        return this->renderer->renderFilledEllipseTexture(this->w()/2, this->h()/2);
    }
};