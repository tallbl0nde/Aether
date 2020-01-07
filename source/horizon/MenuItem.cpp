#include "MenuItem.hpp"

// Default dimensions
#define DEFAULT_WIDTH 300
#define DEFAULT_HEIGHT 70
#define DEFAULT_FONT_SIZE 22

namespace Aether {
    MenuItem::MenuItem(std::string s, std::function<void()> f) : Element() {
        // Create children
        this->rect = new Rectangle(4, 52);
        this->addElement(this->rect);
        this->text = new Text(s, DEFAULT_FONT_SIZE);
        this->text->setScroll(true);
        this->addElement(this->text);

        // Position children
        this->setXYWH(this->x(), this->y(), DEFAULT_WIDTH, DEFAULT_HEIGHT);

        this->setActive(false);
        this->setCallback(f);
        this->setSelectable(true);
    }

    void MenuItem::setX(int x) {
        Element::setX(x);
        this->rect->setX(this->x() + 8);
        this->text->setX(this->x() + 24);
    }

    void MenuItem::setY(int y) {
        Element::setY(y);
        this->rect->setY(this->y() + 9);
        this->text->setY(this->y() + this->h()/2 - this->text->texH()/2);
    }

    void MenuItem::setW(int w) {
        Element::setW(w);
        if (this->text->w() > this->w() - this->text->x()) {
            this->text->setW(this->w() - this->text->x());
        }
    }

    void MenuItem::setXY(int x, int y) {
        this->setX(x);
        this->setY(y);
    }

    void MenuItem::setXYWH(int x, int y, int w, int h) {
        this->setXY(x, y);
        this->setW(w);
        this->setH(h);
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