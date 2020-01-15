#include "InputEvent.hpp"

namespace Aether {
    static Key SDLtoKey(uint8_t k) {
        return (Key)k;
    }

    InputEvent::InputEvent(SDL_Event e) {
        switch (e.type) {
            // Button events
            case SDL_JOYBUTTONDOWN:
                this->type_ = ButtonPressed;
                this->button_ = SDLtoKey(e.jbutton.button);
                break;
            case SDL_JOYBUTTONUP:
                this->type_ = ButtonReleased;
                this->button_ = SDLtoKey(e.jbutton.button);
                break;

            // Touch events
            case SDL_FINGERDOWN:
                this->type_ = TouchPressed;
                this->touchX_ = e.tfinger.x * 1280;
                this->touchY_ = e.tfinger.y * 720;
                this->touchDX_ = 0;
                this->touchDY_ = 0;
                break;
            case SDL_FINGERMOTION:
                this->type_ = TouchMoved;
                this->touchX_ = e.tfinger.x * 1280;
                this->touchY_ = e.tfinger.y * 720;
                this->touchDX_ = e.tfinger.dx * 1280;
                this->touchDY_ = e.tfinger.dy * 720;
                break;
            case SDL_FINGERUP:
                this->type_ = TouchReleased;
                this->touchX_ = e.tfinger.x * 1280;
                this->touchY_ = e.tfinger.y * 720;
                this->touchDX_ = e.tfinger.dx * 1280;
                this->touchDY_ = e.tfinger.dy * 720;
                break;
        }
    }

    EventType InputEvent::type() {
        return this->type_;
    }

    Key InputEvent::button() {
        return this->button_;
    }

    int InputEvent::touchX() {
        return this->touchX_;
    }

    int InputEvent::touchY() {
        return this->touchY_;
    }

    int InputEvent::touchDX() {
        return this->touchDX_;
    }

    int InputEvent::touchDY() {
        return this->touchDY_;
    }

    InputEvent::~InputEvent() {

    }
};