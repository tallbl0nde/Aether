#include "MenuItem.hpp"

// Default dimensions
#define DEFAULT_WIDTH 300
#define DEFAULT_HEIGHT 70
#define DEFAULT_FONT_SIZE 22

namespace Aether {
    MenuItem::MenuItem(std::string s, std::function<void()> f) : Element() {
        // Create and add child elements
        this->rect = new Rectangle(4, 52);
        this->text = new Text(s, DEFAULT_FONT_SIZE);
        this->text->setScroll(true);
        this->addElement(this->rect);
        this->addElement(this->text);

        // Set size
        this->setW(DEFAULT_WIDTH);
        this->setH(DEFAULT_HEIGHT);

        // Position children
        this->rect->setX(this->x() + 8);
        this->text->setX(this->x() + 24);
        this->rect->setY(this->y() + 9);
        this->text->setY(this->y() + this->h()/2 - this->text->texH()/2);

        this->setActive(false);
        this->setCallback(f);
        this->setSelectable(true);
    }

    void MenuItem::setW(int w) {
        Element::setW(w);
        if (this->text->w() > (this->x() + this->w()) - this->text->x()) {
            this->text->setW((this->x() + this->w()) - this->text->x());
        }
    }

    void MenuItem::setWH(int w, int h){
        this->setW(w);
        this->setH(h);
    }

    void MenuItem::setXYWH(int x, int y, int w, int h) {
        this->setXY(x, y);
        this->setWH(w, h);
    }

    void MenuItem::setActive(bool b) {
        if (b) {
            this->rect->setHidden(false);
            this->text->setColour(this->activeColour);
        } else {
            this->rect->setHidden(true);
            this->text->setColour(this->inactiveColour);
        }

        this->active = b;
    }

    void MenuItem::setActiveColour(Colour c) {
        this->activeColour = c;
        this->rect->setColour(c);
        this->setActive(this->active);
    }

    void MenuItem::setActiveColor(Color c) {
        this->setActiveColour(c);
    }

    void MenuItem::setInactiveColour(Colour c) {
        this->inactiveColour = c;
        this->setActive(this->active);
    }

    void MenuItem::setInactiveColor(Color c) {
        this->setInactiveColour(c);
    }
};