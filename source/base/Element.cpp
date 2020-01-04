#include <algorithm>
#include "Element.hpp"

namespace Aether {
    Element::Element(Element * p, int x, int y, int w, int h) {
        this->x_ = x;
        this->y_ = y;
        this->w_ = w;
        this->h_ = h;
        this->scale_ = 1.00d;
        this->parent = p;
        this->hidden_ = false;
        this->callback_ = nullptr;
        this->selectable_ = false;
    }

    int Element::x() {
        return this->x_;
    }

    int Element::y() {
        return this->y_;
    }

    int Element::w() {
        return this->w_;
    }

    int Element::h() {
        return this->h_;
    }

    void Element::setX(int x) {
        this->x_ = x;
    }

    void Element::setY(int y) {
        this->y_ = y;
    }

    void Element::setW(int w) {
        this->w_ = w;
    }

    void Element::setH(int h) {
        this->h_ = h;
    }

    double Element::scale() {
        return this->scale_;
    }

    void Element::setScale(double s) {
        this->scale_ = s;
    }

    void Element::setXY(int x, int y) {
        this->x_ = x;
        this->y_ = y;
    }

    void Element::setWH(int w, int h) {
        this->w_ = w;
        this->h_ = h;
    }

    void Element::setXYWH(int x, int y, int w, int h) {
        this->setXY(x, y);
        this->setWH(w, h);
    }

    void Element::addElement(Element * e) {
        this->children.push_back(e);
    }

    bool Element::removeElement(Element * e) {
        std::vector<Element *>::iterator it = std::find(this->children.begin(), this->children.end(), e);
        if (it != this->children.end()) {
            delete (*it);
            this->children.erase(it);
            return true;
        }
        return false;
    }

    void Element::removeAllElements() {
        while (this->children.size() > 0) {
            delete this->children[0];
            this->children.erase(this->children.begin());
        }
    }

    bool Element::hidden() {
        return this->hidden_;
    }

    void Element::setHidden(bool b) {
        this->hidden_ = b;
    }

    bool Element::selectable() {
        return this->selectable_;
    }

    void Element::setSelectable(bool b) {
        this->selectable_ = b;
    }

    std::function<void()> Element::callback() {
        return this->callback_;
    }

    void Element::setCallback(std::function<void()> f) {
        this->callback_ = f;
    }

    void Element::handleEvent(InputEvent * e) {
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->handleEvent(e);
        }
    }

    void Element::update(uint32_t dt) {
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->update(dt);
        }
    }

    void Element::render() {
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->render();
        }
    }

    Element::~Element() {
        this->removeAllElements();
    }
};