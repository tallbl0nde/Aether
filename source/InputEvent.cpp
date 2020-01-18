#include "InputEvent.hpp"
#include "Utils.hpp"

namespace Aether {
    InputEvent::InputEvent(SDL_Event e) {
        // Turn all directional events into dpad
        Key tmp = Utils::SDLtoKey(e.jbutton.button);
        if (e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP) {
            if (tmp == Key::LSTICK_LEFT || tmp == Key::RSTICK_LEFT) {
                tmp = Key::DPAD_LEFT;
            } else if (tmp == Key::LSTICK_RIGHT || tmp == Key::RSTICK_RIGHT) {
                tmp = Key::DPAD_RIGHT;
            } else if (tmp == Key::LSTICK_UP || tmp == Key::RSTICK_UP) {
                tmp = Key::DPAD_UP;
            } else if (tmp == Key::LSTICK_DOWN || tmp == Key::RSTICK_DOWN) {
                tmp = Key::DPAD_DOWN;
            }
        }

        switch (e.type) {
            // Button events
            case SDL_JOYBUTTONDOWN:
                this->type_ = ButtonPressed;
                this->button_ = tmp;
                this->touchX_ = 0;
                this->touchY_ = 0;
                this->touchDX_ = 0;
                this->touchDY_ = 0;
                break;
            case SDL_JOYBUTTONUP:
                this->type_ = ButtonReleased;
                this->button_ = tmp;
                this->touchX_ = 0;
                this->touchY_ = 0;
                this->touchDX_ = 0;
                this->touchDY_ = 0;
                break;

            // Touch events
            case SDL_FINGERDOWN:
                this->type_ = TouchPressed;
                this->button_ = Key::NO_KEY;
                this->touchX_ = e.tfinger.x * 1280;
                this->touchY_ = e.tfinger.y * 720;
                this->touchDX_ = 0;
                this->touchDY_ = 0;
                break;
            case SDL_FINGERMOTION:
                this->type_ = TouchMoved;
                this->button_ = Key::NO_KEY;
                this->touchX_ = e.tfinger.x * 1280;
                this->touchY_ = e.tfinger.y * 720;
                this->touchDX_ = e.tfinger.dx * 1280;
                this->touchDY_ = e.tfinger.dy * 720;
                break;
            case SDL_FINGERUP:
                this->type_ = TouchReleased;
                this->button_ = Key::NO_KEY;
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