#include "Aether/Overlay.hpp"
#include "Aether/Screen.hpp"
#include "Aether/ThreadPool.hpp"
#include "Aether/types/Timer.hpp"
#include "Aether/utils/Utils.hpp"
#include "Aether/Window.hpp"

// Font size for debug info
static constexpr unsigned int debugFontSize = 18;

// We want a highlight border of 6px
static constexpr unsigned int defaultHighlightBorder = 6;

// Default hold delay is 100 ms
static constexpr int defaultHoldDelay = 100;

// Default 'mark as held' delay is 400 ms
static constexpr int defaultMarkHeldDelay = 400;

// Amount to vary fade alpha by per second
static constexpr short fadeDelta = 800;

namespace Aether {
    Window::Window(const std::string & name, const unsigned int width, const unsigned int height, const LogHandler & log) {
        // Initially have a black background
        this->bgColour = Colour(0, 0, 0, 255);
        this->bgDrawable = new Drawable();

        // Have no fade or debug shown
        this->fade.action = FadeAction::None;
        this->fade.alpha = 0;
        this->fade.in = false;
        this->fade.out = false;
        this->showDebug = false;
        this->lastMillis = 0;
        this->timer = nullptr;

        // No button is held down initially
        this->heldButton = Button::NO_BUTTON;
        this->heldTime = 0;
        this->holdDelay_ = defaultHoldDelay;

        // Start with white highlight colours (we are a friend of Element)
        Element::hiBGColour = Colour(255, 255, 255, 50);
        Element::hiBorderColour = Colour(255, 255, 255, 255);
        Element::selColour = Colour(255, 255, 255, 150);
        Element::hiSize = defaultHighlightBorder;
        this->highlight = [](const uint32_t t) {
            return Colour(255, 255, 255, 255);
        };

        // Initialize the renderer
        this->screen = nullptr;
        Element::renderer = new Renderer();
        Element::renderer->setLogHandler(log);
        this->shouldLoop = Element::renderer->initialize(name, width, height);

        // Create our thread pool
        log("Created a thread pool with size " + std::to_string(ThreadPool::getInstance()->maxConcurrentJobs()), false);
    }

    void Window::performScreenOps() {
        while (!this->screenOps.empty()) {
            std::pair<ScreenOperation, Screen *> item = this->screenOps.front();
            switch (item.first) {
                // Push screen on stack
                case ScreenOperation::Push:
                    this->screenStack.push(this->screen);
                    this->screen = nullptr;
                    break;

                // Pop screen from stack
                case ScreenOperation::Pop:
                    if (!this->screenStack.empty()) {
                        if (this->screen != nullptr) {
                            this->screen->onUnload();
                        }
                        this->screen = this->screenStack.top();
                        this->screenStack.pop();
                    }
                    break;

                // Set screen stored in queue
                case ScreenOperation::Set:
                    if (this->screen != nullptr) {
                        this->screen->onUnload();
                    }
                    this->screen = item.second;
                    if (this->screen != nullptr) {
                        this->screen->onLoad();
                    }
                    break;
            }
            this->screenOps.pop();
        }
    }

    void Window::processEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                        this->exit();
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    // Make a fake touch event
                    SDL_Event event2;
                    if (e.type == SDL_MOUSEMOTION) {
                        // event2.type = SDL_FINGERMOTION;
                        // event2.tfinger.x = (e.motion.x/Element::renderer->windowWidth());
                        // event2.tfinger.y = (e.motion.y/Element::renderer->windowHeight());
                        // event2.tfinger.dy = (e.motion.yrel/Element::renderer->windowHeight());
                        // event2.tfinger.dx = (e.motion.xrel/Element::renderer->windowWidth());

                    } else {
                        event2.type = (e.type == SDL_MOUSEBUTTONUP ? SDL_FINGERUP : SDL_FINGERDOWN);
                        event2.tfinger.x = (e.button.x/(float)Element::renderer->windowWidth());
                        event2.tfinger.y = (e.button.y/(float)Element::renderer->windowHeight());
                        event2.tfinger.dy = 0;
                        event2.tfinger.dx = 0;
                    }

                    SDL_PushEvent(&event2);
                    break;

                case SDL_JOYBUTTONUP:
                case SDL_JOYBUTTONDOWN:
                case SDL_FINGERDOWN:
                case SDL_FINGERMOTION:
                case SDL_FINGERUP:
                    // Create InputEvent and pass to screen/overlay
                    InputEvent * event = new InputEvent(e);

                    // Set touched variable
                    bool old = Element::isTouch;
                    Element::isTouch = (event->button() == Button::NO_BUTTON);

                    // Ignore first directional press or A (ie. only highlight)
                    if (old == true && Element::isTouch == false && ((event->id() != FAKE_ID && (event->button() >= DPAD_LEFT && event->button() <= DPAD_DOWN)) || (event->button() == A))) {
                        delete event;
                        break;
                    }

                    if (event->id() == FAKE_ID && (event->button() < DPAD_LEFT || event->button() > DPAD_DOWN)) {
                        Element::isTouch = true;
                    }

                    if (this->overlays.size() == 0) {
                        this->screen->handleEvent(event);
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
                    this->heldTime = -defaultMarkHeldDelay;
                }
            }
        }
    }

    void Window::renderDebug(const double delta) {
        size_t fps = static_cast<size_t>(1.0/(delta/1000.0));

        // Form the string first
        std::string text = "FPS: " + std::to_string(fps) + " (" + std::to_string(static_cast<int>(delta)) + " ms)\n";
        text += "Mem: " + std::to_string(Element::renderer->memoryUsage()/1024) + " KB\n";
        text += "Surf: " + std::to_string(Element::renderer->surfaceCount()) + "\n";
        text += "Tex: " + std::to_string(Element::renderer->textureCount());

        // Render and position at bottom left
        Drawable * info = Element::renderer->renderWrappedTextSurface(text, debugFontSize, 350);
        info->convertToTexture();
        info->setColour(Colour(0, 200, 200, 200));
        info->render(5, Element::renderer->windowHeight() - info->height() - 5);
        delete info;
    }

    void Window::renderFade(const double delta) {
        switch (this->fade.action) {
            case FadeAction::None:
                break;

            case FadeAction::In:
                Element::renderer->fillWindow(Colour(0, 0, 0, this->fade.alpha));
                this->fade.alpha -= fadeDelta * (delta/1000.0);
                if (this->fade.alpha < 0) {
                    this->fade.alpha = 0;
                    this->fade.action = FadeAction::None;
                }
                break;

            case FadeAction::Out:
                Element::renderer->fillWindow(Colour(0, 0, 0, this->fade.alpha));
                this->fade.alpha += fadeDelta * (delta/1000.0);
                if (this->fade.alpha > 255) {
                    this->fade.alpha = 255;
                    this->fade.action = FadeAction::None;
                    this->shouldLoop = false;
                }
                break;
        }
    }

    void Window::updateHeldButton(const double delta) {
        if (this->heldButton != Button::NO_BUTTON) {
            this->heldTime += delta;
            if (this->heldTime >= this->holdDelay_) {
                this->heldTime -= this->holdDelay_;
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
    }

    void Window::updateOverlays(const double delta) {
        size_t i = 0;
        while (i < this->overlays.size()) {
            // Skip over overlays that shouldn't be closed
            if (!this->overlays[i]->shouldClose()) {
                this->overlays[i]->update(delta);
                i++;
                continue;
            }

            // Otherwise remove from the vector
            this->overlays.erase(this->overlays.begin() + i);
            if (this->overlays.size() == 0) {
                this->screen->setActive();
            } else {
                this->overlays[this->overlays.size() - 1]->setActive();
            }
        }
    }

    void Window::showDebugInfo(const bool show) {
        this->showDebug = show;
    }

    void Window::setFadeIn(const bool fade) {
        this->fade.in = fade;
    }

    void Window::setFadeOut(const bool fade) {
        this->fade.out = fade;
    }

    void Window::setBackgroundColour(const Colour & col) {
        this->bgColour = col;
    }

    bool Window::setBackgroundImage(const std::string & path) {
        // If string is empty simply remove
        if (path.empty()) {
            delete this->bgDrawable;
            this->bgDrawable = new Drawable();
            return true;
        }

        // Attempt to create texture before replacing
        Drawable * tmp = Element::renderer->renderImageSurface(path, Element::renderer->windowWidth(), Element::renderer->windowHeight());
        if (tmp->type() == Drawable::Type::None) {
            return false;
        }
        tmp->convertToTexture();

        // If all succeeded, actually replace the current image
        delete this->bgDrawable;
        this->bgDrawable = tmp;
        return true;
    }

    void Window::setFont(const std::string & path) {
        Element::renderer->setFont(path);
    }

    void Window::setFontSpacing(const double spacing) {
        Element::renderer->setFontSpacing(spacing);
    }

    void Window::setHighlightAnimation(const std::function<Colour(const uint32_t)> & func) {
        if (func == nullptr) {
            return;
        }

        this->highlight = func;
    }

    void Window::setHighlightBackground(const Colour & col) {
        Element::hiBGColour = col;
    }

    void Window::setHighlightOverlay(const Colour & col) {
        Element::selColour = col;
    }

    int Window::holdDelay() {
        return this->holdDelay_;
    }

    void Window::setHoldDelay(const int ms) {
        this->holdDelay_ = ms;
    }

    void Window::addOverlay(Overlay * ovl) {
        // Mark overlay as not closed
        ovl->reuse();

        // Add to vector and make it the active layer
        this->overlays.push_back(ovl);
        this->screen->setInactive();
        if (this->overlays.size() > 1) {
            this->overlays[this->overlays.size() - 2]->setInactive();
        }
    }

    void Window::pushScreen() {
        this->screenOps.push(std::pair<ScreenOperation, Screen *>(ScreenOperation::Push, nullptr));
    }

    void Window::popScreen() {
        this->screenOps.push(std::pair<ScreenOperation, Screen *>(ScreenOperation::Pop, nullptr));
    }

    void Window::removeScreen() {
        if (this->screen == nullptr) {
            return;
        }

        this->screen->onUnload();
        this->screen = nullptr;
    }

    void Window::showScreen(Screen * screen) {
        this->screenOps.push(std::pair<ScreenOperation, Screen *>(ScreenOperation::Set, screen));
    }

    bool Window::loop() {
        // Start the timer if it doesn't exist
        if (this->timer == nullptr) {
            if (this->fade.in) {
                this->fade.action = FadeAction::In;
                this->fade.alpha = 255;
            }

            this->lastMillis = 0;
            this->timer = new Timer();
            this->timer->start();
        }

        // Perform all requested operations with screens, exiting if none set
        this->performScreenOps();
        if (this->screen == nullptr) {
            return false;
        }

        // Check for events and pass to active layer
        this->processEvents();

        // Get the time since last frame and update our state
        double millis = this->timer->elapsedMillis();
        double delta = millis - this->lastMillis;
        this->lastMillis = millis;
        this->screen->update(delta);
        this->updateHeldButton(delta);
        this->updateOverlays(delta);
        Element::hiBorderColour = this->highlight(millis);

        // Clear screen and render image if needed
        Element::renderer->fillWindow(this->bgColour);
        this->bgDrawable->render(0, 0, Element::renderer->windowWidth(), Element::renderer->windowHeight());

        // Present screen and then overlays on top
        this->screen->render();
        for (Overlay * ovl : this->overlays) {
            ovl->render();
        }

        // Render debug information and fade rectangle on very top
        if (this->showDebug) {
            this->renderDebug(delta);
        }
        this->renderFade(delta);

        // Present the framebuffer and return
        Element::renderer->present();
        return this->shouldLoop;
    }

    void Window::exit() {
        if (this->fade.out && this->fade.action != FadeAction::Out) {
            this->fade.action = FadeAction::Out;
            this->fade.alpha = 0;
        } else {
            this->shouldLoop = false;
        }
    }

    Window::~Window() {
        // As we're a friend, delete the cached Element textures
        delete Element::hiBGTex;
        delete Element::hiBorderTex;
        delete Element::selTex;
        Element::hiBGTex = nullptr;
        Element::hiBorderTex = nullptr;
        Element::selTex = nullptr;

        // Stop running threads + clean up renderer
        delete ThreadPool::getInstance();
        delete this->bgDrawable;
        Element::renderer->cleanup();
        delete Element::renderer;

        // Finally delete our timer
        delete this->timer;
    }
}
