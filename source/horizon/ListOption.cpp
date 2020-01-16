#include "ListOption.hpp"

// Font size for text
#define FONT_SIZE 22
// Height of element
#define HEIGHT 70
// Padding on left/right of text
#define TEXT_PADDING 16

namespace Aether {
    ListOption::ListOption(std::string t, std::string v, std::function<void()> f) : Element() {
        this->setH(HEIGHT);
        this->topR = new Rectangle(this->w(), 1);
        this->bottomR = new Rectangle(this->w(), 1);
        this->bottomR->setY(this->y() + this->h());
        this->hint_ = new Text(t, FONT_SIZE);
        this->value_ = new Text(v, FONT_SIZE);
        this->addElement(this->topR);
        this->addElement(this->bottomR);
        this->addElement(this->hint_);
        this->addElement(this->value_);

        this->setCallback(f);
        this->positionElements();
    }

    void ListOption::positionElements() {
        this->hint_->setX(this->x() + TEXT_PADDING);
        this->hint_->setY(this->y() + (this->h() - this->hint_->texH())/2);

        this->value_->setX(this->x() + this->w() - TEXT_PADDING - this->value_->w());
        this->value_->setY(this->y() + (this->h() - this->value_->texH())/2);

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
        return this->hint_->getColour();
    }

    void ListOption::setHintColour(Colour c) {
        this->hint_->setColour(c);
    }

    Colour ListOption::getValueColour() {
        return this->value_->getColour();
    }

    void ListOption::setValueColour(Colour c) {
        this->value_->setColour(c);
    }

    void ListOption::setColours(Colour l, Colour h, Colour v) {
        this->setLineColour(l);
        this->setHintColour(h);
        this->setValueColour(v);
    }

    std::string ListOption::hint() {
        return this->hint_->string();
    }

    void ListOption::setHint(std::string s) {
        this->hint_->setString(s);
        this->positionElements();
    }

    std::string ListOption::value() {
        return this->value_->string();
    }

    void ListOption::setValue(std::string s) {
        this->value_->setString(s);
        this->positionElements();
    }

    void ListOption::setW(int w) {
        Element::setW(w);
        this->positionElements();
    }
}