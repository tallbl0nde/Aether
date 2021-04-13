#include "Aether/horizon/button/HelpButton.hpp"

namespace Aether {
    HelpButton::HelpButton(int x, int y, int d, std::function<void()> f) : Element(x, y, d, d) {
        this->text = new Text(x, y, "\uE142", d);
        this->addElement(this->text);
        this->onPress(f);
    }

    Drawable * HelpButton::renderHighlightBG() {
        return this->renderer->renderFilledEllipseTexture(this->w()/2, this->h()/2);
    }

    Drawable * HelpButton::renderHighlight() {
        return this->renderer->renderEllipseTexture(this->w()/2, this->h()/2, this->hiSize);
    }

    Drawable * HelpButton::renderSelection() {
        return this->renderer->renderFilledEllipseTexture(this->w()/2, this->h()/2);
    }

    Colour HelpButton::colour() {
        return this->text->colour();
    }

    void HelpButton::setColour(Colour c) {
        this->text->setColour(c);
    }
};