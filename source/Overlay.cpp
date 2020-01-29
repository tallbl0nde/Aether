#include "Overlay.hpp"

// Background colour
#define BG_COLOUR Colour{0, 0, 0, 150}

namespace Aether {
    SDL_Texture * Overlay::background = nullptr;

    Overlay::Overlay() : Screen() {
        // Create texture if need be
        if (this->background == nullptr) {
            this->background = SDLHelper::renderFilledRect(this->w(), this->h());
        }
        this->close_ = false;
    }

    void Overlay::close(bool b) {
        this->close_ = b;
    }

    bool Overlay::shouldClose() {
        return this->close_;
    }

    void Overlay::render() {
        // Draw background
        SDLHelper::drawTexture(this->background, BG_COLOUR, this->x(), this->y());

        // Draw elements
        Screen::render();
    }

    Overlay::~Overlay() {
        // I should really destroy the texture here but dunno how to keep it between instances so shhh...
        // if (this->background != nullptr) {
        //     SDLHelper::destroyTexture(this->background);
        //     this->background = nullptr;
        // }
    }
};