#include <algorithm>
#include "Element.hpp"
#include <limits>

#include "Theme.hpp"

// Border size of highlight
#define HIGHLIGHT_SIZE 6

namespace Aether {
    Element::Element(int x, int y, int w, int h) {
        this->highlightTex = nullptr;
        this->setXYWH(x, y, w, h);

        this->parent = nullptr;
        this->hidden_ = false;
        this->callback_ = nullptr;
        this->selectable_ = false;
        this->highlighted_ = false;
        this->selected_ = false;
    }

    void Element::generateHighlight() {
        if (this->highlightTex != nullptr) {
            SDLHelper::destroyTexture(this->highlightTex);
        }
        this->highlightTex = SDLHelper::renderRoundedBox(this->w() + 2 * HIGHLIGHT_SIZE, this->h() + 2 * HIGHLIGHT_SIZE, 5, HIGHLIGHT_SIZE);
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

    bool Element::highlighted() {
        return this->highlighted_;
    }

    void Element::setHighlighted(bool b) {
        this->highlighted_ = b;
    }

    std::function<void()> Element::callback() {
        return this->callback_;
    }

    void Element::setCallback(std::function<void()> f) {
        this->callback_ = f;
    }

    bool Element::handleEvent(InputEvent * e) {
        // Do nothing if hidden
        if (this->hidden_) {
            return false;
        }

        // Pass event to children
        bool handled = false;
        for (size_t i = 0; i < this->children.size(); i++) {
            if (this->children[i]->handleEvent(e)) {
                handled = true;
            }
        }

        // If children didn't handle the event, let this object do so
        if (!handled) {
            // Find child thats highlighted
            Element * hid = nullptr;
            for (size_t i = 0; i < this->children.size(); i++) {
                if (this->children[i]->highlighted()) {
                    hid = this->children[i];
                }
            }

            // Return if none found
            if (hid == nullptr) {
                return false;
            }

            // Attempt to move cursor
            Element * mv = nullptr;
            switch (e->type()) {
                case EventType::ButtonPressed:
                    switch (e->button()) {
                        case Key::DPAD_RIGHT:
                            mv = findElementToMoveTo(hid, [](Element * l, Element * r) {
                                return (r->x() > l->x());
                            });
                            break;

                        case Key::DPAD_LEFT:
                            mv = findElementToMoveTo(hid, [](Element * l, Element * r) {
                                return (r->x() < l->x());
                            });
                            break;

                        case Key::DPAD_UP:
                            mv = findElementToMoveTo(hid, [](Element * a, Element * b) {
                                return (a->y() > b->y());
                            });
                            break;

                        case Key::DPAD_DOWN:
                            mv = findElementToMoveTo(hid, [](Element * a, Element * b) {
                                return (a->y() < b->y());
                            });
                            break;
                    }
                    break;
            }

            if (mv != nullptr) {
                hid->setHighlighted(false);
                mv->setHighlighted(true);
                handled = true;
            }
        }

        return handled;
    }

    void Element::update(uint32_t dt) {
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->update(dt);
        }
    }

    void Element::render() {
        // Draw highlight
        if (this->highlighted_) {
            SDLHelper::drawTexture(this->highlightTex, Theme::Dark.accent, this->x() - HIGHLIGHT_SIZE, this->y() - HIGHLIGHT_SIZE);
        }

        // Draw children
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->render();
        }
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
};