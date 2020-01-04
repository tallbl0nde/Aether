#include "Ellipse.hpp"

namespace Aether {
    Ellipse::Ellipse(Element * e, unsigned int xd, unsigned int yd) : Texture(e) {
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

    Ellipse::~Ellipse() {

    }
};