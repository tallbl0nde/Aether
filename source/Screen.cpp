#include "Aether/Screen.hpp"

// Size of highlight border
#define HIGHLIGHT_SIZE 5

namespace Aether {
    Screen::Screen() : Container(0, 0, 1280, 720) {
        // Init all handlers to nullptr to indicate none set
        for (int i = Button::A; i < Button::SR_RIGHT; i++) {
            this->onButtonPressFuncs[static_cast<Button>(i)] = nullptr;
            this->onButtonReleaseFuncs[static_cast<Button>(i)] = nullptr;
        }
    }

    void Screen::onLoad() {

    }

    void Screen::onUnload() {

    }

    void Screen::onButtonPress(Button k, std::function<void()> f) {
        this->onButtonPressFuncs[k] = f;
    }

    void Screen::onButtonRelease(Button k, std::function<void()> f) {
        this->onButtonReleaseFuncs[k] = f;
    }

    bool Screen::handleEvent(InputEvent * e) {
        // Check for handlers and execute if there is one
        if (e->type() == EventType::ButtonPressed) {
            if (this->onButtonPressFuncs[e->button()] != nullptr) {
                this->onButtonPressFuncs[e->button()]();
                return true;
            }
        } else if (e->type() == EventType::ButtonReleased) {
            if (this->onButtonReleaseFuncs[e->button()] != nullptr) {
                this->onButtonReleaseFuncs[e->button()]();
                return true;
            }
        }

        // If no handlers continue down the chain
        return Container::handleEvent(e);
    }
};