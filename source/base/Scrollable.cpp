#include "Scrollable.hpp"
#include "Types.hpp"

// Default dampening amount
#define DEFAULT_DAMPENING 20
// Delay (in ms) to pause after button held
#define HOLD_DELAY 500
// Delay (in ms) for moving between items
#define MOVE_DELAY 150

namespace Aether {
    Scrollable::Scrollable(Element * e) : Element(e) {
        this->isScrolling = false;
        this->scrollDampening = DEFAULT_DAMPENING;
        this->scrollVelocity = 0;
        this->scrollPos = 0;
        this->maxScrollPos = 0;
        this->showScrollBar_ = true;
    }

    void Scrollable::updateMaxScrollPos() {
        this->maxScrollPos = 0;

        // Loop over child elements and determine maximum y pos
        for (size_t i = 0; i < this->children.size(); i++) {
            unsigned int pos = this->children[i]->y() + this->children[i]->h();
            if (pos > this->maxScrollPos) {
                this->maxScrollPos = pos;
            }
        }

        // If children don't take up enough space don't scroll!
        if (this->maxScrollPos <= this->h()) {
            this->maxScrollPos = 0;
            return;
        }

        // Subtract this element's height as it wasn't accounted for earlier
        this->maxScrollPos -= this->h();
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

    void Scrollable::addElement(Element * e) {
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

    void Scrollable::update(uint32_t dt) {
        // Scrolling stuff here w/ events
        Element::update(dt);
    }

    void Scrollable::render() {
        // Create blank texture (size of screen) + render to it
        SDL_Texture * t = SDLHelper::createTexture(1280, 720);
        SDLHelper::renderToTexture(t);

        Element::render();

        // Reset all rendering calls to screen
        SDLHelper::renderToScreen();

        // Only render applicable part of texture
        SDLHelper::drawTexture(t, Colour{255, 255, 255, 255}, this->x(), this->y(), this->w(), this->h(), this->x(), this->y() + this->scrollPos, this->w(), this->h());
        SDLHelper::destroyTexture(t);
    }

    Scrollable::~Scrollable() {

    }
};