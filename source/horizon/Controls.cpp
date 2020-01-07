#include "Controls.hpp"

namespace Aether {
    void Controls::addItem(ControlItem * i) {
        this->items.push_back(i);
        this->addElement(i);
        this->repositionElements();
    }

    bool Controls::removeItem(ControlItem * i) {
        if (Element::removeElement(i)) {
            std::vector<ControlItem *>::iterator it = std::find(this->items.begin(), this->items.end(), i);
            if (it != this->items.end()) {
                this->items.erase(it);
                this->repositionElements();
                return true;
            }
        }
        return false;
    }

    void Controls::removeAllItems() {
        Element::removeAllElements();
        this->items.empty();
    }

    void Controls::repositionElements() {
        // Iterate over children and position with first element being right-most
        int nextX = this->x() + this->w();
        for (size_t i = 0; i < this->items.size(); i++) {
            this->items[i]->setX(nextX - this->items[i]->w());
            nextX = this->items[i]->x();
            this->items[i]->setY(this->y() + (this->h() - this->items[i]->h())/2);
        }
    }

    void Controls::setX(int x) {
        Element::setX(x);
        this->repositionElements();
    }

    void Controls::setY(int y) {
        Element::setY(y);
        this->repositionElements();
    }

    void Controls::setW(int w) {
        Element::setW(w);
        this->repositionElements();
    }

    void Controls::setH(int h) {
        Element::setH(h);
        this->repositionElements();
    }

    void Controls::setXY(int x, int y) {
        Element::setX(x);
        Element::setY(y);
        this->repositionElements();
    }

    void Controls::setWH(int w, int h) {
        Element::setW(w);
        Element::setH(h);
        this->repositionElements();
    }

    void Controls::setXYWH(int x, int y, int w, int h) {
        Element::setX(x);
        Element::setY(y);
        Element::setW(w);
        Element::setH(h);
        this->repositionElements();
    }

    Colour Controls::getColour() {
        return this->colour;
    }

    Color Controls::getColor() {
        this->getColour();
    }

    void Controls::setColour(Colour c) {
        this->colour = c;
        for (size_t i = 0; i < this->items.size(); i++) {
            this->items[i]->setColour(c);
        }
    }

    void Controls::setColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->setColour(Colour{r, g, b, a});
    }

    void Controls::setColor(Color c) {
        this->setColour(c);
    }

    void Controls::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->setColour(Color{r, g, b, a});
    }
};