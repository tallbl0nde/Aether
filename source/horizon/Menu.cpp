#include "Menu.hpp"

// Dimensions
#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 500
// "Padding" either side of items
#define PADDING 0.125*DEFAULT_WIDTH

namespace Aether {
    Menu::Menu() : Scrollable() {
        this->setW(DEFAULT_WIDTH);
        this->setH(DEFAULT_HEIGHT);
    }

    void Menu::repositionItems() {
        for (size_t i = 0; i < this->items.size(); i++) {
            this->items[i]->setX(this->x() + PADDING);
            this->items[i]->setY(this->y() + (i*this->items[i]->h()));
            this->items[i]->setW(this->w() - 2*PADDING);
        }
    }

    void Menu::setX(int x) {
        Scrollable::setX(x);
        this->repositionItems();
    }

    void Menu::setY(int y) {
        Scrollable::setY(y);
        this->repositionItems();
    }

    void Menu::setW(int w) {
        Scrollable::setW(w);
        this->repositionItems();
    }

    void Menu::setXY(int x, int y) {
        this->setX(x);
        this->setY(y);
    }

    void Menu::setWH(int w, int h) {
        this->setW(w);
        this->setH(h);
    }

    void Menu::setXYWH(int x, int y, int w, int h) {
        this->setXY(x, y);
        this->setWH(w, h);
    }

    void Menu::addItem(MenuItem * i) {
        this->items.push_back(i);
        Scrollable::addElement(i);
        this->repositionItems();
    }

    bool Menu::removeItem(MenuItem * i) {
        if (Scrollable::removeElement(i)) {
            std::vector<MenuItem *>::iterator it = std::find(this->items.begin(), this->items.end(), i);
            if (it != this->items.end()) {
                this->items.erase(it);
                return true;
            }
        }
        return false;
    }

    void Menu::removeAllItems() {
        Scrollable::removeAllElements();
        this->items.empty();
    }

    bool Menu::setActiveItem(MenuItem * i) {
        bool r = false;
        for (size_t j = 0; j < this->items.size(); j++) {
            if (this->items[j] == i) {
                this->items[j]->setActive(true);
                r = true;
            } else {
                this->items[j]->setActive(false);
            }
        }
        return r;
    }

    void Menu::setActiveColour(Colour c) {
        for (size_t i = 0; i < this->items.size(); i++) {
            this->items[i]->setActiveColour(c);
        }
    }

    void Menu::setActiveColor(Color c) {
        this->setActiveColour(c);
    }

    void Menu::setInactiveColour(Colour c) {
        for (size_t i = 0; i < this->items.size(); i++) {
            this->items[i]->setInactiveColour(c);
        }
    }

    void Menu::setInactiveColor(Color c) {
        this->setInactiveColour(c);
    }
};