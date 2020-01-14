#include "Menu.hpp"

// Dimensions
#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 500

namespace Aether {
    Menu::Menu() : Scrollable() {
        this->setW(DEFAULT_WIDTH);
        this->setH(DEFAULT_HEIGHT);
        this->setShowScrollBar(false);
    }

    void Menu::positionItems() {
        unsigned int pos = 0;
        for (size_t i = 0; i < this->items.size(); i++) {
            this->items[i]->setY(this->y() + pos);
            pos += this->items[i]->h();
        }
    }

    void Menu::addItem(MenuItem * i) {
        i->setW(this->w());
        this->items.push_back(i);
        Scrollable::addElement(i);
        this->positionItems();
    }

    bool Menu::removeItem(MenuItem * i) {
        if (Scrollable::removeElement(i)) {
            std::vector<MenuItem *>::iterator it = std::find(this->items.begin(), this->items.end(), i);
            if (it != this->items.end()) {
                this->items.erase(it);
                this->positionItems();
                return true;
            }
        }
        return false;
    }

    void Menu::removeAllItems() {
        Scrollable::removeAllElements();
        this->items.empty();
    }
};