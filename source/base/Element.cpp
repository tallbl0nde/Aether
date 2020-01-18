#include <algorithm>
#include "Element.hpp"
#include <limits>

#include "Theme.hpp"

// Border size of highlight
#define HIGHLIGHT_SIZE 6

namespace Aether {
    Element::Element(int x, int y, int w, int h) {
        this->selectedTex = nullptr;
        this->setXYWH(x, y, w, h);

        this->parent = nullptr;
        this->hidden_ = false;
        this->callback_ = nullptr;
        this->hasSelectable_ = false;
        this->selectable_ = false;
        this->hasHighlighted_ = false;
        this->highlighted_ = false;
        this->selected_ = false;
        this->touchable_ = false;
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
        int diff = x - this->x();
        this->x_ = x;
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->setX(this->children[i]->x() + diff);
        }
    }

    void Element::setY(int y) {
        int diff = y - this->y();
        this->y_ = y;
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->setY(this->children[i]->y() + diff);
        }
    }

    void Element::setW(int w) {
        this->w_ = w;
    }

    void Element::setH(int h) {
        this->h_ = h;
    }

    void Element::setXY(int x, int y) {
        this->setX(x);
        this->setY(y);
    }

    void Element::setWH(int w, int h) {
        this->setW(w);
        this->setH(h);
    }

    void Element::setXYWH(int x, int y, int w, int h) {
        this->setXY(x, y);
        this->setWH(w, h);
    }

    void Element::setParent(Element * p) {
        this->parent = p;
    }

    void Element::addElement(Element * e) {
        e->setParent(this);
        if (e->selectable()) {
            this->setHasSelectable(true);
        }
        if (e->highlighted() || e->hasHighlighted()) {
            this->setHasHighlighted(true);
        }
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

    bool Element::isVisible() {
        if (this->hidden_ || this->x() > 1280 || this->x() + this->w() < 0 || this->y() > 720 || this->y() + this->h() < 0) {
            return false;
        }

        return true;
    }

    bool Element::hidden() {
        return this->hidden_;
    }

    void Element::setHidden(bool b) {
        this->hidden_ = b;
    }

    bool Element::selected() {
        return this->selected_;
    }

    bool Element::selectable() {
        return this->selectable_;
    }

    void Element::setSelectable(bool b) {
        this->selectable_ = b;
        this->setHasSelectable(b);
    }

    bool Element::touchable() {
        return this->touchable_;
    }

    void Element::setTouchable(bool b) {
        this->touchable_ = b;
    }

    bool Element::highlighted() {
        return this->highlighted_;
    }

    void Element::setHighlighted(bool b) {
        this->highlighted_ = b;
        if (this->parent != nullptr) {
            this->parent->setHasHighlighted(b);
        }
        if (!b) {
            this->selected_ = false;
        }
    }

    std::function<void()> Element::callback() {
        return this->callback_;
    }

    void Element::setCallback(std::function<void()> f) {
        this->callback_ = f;
        this->setSelectable(true);
        this->setTouchable(true);
    }

    bool Element::handleEvent(InputEvent * e) {
        if (this->highlighted_ && e->button() == Key::A) {
            switch(e->type()) {
                case EventType::ButtonPressed:
                    this->selected_ = true;
                    break;

                case EventType::ButtonReleased:
                    if (this->selected_) {
                        this->selected_ = false;
                        if (this->callback_ != nullptr) {
                            this->callback_();
                        }
                    }
                    break;
            }
        }
    }

    void Element::update(uint32_t dt) {
        // Do nothing if hidden or off-screen
        if (!this->isVisible()) {
            return;
        }

        // Update children
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->update(dt);
        }
    }

    bool Element::hasHighlighted() {
        return this->hasHighlighted_;
    }

    void Element::setHasHighlighted(bool b) {
        this->hasHighlighted_ = b;
        if (this->parent != nullptr) {
            this->parent->setHasHighlighted(b);
        }
    }

    bool Element::hasSelectable() {
        return this->hasSelectable_;
    }

    void Element::setHasSelectable(bool b) {
        this->hasSelectable_ = b;
        if (this->parent != nullptr) {
            this->parent->setHasSelectable(b);
        }
    }

    void Element::render() {
        // Do nothing if hidden or off-screen
        if (!this->isVisible()) {
            return;
        }

        // Draw children
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->render();
        }
    }

    void Element::renderHighlighted(Colour bg, Colour hi, Colour sel, unsigned int sz) {
        // Draw background
        SDLHelper::drawFilledRect(bg, this->x(), this->y(), this->w(), this->h());

        // Render this element
        this->render();

        // Draw outline
        SDLHelper::drawRect(hi, this->x() - sz, this->y() - sz, this->w() + 2*sz, this->h() + 2*sz, sz);

        // Draw selected overlay if selected
        if (this->selected()) {
            SDLHelper::drawFilledRect(sel, this->x(), this->y(), this->w(), this->h());
        }
    }

    void Element::setActive() {
        this->setHighlighted(true);
    }

    void Element::setInactive() {
        this->setHighlighted(false);
    }

    Element::~Element() {
        this->removeAllElements();
    }

    Element * getHighlightedElement(Element * root) {
        Element * el = root;
        while (el->hasHighlighted()) {
            for (size_t i = 0; i < el->children.size(); i++) {
                if (el->children[i]->hasHighlighted() || el->children[i]->highlighted()) {
                    el = el->children[i];
                    break;
                }
            }
        }

        if (el != root) {
            return el;
        }

        return nullptr;
    }
};