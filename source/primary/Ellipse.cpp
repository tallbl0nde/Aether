#include "Ellipse.hpp"

namespace Aether {
    Ellipse::Ellipse(int x, int y, unsigned int xd, unsigned int yd) : Texture(x, y) {
        this->xDiameter_ = xd;
        this->yDiameter_ = ((yd == 0) ? xd : yd);
        this->redrawTexture();
    }

    void Ellipse::redrawTexture() {
        this->setTexture(SDLHelper::renderEllipse(this->xDiameter_, this->yDiameter_));
    }

    unsigned int Ellipse::xDiameter() {
        return this->xDiameter_;
    }

    void Ellipse::setXDiameter(unsigned int xd) {
        this->xDiameter_ = xd;
    }

    unsigned int Ellipse::yDiameter() {
        return this->yDiameter_;
    }

    void Ellipse::setYDiameter(unsigned int yd) {
        this->yDiameter_ = yd;
    }

    void Ellipse::renderHighlighted(Colour bg, Colour hi, Colour sel, unsigned int sz) {
        int midX = this->x() + this->w()/2;
        int midY = this->y() + this->h()/2;

        // Draw outline
        SDLHelper::drawEllipse(hi, midX, midY, this->w() + 2*sz, this->h() + 2*sz);

        // Draw background
        SDLHelper::drawEllipse(bg, midX, midY, this->w(), this->h());

        // Render this element
        this->render();

        // Draw selected overlay if selected
        if (this->selected()) {
            SDLHelper::drawEllipse(sel, midX, midY, this->w(), this->h());
        }
    }
};