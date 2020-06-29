#include "Aether/primary/Ellipse.hpp"

namespace Aether {
    Ellipse::Ellipse(int x, int y, unsigned int xd, unsigned int yd) : Texture(x, y, RenderType::OnCreate) {
        this->xDiameter_ = xd;
        this->yDiameter_ = ((yd == 0) ? xd : yd);
        this->regenerate();
    }

    void Ellipse::generateSurface() {
        // This is safe - see Texture::regenerate()
        this->texture = SDLHelper::renderFilledEllipse(this->xDiameter_/2, this->yDiameter_/2);
    }

    unsigned int Ellipse::xDiameter() {
        return this->xDiameter_;
    }

    void Ellipse::setXDiameter(unsigned int xd) {
        this->xDiameter_ = xd;
        this->regenerate();
    }

    unsigned int Ellipse::yDiameter() {
        return this->yDiameter_;
    }

    void Ellipse::setYDiameter(unsigned int yd) {
        this->yDiameter_ = yd;
        this->regenerate();
    }

    void Ellipse::renderHighlighted() {
        // Draw background
        SDLHelper::drawFilledEllipse(this->hiBG, this->x() + this->w()/2, this->y() + this->h()/2, this->w()/2, this->h()/2);

        // Draw outline
        SDLHelper::drawEllipse(this->hiBorder, this->x() + this->w()/2, this->y() + this->h()/2, this->w()/2, this->h()/2, this->hiSize);
    }

    void Ellipse::renderSelected() {
        SDLHelper::drawFilledEllipse(this->hiSel, this->x() + this->w()/2, this->y() + this->h()/2, this->w()/2, this->h()/2);
    }
};