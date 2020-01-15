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

    void List::addItem(ListItem * i) {
        this->addElement(i);
    }

    bool List::removeItem(ListItem * i) {
         return this->removeElement(i);
    }

    void List::removeAllItems() {
        this->removeAllElements();
    }
}