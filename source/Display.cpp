#include "Display.hpp"
#include "SDLHelper.hpp"

// Struct representing a "clock", which stores time between ticks
struct Clock {
    uint32_t last_tick = 0;
    uint32_t delta = 0;

    void tick() {
        uint32_t tick = SDL_GetTicks();
        delta = tick - last_tick;
        last_tick = tick;
    }
};
static struct Clock dtClock;

namespace Aether {
    Display::Display() : Element(nullptr, 0, 0, 1280, 720) {
        // Initialize SDL (loop set to false if an error)
        this->loop_ = SDLHelper::initSDL();
    }

    void Display::setBackgroundColour(uint8_t r, uint8_t g, uint8_t b) {
        this->bg.r = r;
        this->bg.g = g;
        this->bg.b = b;
        this->bg.a = 255;
    }

    void Display::addScreen(Screen * s) {
        this->screens.push_back(s);
        this->screen = this->screens.size()-1;
    }

    bool Display::setScreen(Screen * s) {
        std::vector<Screen *>::iterator it = std::find(this->screens.begin(), this->screens.end(), s);
        if (it != this->screens.end()) {
            this->screen = std::distance(this->screens.begin(), it);
            return true;
        }

        return false;
    }

    bool Display::loop() {
        // Quit loop if no screens are added
        if (this->screens.size() == 0) {
            return false;
        }

        // Poll all events + pass

        // Update children
        dtClock.tick();
        this->screens[this->screen]->update(dtClock.delta);

        // Clear screen/draw background
        SDLHelper::setColour(this->bg);
        SDLHelper::clearScreen();
        SDLHelper::setColour(SDL_Color{255, 255, 255, 255});

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