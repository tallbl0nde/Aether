#include "Scrollable.hpp"
#include "Types.hpp"

// Default dampening amount
#define DEFAULT_DAMPENING 20
// Delay (in ms) to pause after button held
#define HOLD_DELAY 500
// Delay (in ms) for moving between items
#define MOVE_DELAY 150
// Padding for very top and very bottom elements
#define PADDING 40
// Padding either side of items (to allow for scroll bar)
#define SIDE_PADDING 50
// Height of scroll bar
#define SCROLLBAR_SIZE 100

namespace Aether {
    Scrollable::Scrollable() : Element() {
        this->isScrolling = false;
        this->scrollDampening = DEFAULT_DAMPENING;
        this->scrollVelocity = 0;
        this->scrollPos = 0;
        this->maxScrollPos = 0;
        this->showScrollBar_ = true;

        // Render scroll bar texture
        this->scrollBar = SDLHelper::renderRect(5, SCROLLBAR_SIZE);
        this->scrollBarColour = Colour{255, 255, 255, 255};
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

    void Scrollable::setH(int h) {
        Element::setH(h);
        this->updateMaxScrollPos();
    }

    void Scrollable::setWH(int w, int h) {
        Element::setWH(w, h);
        this->updateMaxScrollPos();
    }

    void Scrollable::setXYWH(int x, int y, int w, int h) {
        Element::setXYWH(x, y, w, h);
        this->updateMaxScrollPos();
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

    void Scrollable::setScrollBarColor(Color c) {
        this->setScrollBarColour(c);
    }

    void Scrollable::setScrollBarColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->setScrollBarColour(Colour{r, g, b, a});
    }

    void Scrollable::setScrollBarColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->setScrollBarColour(Colour{r, g, b, a});
    }


    void Scrollable::addElement(Element * e) {
        e->setX(this->x() + SIDE_PADDING);
        e->setW(this->w() - 2*SIDE_PADDING);
        Element::addElement(e);
        this->updateMaxScrollPos();
    }

    bool Scrollable::removeElement(Element * e) {
        bool b = Element::removeElement(e);
        if (b) {
            this->updateMaxScrollPos();
        }
        return b;
    }

    void Scrollable::removeAllElements() {
        Element::removeAllElements();
        this->updateMaxScrollPos();
    }

    // void Scrollable::event(InputEvent * e) {

    // }

    void Scrollable::update(uint32_t dt) {
        // Scrolling stuff here w/ events
        Element::update(dt);

        // Loop over items and adjust position if selected item is outside view
        int bottom = this->y() + this->h() + this->scrollPos - 2*PADDING;
        for (size_t i = 0; i < this->children.size(); i++) {
            if (this->children[i]->highlighted()) {
                if (this->children[i]->y() + this->children[i]->h() > bottom) {
                    this->scrollPos += (180 + 6*((this->children[i]->y() + this->children[i]->h()) - bottom)) * (dt/1000.0);
                } else if (this->children[i]->y() < this->y() + this->scrollPos) {
                    this->scrollPos -= (180 + 6*((this->y() + this->scrollPos) - this->children[i]->y())) * (dt/1000.0);
                }
                break;
            }
        }
    }

    void Scrollable::render() {
        // Create blank texture (size of element) + render to it
        SDL_Texture * t = SDLHelper::createTexture(1280, 720);
        SDLHelper::renderToTexture(t);
        SDLHelper::setOffset(0, -this->scrollPos);

        Element::render();

        // Reset all rendering calls to screen
        SDLHelper::resetOffset();
        SDLHelper::renderToScreen();

        // Only render applicable part of texture
        SDLHelper::drawTexture(t, Colour{255, 255, 255, 255}, this->x(), this->y(), this->w(), this->h(), this->x(), this->y() - PADDING, this->w(), this->h());
        SDLHelper::destroyTexture(t);

        // Draw scroll bar
        if (this->maxScrollPos != 0 && this->showScrollBar_) {
            int yPos = this->y() + (((float)this->scrollPos / this->maxScrollPos) * (this->h() - SCROLLBAR_SIZE - PADDING*2));
            SDLHelper::drawTexture(this->scrollBar, this->scrollBarColour, this->x() + this->w() - 5, yPos);
        }
    }

    Scrollable::~Scrollable() {
        SDLHelper::destroyTexture(this->scrollBar);
    }
};