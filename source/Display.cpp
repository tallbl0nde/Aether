#include "Display.hpp"
#include "utils/Utils.hpp"

// Delay (in ms) to pause after button held
#define HOLD_DELAY 400
// Delay (in ms) for moving between items
#define MOVE_DELAY 100

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
    Display::Display() : Element(0, 0, 1280, 720) {
        this->setParent(nullptr);

        this->heldButton = Button::NO_BUTTON;
        this->heldTime = 0;

        this->hiAnim = [](uint32_t t){
            return Colour{255, 255, 255, 255};
        };

        // Initialize SDL (loop set to false if an error)
        this->loop_ = SDLHelper::initSDL();
    }

    void Display::setBackgroundColour(uint8_t r, uint8_t g, uint8_t b) {
        this->bg.r = r;
        this->bg.g = g;
        this->bg.b = b;
        this->bg.a = 255;
    }

    void Display::addOverlay(Overlay * o) {
        this->overlays.push_back(o);
        this->screens[this->screen]->setInactive();
        if (this->overlays.size() > 1) {
            this->overlays[this->overlays.size() - 2]->setInactive();
        }
    }

    void Display::addScreen(Screen * s) {
        this->screens.push_back(s);
        s->setParent(this);
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

    void Display::setHighlightColours(Colour bg, Colour sel) {
        this->hiBG = bg;
        this->hiSel = sel;
    }

    void Display::setHighlightAnimation(std::function<Colour(uint32_t)> f) {
        this->hiAnim = f;
    }

    bool Display::loop() {
        // Quit loop if no screens are added
        if (this->screens.size() == 0) {
            return false;
        }

        // Poll all events + pass
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_JOYBUTTONUP:
                case SDL_JOYBUTTONDOWN:
                case SDL_FINGERDOWN:
                case SDL_FINGERMOTION:
                case SDL_FINGERUP:
                    // Create InputEvent and pass to screen/overlay
                    InputEvent * event = new InputEvent(e);
                    if (this->overlays.size() == 0) {
                        this->screens[this->screen]->handleEvent(event);
                    } else {
                        this->overlays[this->overlays.size()-1]->handleEvent(event);
                    }
                    delete event;
                    break;
            }

            // (Re)set held button
            if (e.type == SDL_JOYBUTTONUP && e.jbutton.which != FAKE_ID && e.jbutton.button == this->heldButton) {
                this->heldButton = Button::NO_BUTTON;
            } else if (e.type == SDL_JOYBUTTONDOWN && e.jbutton.which != FAKE_ID) {
                Button tmp = Utils::SDLtoButton(e.jbutton.button);
                // Only add held if a directional button
                if (tmp >= Button::DPAD_LEFT && tmp <= Button::RSTICK_DOWN) {
                    this->heldButton = Utils::SDLtoButton(e.jbutton.button);
                    this->heldTime = -HOLD_DELAY;
                }
            }
        }

        // Update screems
        dtClock.tick();
        this->screens[this->screen]->update(dtClock.delta);

        // Update overlays
        for (size_t i = 0; i < this->overlays.size(); i++) {
            if (this->overlays[i]->shouldClose()) {
                delete this->overlays[i];
                this->overlays.erase(this->overlays.begin() + i);
                i--;
                if (this->overlays.size() == 0) {
                    this->screens[this->screen]->setActive();
                } else {
                    this->overlays[this->overlays.size() - 1]->setActive();
                }
                continue;
            }

            this->overlays[i]->update(dtClock.delta);
        }

        // Push button pressed/released event if held
        if (this->heldButton != Button::NO_BUTTON) {
            this->heldTime += dtClock.delta;
            if (this->heldTime >= MOVE_DELAY) {
                this->heldTime -= MOVE_DELAY;
                // Send pushed event
                SDL_Event event;
                event.type = SDL_JOYBUTTONDOWN;
                event.jbutton.which = FAKE_ID;
                event.jbutton.button = (uint8_t)this->heldButton;
                event.jbutton.state = SDL_PRESSED;
                SDL_PushEvent(&event);
                // Send released event (so basically a verrry fast button press)
                SDL_Event event2;
                event2.type = SDL_JOYBUTTONUP;
                event2.jbutton.which = FAKE_ID;
                event2.jbutton.button = (uint8_t)this->heldButton;
                event2.jbutton.state = SDL_RELEASED;
                SDL_PushEvent(&event2);
            }
        }

        // Clear screen/draw background
        SDLHelper::clearScreen(this->bg);

        // Update highlight border colour
        this->hiBorder = this->hiAnim(dtClock.last_tick);

        // Render current screen
        this->screens[this->screen]->render();

        // Draw overlays
        for (size_t i = 0; i < this->overlays.size(); i++) {
            this->overlays[i]->render();
        }

        // Draw FPS
        std::string ss = "FPS: " + std::to_string((int)(1.0/(dtClock.delta/1000.0))) + " (" + std::to_string(dtClock.delta) + " ms)";
        SDL_Texture * tt = SDLHelper::renderText(ss.c_str(), 20);
        SDLHelper::drawTexture(tt, SDL_Color{0, 150, 150, 255}, 5, 695);
        SDLHelper::destroyTexture(tt);

        SDLHelper::draw();

        return this->loop_;
    }

    void Display::exit() {
        this->loop_ = false;
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