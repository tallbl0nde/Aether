#include <algorithm>
#include "Aether/base/Element.hpp"
#include <limits>

// Border size of highlight
#define HIGHLIGHT_SIZE 6
// Corner radius of highlight
#define HIGHLIGHT_RAD 4

namespace Aether {
    Colour Element::hiBGColour = Colour(255, 255, 255, 255);
    Colour Element::hiBorderColour = Colour(255, 255, 255, 255);
    Colour Element::selColour = Colour(255, 255, 255, 255);
    unsigned int Element::hiSize = HIGHLIGHT_SIZE;
    Drawable * Element::hiBGTex = nullptr;
    Drawable * Element::hiBorderTex = nullptr;
    Drawable * Element::selTex = nullptr;
    Renderer * Element::renderer = nullptr;
    Element * Element::hiOwner = nullptr;
    Element * Element::selOwner = nullptr;
    bool Element::isTouch = false;

    Element::Element(int x, int y, int w, int h) {
        this->setXYWH(x, y, w, h);

        this->parent_ = nullptr;
        this->hidden_ = false;
        this->onPressFunc_ = nullptr;
        this->hasSelectable_ = false;
        this->selectable_ = false;
        this->hasHighlighted_ = false;
        this->highlighted_ = false;
        this->selected_ = false;
        this->hasSelected_ = false;
        this->touchable_ = false;

        this->focused_ = nullptr;
    }

    void Element::renderHighlightTextures() {
        // Don't recreate unless focus changed
        if (this->hiOwner == this) {
            return;
        }

        delete this->hiBorderTex;
        this->hiBorderTex = this->renderHighlight();
        delete this->hiBGTex;
        this->hiBGTex = this->renderHighlightBG();

        this->hiOwner = this;
    }

    void Element::renderSelectionTexture() {
        // Don't recreate unless focus changed
        if (this->selOwner == this) {
            return;
        }

        delete this->selTex;
        this->selTex = this->renderSelection();

        this->selOwner = this;
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

    Element * Element::parent() {
        return this->parent_;
    }

    void Element::setParent(Element * p) {
        this->parent_ = p;
    }

    void Element::addElement(Element * e) {
        this->addElementAt(e, this->children.size());
    }

    void Element::addElementAt(Element * e, size_t i) {
        // If wanting to insert at end do just that!
        if (i > this->children.size()) {
            i = this->children.size();
        }

        // Otherwise insert and perform the necessary checks
        e->setParent(this);
        if (e->selectable() || e->hasSelectable()) {
            this->setHasSelectable(true);
        }
        if (e->highlighted() || e->hasHighlighted()) {
            this->setHasHighlighted(true);
        }
        this->children.insert(this->children.begin() + i, e);
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
        for (size_t i = 0; i < this->children.size(); i++) {
            delete this->children[i];
        }
        this->children.clear();
    }

    bool Element::returnElement(Element * e) {
        std::vector<Element *>::iterator it = std::find(this->children.begin(), this->children.end(), e);
        if (it != this->children.end()) {
            this->children.erase(it);
            e->setParent(nullptr);
            return true;
        }
        return false;
    }

    void Element::returnAllElements() {
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->setParent(nullptr);
        }
        this->children.clear();
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

    void Element::setSelected(bool b) {
        this->selected_ = b;
        if (this->parent_ != nullptr) {
            this->parent_->setHasSelected(b);
        }
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
        if (this->parent_ != nullptr) {
            this->parent_->setHasHighlighted(b);
        }
        if (!b) {
            this->setSelected(false);
        }
    }

    std::function<void()> Element::onPressFunc() {
        return this->onPressFunc_;
    }

    void Element::onPress(std::function<void()> f) {
        this->onPressFunc_ = f;
        this->setSelectable(true);
        this->setTouchable(true);
    }

    bool Element::handleEvent(InputEvent * e) {
        // Handles selecting by either touch or A
        switch (e->type()) {
            case EventType::ButtonPressed:
                if (e->button() == Button::A && this->highlighted_) {
                    this->setSelected(true);
                    return true;
                }
                break;

            case EventType::ButtonReleased:
                if (e->button() == Button::A && this->selected_) {
                    this->setSelected(false);
                    if (this->onPressFunc_ != nullptr) {
                        this->onPressFunc_();
                        return true;
                    }
                }
                break;

            case EventType::TouchPressed:
                if (e->touchX() >= this->x() && e->touchY() >= this->y() && e->touchX() <= this->x() + this->w() && e->touchY() <= this->y() + this->h() && this->touchable_ && !this->hidden()) {
                    this->setSelected(true);
                    return true;
                }
                break;

            case EventType::TouchMoved:
                if ((e->touchX() < this->x() || e->touchY() < this->y() || e->touchX() > this->x() + this->w() || e->touchY() > this->y() + this->h()) && this->selected_) {
                    if (e->touchX() - e->touchDX() >= this->x() && e->touchY() - e->touchDY() >= this->y() && e->touchX() - e->touchDX() <= this->x() + this->w() && e->touchY() - e->touchDY() <= this->y() + this->h()) {
                        this->setSelected(false);
                        return true;
                    }
                }
                break;

            case EventType::TouchReleased:
                if (e->touchX() >= this->x() && e->touchY() >= this->y() && e->touchX() <= this->x() + this->w() && e->touchY() <= this->y() + this->h() && this->selected_) {
                    this->setSelected(false);
                    if (this->selectable_) {
                        moveHighlight(this);
                    }
                    if (this->onPressFunc_ != nullptr) {
                        this->onPressFunc_();
                    }
                    return true;
                }
                break;
        }

        return false;
    }

    void Element::update(unsigned int dt) {
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
        if (this->parent_ != nullptr) {
            this->parent_->setHasHighlighted(b);
        }
    }

    bool Element::hasSelectable() {
        return this->hasSelectable_;
    }

    void Element::setHasSelectable(bool b) {
        this->hasSelectable_ = b;
        if (this->parent_ != nullptr) {
            this->parent_->setHasSelectable(b);
        }
    }

    bool Element::hasSelected() {
        return this->hasSelected_;
    }

    void Element::setHasSelected(bool b) {
        this->hasSelected_ = b;
        if (this->parent_ != nullptr) {
            this->parent_->setHasSelected(b);
        }
    }

    void Element::render() {
        // Do nothing if hidden or off-screen
        if (!this->isVisible()) {
            return;
        }

        // Render highlight background if highlighted
        int w, h;
        if (this->highlighted() && !this->isTouch) {
            this->renderHighlightTextures();
            this->hiBGTex->setColour(this->hiBGColour);
            this->hiBGTex->render(this->x_ + (this->w_ - this->hiBGTex->width())/2, this->y_ + (this->h_ - this->hiBGTex->height())/2);
        }

        // Render children next
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->render();
        }

        // Render selected/held layer
        if (this->selected()) {
            this->renderSelectionTexture();
            this->selTex->setColour(this->selColour);
            this->selTex->render(this->x_ + (this->w_ - this->selTex->width())/2, this->y_ + (this->h_ - this->selTex->height())/2);
        }

        // Finally render highlight border if needed
        if (this->highlighted() && !this->isTouch) {
            this->hiBorderTex->setColour(this->hiBorderColour);
            this->hiBorderTex->render(this->x_ + (this->w_ - this->hiBorderTex->width())/2, this->y_ + (this->h_ - this->hiBorderTex->height())/2);
        }
    }

    Drawable * Element::renderHighlightBG() {
        return this->renderer->renderFilledRectTexture(this->w(), this->h());
    }

    Drawable * Element::renderHighlight() {
        return this->renderer->renderRoundRectTexture(this->w() + 2*this->hiSize, this->h() + 2*this->hiSize, HIGHLIGHT_RAD, this->hiSize);
    }

    Drawable * Element::renderSelection() {
        return this->renderer->renderFilledRectTexture(this->w(), this->h());
    }

    void Element::setActive() {
        this->setHighlighted(true);
    }

    void Element::setInactive() {
        this->setHighlighted(false);
    }

    void Element::setFocused(Element * e) {
        if (this->focused_ != nullptr) {
            this->focused_->setInactive();
        }
        this->focused_ = e;

        if (e == nullptr) {
            return;
        }
        e->setInactive();

        // Climb up the tree until we find the root
        Element * last = e;
        Element * next = e->parent();
        while (next != nullptr) {
            // Stop if not focussed
            if (next->focused() != last) {
                return;
            }
            last = next;
            next = next->parent();
        }

        // If we've reached the root then we need to set it active
        e->setActive();
    }

    void Element::setFocussed(Element * e) {
        this->setFocused(e);
    }

    Element * Element::focused() {
        return this->focused_;
    }

    Element * Element::focussed() {
        return this->focused();
    }

    Element::~Element() {
        if (this->parent_ != nullptr) {
            if (this->parent_->focused() == this) {
                this->parent_->setFocused(nullptr);
            }
        }
        this->removeAllElements();

        // Covers an edge case where an element could get the same address as the deleted element
        // and thus would cause the highlight to not be regenerated
        if (this->hiOwner == this) {
            this->hiOwner = nullptr;
        }
        if (this->selOwner == this) {
            this->selOwner = nullptr;
        }
    }

    void moveHighlight(Element * e) {
        Element * next = e;

        // First get root element (screen)
        while (e->parent_->parent_ != nullptr) {
            e = e->parent_;
        }

        // Set inactive
        e->setInactive();

        // Set focused up the tree
        e = next;
        while (e->parent_ != nullptr) {
            e->parent_->setFocused(e);
            e = e->parent_;
        }
    }
};