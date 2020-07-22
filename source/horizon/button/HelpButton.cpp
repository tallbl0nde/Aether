#include "Aether/horizon/button/HelpButton.hpp"

namespace Aether {
    HelpButton::HelpButton(int x, int y, int d, std::function<void()> f) : Element(x, y, d, d) {
        this->text = new Text(x, y, "\uE142", d);
        this->addElement(this->text);
        this->setCallback(f);
    }

    SDL_Texture * HelpButton::renderHighlightBG() {
        return SDLHelper::renderFilledEllipse(this->w()/2, this->h()/2);
    }

    SDL_Texture * HelpButton::renderHighlight() {
        return SDLHelper::renderEllipse(this->w()/2, this->h()/2, this->hiSize);
    }

    SDL_Texture * HelpButton::renderSelection() {
        return SDLHelper::renderFilledEllipse(this->w()/2, this->h()/2);
    }

    Colour HelpButton::getColour() {
        return this->text->getColour();
    }

    void HelpButton::setColour(Colour c) {
        this->text->setColour(c);
    }
};