#include "Aether/primary/Text.hpp"

// Time to pause after text scroll
#define DEFAULT_SCROLL_PAUSE 700
// Default scrolling speed (pps)
#define DEFAULT_SCROLL_SPEED 120

namespace Aether {
    Text::Text(int x, int y, std::string s, unsigned int f, RenderType r) : BaseText(x, y, s, f, r) {
        this->scroll_ = false;
        this->scrollSpeed_ = DEFAULT_SCROLL_SPEED;
        this->scrollWaitTime_ = DEFAULT_SCROLL_PAUSE;

        // Now check if we need to render immediately
        if (this->renderType == RenderType::OnCreate) {
            this->generateSurface();
            this->convertSurface();
        }

        this->setScroll(this->scroll_);
    }

    void Text::generateSurface() {
        this->drawable = this->renderer->renderTextSurface(this->string_, this->fontSize_);
    }

    bool Text::scroll() {
        return this->scroll_;
    }

    void Text::setScroll(bool s) {
        this->scroll_ = s;
        this->scrollPauseTime = -this->scrollWaitTime_;
        this->scrollPosition = 0;
        this->setMask(0, 0, this->w(), this->texH());
    }

    int Text::scrollWaitTime() {
        return this->scrollWaitTime_;
    }

    void Text::setScrollWaitTime(unsigned int t) {
        this->scrollWaitTime_ = t;
    }

    int Text::scrollSpeed() {
        return this->scrollSpeed_;
    }

    void Text::setScrollSpeed(int s) {
        this->scrollSpeed_ = s;
    }

    void Text::setFontSize(unsigned int s) {
        BaseText::setFontSize(s);
        this->setScroll(this->scroll_);
    }

    void Text::setString(std::string s) {
        BaseText::setString(s);
        this->setScroll(this->scroll_);
    }

    void Text::update(uint32_t dt) {
        BaseText::update(dt);

        // Check if need to scroll and do so
        if (this->scroll()) {
            if (this->texW() > this->w()) {
                if (this->scrollPosition >= (this->texW() - this->w())) {
                    // If we're past the end and we've waited, go back to the start
                    if (this->scrollPauseTime > this->scrollWaitTime_) {
                        this->setMask(0, 0, this->w(), this->texH());
                        this->scrollPauseTime = -this->scrollWaitTime_;
                        this->scrollPosition = 0;

                    // Otherwise keep waiting
                    } else {
                        this->scrollPauseTime += dt;
                    }

                } else {
                    // Don't scroll until we've waited long enough
                    if (this->scrollPauseTime < 0) {
                        this->scrollPauseTime += dt;

                    // Otherwise we can scroll :)
                    } else {
                        float tmp = this->scrollSpeed_*(dt/1000.0);
                        this->scrollPosition += (tmp < 1 ? tmp : (int)tmp);     // Little workaround to allow text to scroll smoothly (not accurate sadly :/)
                        this->setMask((int)this->scrollPosition, 0, this->w(), this->texH());
                    }
                }
            }
        }
    }

    void Text::setW(int w) {
        BaseText::setW(w);
        this->setScroll(this->scroll_);
    }
};