#include "Display.hpp"
#include "SDLHelper.hpp"

namespace Aether {
    Display::Display() : Element(nullptr, 0, 0, 1280, 720) {
        this->screen = 0;

        // Initialize SDL (loop set to false if an error)
        this->loop_ = SDLHelper::initSDL();
    }

    void Display::addScreen(Screen * s) {
        this->screens.push_back(s);
    }

    bool Display::loop(uint32_t dt) {
        // Poll all events + pass

        // Update children
        this->screens[this->screen]->update(dt);

        // Clear screen/draw background
        SDLHelper::clearScreen();

        // Render current screen
        this->screens[this->screen]->render();

        SDLHelper::draw();

        return this->loop_;
    }

    Display::~Display() {
        while (this->screens.size() > 0) {
            delete this->screens[0];
            this->screens.erase(this->screens.begin());
        }

        // Clean up SDL
        SDLHelper::exitSDL();
    }
};