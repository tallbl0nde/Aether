#include "Aether/primary/Text.hpp"
#include "Aether/types/Timer.hpp"

// Default time to pause once we reach the end
static constexpr unsigned int defaultWaitInterval = 700;

// Default scrolling speed
static constexpr int defaultScrollSpeed = 120;

namespace Aether {
    Text::Text(const int x, const int y, const std::string & str, const unsigned int size, const Render type) : BaseText(x, y, str, size) {
        // Initialize default scrolling parameters
        this->scroll.allowed = false;
        this->scroll.position = 0;
        this->scroll.speed = defaultScrollSpeed;
        this->scroll.timer = new Timer();
        this->scroll.waitInterval = defaultWaitInterval;

        // Render based on requested type
        if (type == Render::Sync) {
            this->renderSync();

        } else if (type == Render::Async) {
            this->renderAsync();
        }
    }

    std::pair<int, int> Text::getDimensions(const std::string & str, const unsigned int size) {
        return Text::renderer->calculateTextDimensions(str, size);
    }

    Drawable * Text::renderDrawable() {
        if (this->string_.empty()) {
            return new Drawable();
        } else {
            return this->renderer->renderTextSurface(this->string_, this->fontSize_);
        }
    }

    bool Text::canScroll() {
        return this->scroll.allowed;
    }

    void Text::setCanScroll(const bool scroll) {
        this->scroll.allowed = scroll;
        this->scroll.position = 0;
        this->scroll.timer->stop();
        this->setMask(0, 0, this->w(), this->textureHeight());
    }

    void Text::setScrollPause(const unsigned int ms) {
        this->scroll.waitInterval = ms;
    }

    void Text::setScrollSpeed(const int pps) {
        this->scroll.speed = pps;
    }

    void Text::setString(const std::string & str) {
        BaseText::setString(str);
        this->setCanScroll(this->scroll.allowed);
    }

    void Text::setFontSize(const unsigned int size) {
        BaseText::setFontSize(size);
        this->setCanScroll(this->scroll.allowed);
    }

    void Text::update(unsigned int dt) {
        BaseText::update(dt);

        // Check if we need to scroll and do so
        if (this->scroll.allowed && this->textureWidth() > this->w()) {
            // If we're past the end
            if (this->scroll.position >= (this->textureWidth() - this->w())) {
                // Go back to the start once we've paused
                if (this->scroll.timer->elapsedMillis() >= this->scroll.waitInterval) {
                    this->scroll.position = 0;
                    this->scroll.timer->stop();
                    this->scroll.timer->start();
                    this->setMask(0, 0, this->w(), this->textureHeight());
                }

            // Otherwise if we're not at the end don't scroll until we've waited long enough
            } else if (this->scroll.timer->elapsedMillis() >= this->scroll.waitInterval) {
                float tmp = this->scroll.speed * (dt/1000.0);
                this->scroll.position += (tmp < 1 ? tmp : static_cast<int>(tmp));
                this->setMask(static_cast<int>(this->scroll.position), 0, this->w(), this->textureHeight());

                // Reset timer once we reach the end
                if (this->scroll.position >= (this->textureWidth() - this->w())) {
                    this->scroll.position = (this->textureWidth() - this->w());
                    this->scroll.timer->stop();
                    this->scroll.timer->start();
                }
            }
        }
    }

    void Text::setW(int w) {
        BaseText::setW(w);
        this->setCanScroll(this->scroll.allowed);
    }

    Text::~Text() {
        delete this->scroll.timer;
    }
};