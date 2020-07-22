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

    SDL_Texture * Ellipse::renderHighlightBG() {
        return SDLHelper::renderFilledEllipse(this->w()/2, this->h()/2);
    }

    SDL_Texture * Ellipse::renderHighlight() {
        return SDLHelper::renderEllipse(this->w()/2, this->h()/2, this->hiSize);
    }

    SDL_Texture * Ellipse::renderSelection() {
        return SDLHelper::renderFilledEllipse(this->w()/2, this->h()/2);
    }
};