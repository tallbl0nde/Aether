#include "ListOption.hpp"

// Font size for text
#define FONT_SIZE 22
// Height of element
#define HEIGHT 70
// Padding on left/right of text
#define TEXT_PADDING 16

namespace Aether {
    ListOption::ListOption(std::string t, std::string v, std::function<std::string()> f) : ListItem() {
        this->setH(HEIGHT);
        this->topR = new Rectangle(this->w(), 1);
        this->bottomR = new Rectangle(this->w(), 1);
        this->bottomR->setY(this->y() + this->h());
        this->string = new Text(t, FONT_SIZE);
        this->value = new Text(v, FONT_SIZE);
        this->addElement(this->topR);
        this->addElement(this->bottomR);
        this->addElement(this->string);
        this->addElement(this->value);

        this->func = f;
        this->positionElements();
        this->setSelectable(true);
    }

    void ListOption::positionElements() {
        this->string->setX(this->x() + TEXT_PADDING);
        this->string->setY(this->y() + (this->h() - this->string->texH())/2);

        this->value->setX(this->x() + this->w() - TEXT_PADDING - this->value->w());
        this->value->setY(this->y() + (this->h() - this->value->texH())/2);

        this->topR->setW(this->w());
        this->bottomR->setW(this->w());
    }

    Colour ListOption::getLineColour() {
        return this->topR->getColour();
    }

    void ListOption::setLineColour(Colour c) {
        this->topR->setColour(c);
        this->bottomR->setColour(c);
    }

    Colour ListOption::getHintColour() {
        return this->string->getColour();
    }

    void ListOption::setHintColour(Colour c) {
        this->string->setColour(c);
    }

    Colour ListOption::getValueColour() {
        return this->value->getColour();
    }

    void ListOption::setValueColour(Colour c) {
        this->value->setColour(c);
    }

    void ListOption::setColours(Colour l, Colour h, Colour v) {
        this->setLineColour(l);
        this->setHintColour(h);
        this->setValueColour(v);
    }

    void ListOption::setW(int w) {
        ListItem::setW(w);
        this->positionElements();
    }
}