#include "Screen.hpp"

// Size of highlight border
#define HIGHLIGHT_SIZE 5

namespace Aether {
    Screen::Screen() : Container(0, 0, 1280, 720) {
        // Init all callbacks to nullptr to indicate none set
        for (int i = Key::A; i < Key::SR_RIGHT; i++) {
            this->callbacks[static_cast<Key>(i)] = nullptr;
        }
    }

    void Screen::setKeyCallback(Key k, std::function<void()> f) {
        this->callbacks[k] = f;
    }

    bool Screen::handleEvent(InputEvent * e) {
        // Check for callback and execute if there is one
        if (e->type() == EventType::ButtonPressed) {
            if (this->callbacks[e->button()] != nullptr) {
                this->callbacks[e->button()]();
                return true;
            }
        }

        // If no callback continue down the chain
        return Container::handleEvent(e);
    }

    void Screen::render(Colour bg, Colour hi, Colour sel) {
        // Render children
        Container::render();

        // If there is a highlighted element draw behind it and then render it
        Element * el = getHighlightedElement(this);
        if (el != nullptr) {
            el->renderHighlighted(bg, hi, sel, HIGHLIGHT_SIZE);
        }
    }
};