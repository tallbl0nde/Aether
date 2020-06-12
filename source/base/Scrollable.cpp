#include "Aether/base/Scrollable.hpp"

// Default catchup amount
#define DEFAULT_CATCHUP 6
// Default dampening amount
#define DEFAULT_DAMPENING 20
// Maximum scrollVelocity
#define MAX_VELOCITY 70
// Padding for very top and very bottom elements
#define PADDING 40
// Padding either side of items (to allow for scroll bar)
#define SIDE_PADDING 50
// Height of scroll bar
#define SCROLLBAR_SIZE 100
// Amount touch can deviate (in px) before scrolling
#define TOUCH_RADIUS 30

namespace Aether {
    SDL_Texture * Scrollable::scrollBar = nullptr;

    Scrollable::Scrollable(int x, int y, int w, int h, Padding p) : Container(x, y, w, h) {
        this->canScroll_ = true;
        this->isScrolling = false;
        this->isTouched = false;
        this->paddingType = p;
        this->scrollCatchup = DEFAULT_CATCHUP;
        this->scrollDampening = DEFAULT_DAMPENING;
        this->scrollVelocity = 0;
        this->scrollPos_ = 0;
        this->maxScrollPos_ = 0;
        this->showScrollBar_ = true;
        if (this->scrollBar == nullptr) {
            this->scrollBar = SDLHelper::renderFilledRect(5, SCROLLBAR_SIZE);
        }
        this->scrollBarColour = Colour{255, 255, 255, 255};
        this->touchY = std::numeric_limits<int>::min();
        this->renderTex = SDLHelper::createTexture(w, h);
    }

    int Scrollable::paddingAmount() {
        switch (this->paddingType) {
            case Padding::Default:
                return SIDE_PADDING;
                break;

            case Padding::FitScrollbar:
                return 15;
                break;

            case Padding::None:
                return 5;   // Need some padding to handle highlight
                break;
        }
    }

    void Scrollable::updateMaxScrollPos() {
        this->maxScrollPos_ = 0;

        if (this->children.size() == 0) {
            return;
        }

        this->maxScrollPos_ = 2 * PADDING;

        // Loop over child elements and determine maximum y pos
        for (size_t i = 0; i < this->children.size(); i++) {
            this->maxScrollPos_ += this->children[i]->h();
        }

        // If children don't take up enough space don't scroll!
        if (this->maxScrollPos_ <= this->h()) {
            this->maxScrollPos_ = 0;
            return;
        }

        // Subtract this element's height as it wasn't accounted for earlier
        this->maxScrollPos_ -= this->h();
    }

    void Scrollable::stopScrolling() {
        // Move highlight to top element if not visible
        if (this->isScrolling) {
            if (this->hasSelectable() && this->focused() != nullptr) {
                if (!(this->focused()->y() >= this->y() && this->focused()->y() + this->focused()->h() <= this->y() + this->h())) {
                    for (size_t i = 0; i < this->children.size(); i++) {
                        if (this->children[i]->y() > this->y() && this->children[i]->selectable()) {
                            this->setFocused(this->children[i]);
                            if (this->parent()->focused() == this) {
                                this->focused()->setActive();
                            }
                            break;
                        }
                    }
                }
            }

            this->scrollVelocity = 0;
            this->isScrolling = false;
        }
    }

    void Scrollable::setW(int w) {
        Container::setW(w);
        SDLHelper::destroyTexture(this->renderTex);
        this->renderTex = SDLHelper::createTexture(this->w(), this->h());
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->setW(this->w() - 2*this->paddingAmount());
        }
    }

    void Scrollable::setH(int h) {
        Container::setH(h);
        SDLHelper::destroyTexture(this->renderTex);
        this->renderTex = SDLHelper::createTexture(this->w(), this->h());
        this->updateMaxScrollPos();
    }

    int Scrollable::catchup() {
        return this->scrollCatchup;
    }

    void Scrollable::setCatchup(int c) {
        this->scrollCatchup = c;
    }

    float Scrollable::dampening() {
        return this->scrollDampening;
    }

    void Scrollable::setDampening(float d) {
        this->scrollDampening = d;
    }

    bool Scrollable::showScrollBar() {
        return this->showScrollBar_;
    }

    void Scrollable::setShowScrollBar(bool b) {
        this->showScrollBar_ = b;
    }

    void Scrollable::setScrollBarColour(Colour c) {
        this->scrollBarColour = c;
    }

    bool Scrollable::canScroll() {
        return this->canScroll_;
    }

    void Scrollable::setCanScroll(bool b) {
        this->canScroll_ = b;
        if (!b) {
            this->stopScrolling();
        }
    }

    int Scrollable::maxScrollPos() {
        return this->maxScrollPos_;
    }

    int Scrollable::scrollPos() {
        return this->scrollPos_;
    }

    void Scrollable::setScrollPos(int pos) {
        unsigned int old = this->scrollPos_;
        if (pos < 0) {
            this->scrollPos_ = 0;
        } else if (pos > this->maxScrollPos_) {
            this->scrollPos_ = this->maxScrollPos_;
        } else {
            this->scrollPos_ = pos;
        }

        // Update children positions
        if (old != this->scrollPos_) {
            for (size_t i = 0; i < this->children.size(); i++) {
                this->children[i]->setY(this->children[i]->y() - (this->scrollPos_ - old));
            }
        }
    }

    void Scrollable::addElement(Element * e) {
        this->addElementAt(e, this->children.size());
    }

    void Scrollable::addElementAt(Element * e, size_t i) {
        // Position element at correct position
        e->setX(this->x() + this->paddingAmount());
        if (i == 0) {
            if (this->children.empty()) {
                e->setY(this->y() + PADDING);
            } else {
                e->setY(this->children[0]->y());
            }
        } else {
            Element * last = this->children[i - 1];
            e->setY(last->y() + last->h());
        }
        e->setW(this->w() - 2*this->paddingAmount());

        Container::addElementAt(e, i);

        // Increment following children's y value
        for (size_t j = i + 1; j < this->children.size(); j++) {
            Element * last = this->children[j-1];
            this->children[j]->setY(last->y() + last->h());
        }
        this->updateMaxScrollPos();
    }

    bool Scrollable::addElementAfter(Element * e, Element * a) {
        // Handle nullptr
        if (a == nullptr) {
            Container::addElementAt(e, 0);
            return true;
        }

        // Find element to succeed
        std::vector<Element *>::iterator it = std::find(this->children.begin(), this->children.end(), a);
        if (it == this->children.end()) {
            return false;
        }

        // Insert at position
        this->addElementAt(e, std::distance(this->children.begin(), it) + 1);
        return true;
    }

    bool Scrollable::addElementBefore(Element * e, Element * b) {
        // Handle nullptr
        if (b == nullptr) {
            this->addElement(e);
            return true;
        }

        // Find element to succeed
        std::vector<Element *>::iterator it = std::find(this->children.begin(), this->children.end(), b);
        if (it == this->children.end()) {
            return false;
        }

        // Insert at position
        this->addElementAt(e, std::distance(this->children.begin(), it));
        this->setScrollPos(this->scrollPos_ + e->h());
        return true;
    }

    bool Scrollable::removeElement(Element * e) {
        std::vector<Element *>::iterator it = std::find(this->children.begin(), this->children.end(), e);
        // If the element was found loop over remaining elements and delete
        if (it != this->children.end()) {
            // Store height of element to delete
            int h = e->h();
            long d = std::distance(this->children.begin(), it);

            // If element is focussed change focus to a surrounding element if possible
            if (this->focussed() == this->children[d]) {
                this->setFocussed(nullptr);
                for (size_t i = 1; i < this->children.size()-d; i++) {
                    if (d-i < this->children.size()) {
                        if (this->children[d-i]->selectable()) {
                            this->setFocussed(this->children[d-i]);
                            break;
                        }
                    } else if (d+i < this->children.size()) {
                        if (this->children[d+i]->selectable()) {
                            this->setFocussed(this->children[d+i]);
                            break;
                        }
                    }
                }
            }

            // Delete here (don't use Container:: to avoid repeated find)
            delete this->children[d];
            this->children.erase(this->children.begin() + d);

            // Recalculate y values
            for (size_t j = d; j < this->children.size(); j++) {
                this->children[j]->setY(this->children[j]->y() - h);
            }

            // Update max pos
            this->updateMaxScrollPos();
            return true;
        }
        return false;
    }

    void Scrollable::removeAllElements() {
        this->stopScrolling();
        Container::removeAllElements();
        this->setScrollPos(0);
        this->updateMaxScrollPos();
    }

    bool Scrollable::removeElementsAfter(Element * e) {
        std::vector<Element *>::iterator it = std::find(this->children.begin(), this->children.end(), e);
        // If the element was found loop over remaining elements and delete
        if (it != this->children.end()) {
            size_t d = std::distance(this->children.begin(), it);
            d++;
            while (d < this->children.size()) {
                // If an element being removed is focussed change focus to a selectable element
                if (this->focussed() == this->children[d]) {
                    this->setFocussed(nullptr);
                    for (size_t i = d+1; i > 0; i--) {
                        if (this->children[i-1]->selectable()) {
                            this->setFocussed(this->children[i-1]);
                            break;
                        }
                    }
                }

                // We don't call Element::removeElement to avoid the repeated finds
                delete this->children[d];
                this->children.erase(this->children.begin() + d);
            }

            // Update scrolling vars
            this->updateMaxScrollPos();
            if (this->scrollPos_ > this->maxScrollPos_) {
                this->setScrollPos(this->maxScrollPos_);
            }
            return true;
        }
        return false;
    }

    bool Scrollable::removeElementsBefore(Element * e) {
        std::vector<Element *>::iterator it = std::find(this->children.begin(), this->children.end(), e);
        // If the element was found loop over remaining elements and delete
        if (it != this->children.end()) {
            long d = std::distance(this->children.begin(), it) - 1;
            if (d == -1) {
                // No need to iterate over everything if nothing to remove
                return true;
            }

            int decH = 0;
            while (d >= 0) {
                // If an element being removed is focussed change focus to a selectable element
                if (this->focussed() == this->children[d]) {
                    this->setFocussed(nullptr);
                    for (size_t i = d; i < this->children.size(); i++) {
                        if (this->children[i]->selectable()) {
                            this->setFocussed(this->children[i]);
                            break;
                        }
                    }
                }

                // We don't call Element::removeElement to avoid the repeated finds
                decH += this->children[d]->h();
                delete this->children[d];
                this->children.erase(this->children.begin() + d);
                d--;
            }

            // Recalculate y values
            for (size_t j = 0; j < this->children.size(); j++) {
                this->children[j]->setY(this->children[j]->y() - decH);
            }

            // Update scrolling vars
            this->updateMaxScrollPos();
            if (this->scrollPos_ > this->maxScrollPos_) {
                this->setScrollPos(this->maxScrollPos_);
            }
            this->setScrollPos(this->scrollPos_ - e->h());
            return true;
        }
        return false;
    }

    bool Scrollable::handleEvent(InputEvent * e) {
        switch (e->type()) {
            case EventType::TouchPressed:
                if (e->touchX() >= this->x() && e->touchX() <= this->x() + this->w() && e->touchY() >= this->y() && e->touchY() <= this->y() + this->h()) {
                    // Activate this element
                    this->isTouched = true;
                    // Note we need to traverse up the tree in order to ensure scrollable is focussed
                    Element * elm = this->parent();
                    if (elm != nullptr) {
                        while (elm->parent() != nullptr) {
                            elm->parent()->setFocused(elm);
                            elm = elm->parent();
                        }
                    }
                    // Now set scrollable focussed
                    this->parent()->setFocused(this);

                    this->touchY = e->touchY();
                    if (this->isScrolling) {
                        this->scrollVelocity = 0;
                    } else {
                        // If not scrolling pass event (ie. select)
                        Container::handleEvent(e);
                    }
                    return true;
                }
                break;

            case EventType::TouchMoved:
                if (this->isTouched) {
                    // Check touchY and change from tap to swipe if outside threshold
                    if (this->touchY != std::numeric_limits<int>::min()) {
                        if (e->touchY() > this->touchY + TOUCH_RADIUS || e->touchY() < this->touchY - TOUCH_RADIUS || e->touchX() < this->x() || e->touchX() > this->x() + this->w()) {
                            for (size_t i = 0; i < this->children.size(); i++) {
                                if (this->children[i]->selected() || this->children[i]->hasSelected()) {
                                    this->children[i]->setInactive();
                                    break;
                                }
                            }
                            this->touchY = std::numeric_limits<int>::min();
                        }
                    } else {
                        if (this->canScroll_) {
                            this->setScrollPos(this->scrollPos_ - e->touchDY());
                            this->scrollVelocity = -e->touchDY();
                            if (this->scrollVelocity > MAX_VELOCITY) {
                                this->scrollVelocity = MAX_VELOCITY;
                            } else if (this->scrollVelocity < -MAX_VELOCITY) {
                                this->scrollVelocity = -MAX_VELOCITY;
                            }
                        }
                    }

                    return true;
                }
                break;

            case EventType::TouchReleased:
                if (this->isTouched) {
                    this->isTouched = false;
                    this->touchY = std::numeric_limits<int>::min();
                    Container::handleEvent(e);
                    if (this->canScroll_) {
                        this->isScrolling = true;
                    }
                    return true;
                }
                break;

            // Buttons are handled as a container would
            default:
                this->stopScrolling();
                return Container::handleEvent(e);
                break;
        }

        return false;
    }

    void Scrollable::update(uint32_t dt) {
        // Update all children first
        Container::update(dt);

        // If scrolling due to touch event
        if (this->isScrolling) {
            this->setScrollPos(this->scrollPos_ + this->scrollVelocity);
            if (this->scrollPos_ == 0 || this->scrollPos_ == this->maxScrollPos_) {
                this->scrollVelocity = 0;
            }

            if (this->scrollVelocity < 0) {
                this->scrollVelocity += this->scrollDampening * (dt/1000.0);
            } else if (this->scrollVelocity > 0) {
                this->scrollVelocity -= this->scrollDampening * (dt/1000.0);
            }

            if (this->scrollVelocity > -1 && this->scrollVelocity < 1) {
                this->stopScrolling();
            }
        }
    }

    void Scrollable::render() {
        SDLHelper::renderToTexture(this->renderTex);
        SDL_BlendMode bld = SDLHelper::getBlendMode();
        SDLHelper::setBlendMode(SDL_BLENDMODE_NONE);
        SDLHelper::setOffset(-this->x(), -this->y());

        Container::render();

        // Reset all rendering calls to screen
        SDLHelper::setOffset(0, 0);
        SDLHelper::setBlendMode(bld);
        SDLHelper::renderToScreen();

        // Render texture
        SDLHelper::drawTexture(this->renderTex, Colour{255, 255, 255, 255}, this->x(), this->y(), this->w(), this->h());

        // Draw scroll bar
        if (this->maxScrollPos_ != 0 && this->showScrollBar_) {
            int yPos = this->y() + PADDING/2 + (((float)this->scrollPos_ / this->maxScrollPos_) * (this->h() - SCROLLBAR_SIZE - PADDING));
            SDLHelper::drawTexture(this->scrollBar, this->scrollBarColour, this->x() + this->w() - 5, yPos);
        }
    }

    Scrollable::~Scrollable() {
        // I should do this but it's static /shrug
        // SDLHelper::destroyTexture(this->scrollBar);
        SDLHelper::destroyTexture(this->renderTex);
    }
};