#include "Aether/horizon/controls/Controls.hpp"

namespace Aether {
    Controls::Controls(int x, int y, int w, int h) : Container(x, y, w, h) {

    }

    void Controls::addItem(ControlItem * i) {
        this->addElement(i);
        this->repositionElements();
    }

    bool Controls::removeItem(ControlItem * i) {
        if (Container::removeElement(i)) {
            this->repositionElements();
            return true;
        }
        return false;
    }

    void Controls::removeAllItems() {
        Container::removeAllElements();
    }

    bool Controls::returnItem(ControlItem * i) {
        if (Container::returnElement(i)) {
            this->repositionElements();
            return true;
        }
        return false;
    }

    void Controls::returnAllItems() {
        Container::returnAllElements();
    }

    void Controls::repositionElements() {
        // Iterate over children and position with first element being right-most
        int nextX = this->x() + this->w();
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->setX(nextX - this->children[i]->w());
            nextX = this->children[i]->x();
            this->children[i]->setY(this->y() + (this->h() - this->children[i]->h())/2);
        }
    }

    void Controls::setX(int x) {
        Container::setX(x);
    }

    void Controls::setY(int y) {
        Container::setY(y);
    }

    void Controls::setW(int w) {
        Container::setW(w);
        this->repositionElements();
    }

    void Controls::setH(int h) {
        Container::setH(h);
        this->repositionElements();
    }

    Colour Controls::getColour() {
        return this->colour;
    }

    void Controls::setColour(Colour c) {
        this->colour = c;
        for (size_t i = 0; i < this->children.size(); i++) {
            // If used correctly, this should never cause an error
            static_cast<ControlItem *>(this->children[i])->setColour(c);
        }
    }
};