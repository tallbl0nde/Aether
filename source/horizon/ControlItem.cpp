#include <algorithm>
#include "ControlItem.hpp"

// Font sizes
#define HINT_FONT_SIZE 22
#define ICON_FONT_SIZE 25

// "Padding" around actual textures
#define PADDING 20
// Gap between icon + hint
#define TEXT_GAP 13

// "Map" of keys to unicode characters
const std::string keyChar[] = {
    "\uE0E0", // A
    "\uE0E1", // B
    "\uE0E2", // X
    "\uE0E3", // Y
    "\uE0F7", // LSTICK
    "\uE0F8", // RSTICK
    "\uE0E4", // L
    "\uE0E5", // R
    "\uE0E6", // ZL
    "\uE0E7", // ZR
    "\uE0EF", // PLUS
    "\uE0F0", // MINUS
    "\uE0ED", // DPAD_LEFT
    "\uE0EB", // DPAD_UP
    "\uE0EE", // DPAD_RIGHT
    "\uE0EC", // DPAD_DOWN
    "", // LSTICK_LEFT
    "", // LSTICK_UP
    "", // LSTICK_RIGHT
    "", // LSTICK_DOWN
    "", // RSTICK_LEFT
    "", // RSTICK_UP
    "", // RSTICK_RIGHT
    "", // RSTICK_DOWN
    "\uE0E8", // SL_LEFT
    "\uE0E9", // SR_LEFT
    "\uE0E8", // SL_RIGHT
    "\uE0E9"  // SR_RIGHT
};

namespace Aether {
    ControlItem::ControlItem(Element * e, Key k, std::string s) : Element(e) {
        this->icon = new Text(this, keyChar[k], ICON_FONT_SIZE, FontType::Extended);
        this->hint = new Text(this, s, HINT_FONT_SIZE);
        this->setXY(this->x(), this->y());
        this->setW(PADDING*2 + this->icon->w() + TEXT_GAP + this->hint->w());
        this->setH(std::max(this->icon->h(), this->hint->h()) + PADDING*2);
        this->addElement(this->icon);
        this->addElement(this->hint);
    }

    void ControlItem::setX(int x) {
        Element::setX(x);
        this->icon->setX(this->x() + PADDING);
        this->hint->setX(this->icon->x() + this->icon->w() + TEXT_GAP);
    }

    void ControlItem::setY(int y) {
        Element::setY(y);
        this->icon->setY(this->y() + (this->h() - this->icon->h())/2);
        this->hint->setY(this->y() + (this->h() - this->hint->h())/2);
    }

    void ControlItem::setXY(int x, int y) {
        this->setX(x);
        this->setY(y);
    }

    Colour ControlItem::getColour() {
        return this->colour;
    }

    Color ControlItem::getColor() {
        this->getColour();
    }

    void ControlItem::setColour(Colour c) {
        this->colour = c;
        this->icon->setColour(c);
        this->hint->setColour(c);
    }

    void ControlItem::setColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->setColour(Colour{r, g, b, a});
    }

    void ControlItem::setColor(Color c) {
        this->setColour(c);
    }

    void ControlItem::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->setColor(Color{r, g, b, a});
    }

    ControlItem::~ControlItem() {

    }
};