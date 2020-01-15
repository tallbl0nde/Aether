#include "Menu.hpp"

// Dimensions
#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 500

namespace Aether {
    Menu::Menu() : Scrollable() {
        this->setW(DEFAULT_WIDTH);
        this->setH(DEFAULT_HEIGHT);
        this->setShowScrollBar(false);
        this->setCatchup(15);
    }

    void Menu::addItem(MenuItem * i) {
        this->addElement(i);
    }

    bool Menu::removeItem(MenuItem * i) {
        return this->removeElement(i);
    }

    void Menu::removeAllItems() {
        this->removeAllElements();
    }
};