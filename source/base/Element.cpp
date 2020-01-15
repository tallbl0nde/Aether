#include <algorithm>
#include "Element.hpp"
#include <limits>

#include "Theme.hpp"

// Border size of highlight
#define HIGHLIGHT_SIZE 6

namespace Aether {
    Element::Element(int x, int y, int w, int h) {
        this->highlightTex = nullptr;
        this->selectedTex = nullptr;
        this->setXYWH(x, y, w, h);

        this->parent = nullptr;
        this->hidden_ = false;
        this->callback_ = nullptr;
        this->selectable_ = false;
        this->hasHighlighted_ = false;
        this->hasSelectable_ = false;
        this->highlighted_ = false;
        this->selected_ = false;
        this->touchable_ = false;
    }

    void Element::generateHighlight() {
        if (this->highlightTex != nullptr) {
            SDLHelper::destroyTexture(this->highlightTex);
        }
        this->highlightTex = SDLHelper::renderRoundedBox(this->w() + 2 * HIGHLIGHT_SIZE, this->h() + 2 * HIGHLIGHT_SIZE, 5, HIGHLIGHT_SIZE);

        if (this->selectedTex != nullptr) {
            SDLHelper::destroyTexture(this->selectedTex);
        }
        this->selectedTex = SDLHelper::renderRect(this->w(), this->h());
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
        this->generateHighlight();
    }

    void Element::setH(int h) {
        this->h_ = h;
        this->generateHighlight();
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
        e->setSelectable(e->selectable());
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

    bool Element::selectable() {
        return this->selectable_;
    }

    void Element::setSelectable(bool b) {
        this->selectable_ = b;
        if (this->parent != nullptr) {
            this->parent->setHasSelectable(b);
        }
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

    bool Element::selected() {
        return this->selected_;
    }

    void Element::setSelected(bool b) {
        this->selected_ = b;
    }

    void Element::setHighlighted(bool b) {
        this->highlighted_ = b;
        if (this->parent != nullptr) {
            this->parent->setHasHighlighted(b);
        }
        if (!b) {
            this->setSelected(false);
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
        return false;
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

    void Element::render() {
        // Do nothing if hidden or off-screen
        if (!this->isVisible()) {
            return;
        }

        // Draw highlight
        if (this->selected_) {
            SDLHelper::drawTexture(this->selectedTex, Colour{200, 20, 20, 100}, this->x(), this->y());
        }

        // Draw children
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->render();
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

    void Element::setActive() {
        this->setHighlighted(true);
    }

    void Element::setInactive() {
        this->setHighlighted(false);
    }

    Element::~Element() {
        this->removeAllElements();
        if (this->highlightTex != nullptr) {
            SDLHelper::destroyTexture(this->highlightTex);
        }
    }

    Element * findElementToMoveTo(Element * curr, std::function<bool(Element *, Element *)> cmp) {
        // Get root element (screen)
        Element * rootScreen = curr;
        while (rootScreen->parent->parent != nullptr) {
            rootScreen = rootScreen->parent;
        }

        // Now with root element, search through ALL elements to find candidates
        std::vector<Element *> elms = getAllChildren(rootScreen, false);

        // Calculate distance for each selectable element and return the smallest
        Element * shortest = nullptr;
        unsigned int dist = std::numeric_limits<unsigned int>::max();
        for (size_t i = 0; i < elms.size(); i++) {
            if (elms[i]->selectable() && !elms[i]->hidden() && cmp(curr, elms[i])) {
                unsigned int d = abs(elms[i]->x() - curr->x()) + abs(elms[i]->y() - curr->y());
                if (d < dist) {
                    dist = d;
                    shortest = elms[i];
                }
            }
        }

        return shortest;
    }

    std::vector<Element *> getAllChildren(Element * root, bool add) {
        // Create initial vector (containing of root element)
        std::vector<Element *> elms;
        if (add) {
            elms.push_back(root);
        }

        // Recursively call for all children + append vectors
        for (size_t i = 0; i < root->children.size(); i++) {
            elms.push_back(root->children[i]);
            std::vector<Element *> tmp = getAllChildren(root->children[i], true);
            elms.reserve(elms.size() + tmp.size() + 1);
            elms.insert(elms.end(), tmp.begin(), tmp.end());
        }

        return elms;
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