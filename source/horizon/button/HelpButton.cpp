#include "Aether/horizon/button/HelpButton.hpp"

namespace Aether {
    HelpButton::HelpButton(int x, int y, int d, std::function<void()> f) : Element(x, y, d, d) {
        this->text = new Text(x, y, "\uE142", d);
        this->addElement(this->text);
        this->setCallback(f);
    }

    void HelpButton::renderHighlightBG() {
        SDLHelper::drawFilledEllipse(this->hiBG, this->x() + this->w()/2, this->y() + this->h()/2, this->w()/2, this->h()/2);
    }

    void HelpButton::renderHighlight() {
        SDLHelper::drawEllipse(this->hiBorder, this->x() + this->w()/2, this->y() + this->h()/2, this->w()/2, this->h()/2, this->hiSize);
    }

    void HelpButton::renderSelection() {
        SDLHelper::drawFilledEllipse(this->hiSel, this->x() + this->w()/2, this->y() + this->h()/2, this->w()/2, this->h()/2);
    }

    Colour HelpButton::getColour() {
        return this->text->getColour();
    }

    void HelpButton::setColour(Colour c) {
        this->text->setColour(c);
    }
};