#include "MenuSeparator.hpp"

// Pixels to pad above/below line
#define PADDING 14

namespace Aether {
    MenuSeparator::MenuSeparator(Colour c) : Element() {
        this->setH(2*PADDING + 1);
        this->rect = new Rectangle(100, 1);
        this->rect->setY(this->y() + PADDING);
        this->rect->setColour(c);
        this->addElement(this->rect);
    }

    void MenuSeparator::setW(int w) {
        Element::setW(w);
        this->rect->setW(w);
    }
};