#include "List.hpp"

// Dimensions
#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 500

namespace Aether {
    List::List() : Scrollable() {
        this->setW(DEFAULT_WIDTH);
        this->setH(DEFAULT_HEIGHT);
        this->setShowScrollBar(true);
    }

    void List::positionItems() {
        unsigned int pos = 0;
        for (size_t i = 0; i < this->items.size(); i++) {
            this->items[i]->setY(this->y() + pos);
            pos += this->items[i]->h();
        }
    }

    void List::addItem(ListItem * i) {
        i->setW(this->w());
        this->items.push_back(i);
        this->addElement(i);
        this->positionItems();
    }

    bool List::removeItem(ListItem * i) {
        if (this->removeElement(i)) {
            std::vector<ListItem *>::iterator it = std::find(this->items.begin(), this->items.end(), i);
            if (it != this->items.end()) {
                this->items.erase(it);
                this->positionItems();
                return true;
            }
        }
        return false;
    }

    void List::removeAllItems() {
        this->removeAllElements();
        this->items.empty();
    }
}