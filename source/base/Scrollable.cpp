#include "Scrollable.hpp"

// Default catchup amount
#define DEFAULT_CATCHUP 6
// Default dampening amount
#define DEFAULT_DAMPENING 20
// Padding for very top and very bottom elements
#define PADDING 40
// Padding either side of items (to allow for scroll bar)
#define SIDE_PADDING 50
// Height of scroll bar
#define SCROLLBAR_SIZE 100

namespace Aether {
    Scrollable::Scrollable(int x, int y, int w, int h) : Container(x, y, w, h) {
        this->isScrolling = false;
        this->scrollCatchup = DEFAULT_CATCHUP;
        this->scrollDampening = DEFAULT_DAMPENING;
        this->scrollVelocity = 0;
        this->scrollPos = 0;
        this->maxScrollPos = 0;
        this->showScrollBar_ = true;

        // Render scroll bar texture
        this->scrollBar = SDLHelper::renderFilledRect(5, SCROLLBAR_SIZE);
        this->scrollBarColour = Colour{255, 255, 255, 255};
    }

    void Scrollable::setScrollPos(int pos) {
        unsigned int old = this->scrollPos;
        if (pos < 0) {
            this->scrollPos = 0;
        } else if (pos > this->maxScrollPos + 2*PADDING) {
            this->scrollPos = this->maxScrollPos + 2*PADDING;
        } else {
            this->scrollPos = pos;
        }

        // Update children positions
        if (old != this->scrollPos) {
            for (size_t i = 0; i < this->children.size(); i++) {
                this->children[i]->setY(this->children[i]->y() - (this->scrollPos - old));
            }
        }
    }

    void Scrollable::updateMaxScrollPos() {
        this->maxScrollPos = 0;

        if (this->children.size() == 0) {
            return;
        }

        // Loop over child elements and determine maximum y pos
        for (size_t i = 0; i < this->children.size(); i++) {
            this->maxScrollPos += this->children[i]->h();
        }

        // If children don't take up enough space don't scroll!
        if (this->maxScrollPos <= this->h()) {
            this->maxScrollPos = 0;
            return;
        }

        // Subtract this element's height as it wasn't accounted for earlier
        this->maxScrollPos -= this->h();
    }

    void Scrollable::setW(int w) {
        Container::setW(w);
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->setW(this->w() - 2*SIDE_PADDING);
        }
    }

    void Scrollable::setH(int h) {
        Container::setH(h);
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

    void Scrollable::setScrollBarColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->setScrollBarColour(Colour{r, g, b, a});
    }

    void Scrollable::addElement(Element * e) {
        // Position element at correct position
        e->setX(this->x() + SIDE_PADDING);
        if (this->children.size() == 0) {
            e->setY(this->y() + PADDING);
        } else {
            e->setY(this->children[this->children.size() - 1]->y() + this->children[this->children.size() - 1]->h());
        }
        e->setW(this->w() - 2*SIDE_PADDING);

        Container::addElement(e);
        this->updateMaxScrollPos();
    }

    bool Scrollable::removeElement(Element * e) {
        bool b = Container::removeElement(e);
        if (b) {
            this->updateMaxScrollPos();
        }
        return b;
    }

    void Scrollable::removeAllElements() {
        Container::removeAllElements();
        this->updateMaxScrollPos();
    }

    void Scrollable::update(uint32_t dt) {
        // Update all children first
        Container::update(dt);

        if (this->maxScrollPos != 0 && this->focussed != nullptr) {
            int sMid = this->y() + this->h()/2;
            int cMid = this->focussed->y() + (this->focussed->h()/2);
            this->setScrollPos(this->scrollPos + (this->scrollCatchup * (cMid - sMid) * (dt/1000.0)));
        }
    }

    void Scrollable::render() {
        // Create blank texture (size of element) + render to it
        SDL_Texture * t = SDLHelper::createTexture(1280, 720);
        SDLHelper::renderToTexture(t);

        Container::render();

        // Reset all rendering calls to screen
        SDLHelper::renderToScreen();

        // Only render applicable part of texture
        SDLHelper::drawTexture(t, Colour{255, 255, 255, 255}, this->x(), this->y(), this->w(), this->h(), this->x(), this->y(), this->w(), this->h());
        SDLHelper::destroyTexture(t);

        // Draw scroll bar
        if (this->maxScrollPos != 0 && this->showScrollBar_) {
            int yPos = this->y() + (((float)this->scrollPos / (this->maxScrollPos + 2*PADDING)) * (this->h() - SCROLLBAR_SIZE));
            SDLHelper::drawTexture(this->scrollBar, this->scrollBarColour, this->x() + this->w() - 5, yPos);
        }
    }

    Scrollable::~Scrollable() {
        SDLHelper::destroyTexture(this->scrollBar);
    }
};