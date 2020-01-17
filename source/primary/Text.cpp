#include "Text.hpp"

// Time to pause after text scroll
#define DEFAULT_SCROLL_PAUSE 700
// Default scrolling speed (pps)
#define DEFAULT_SCROLL_SPEED 120

namespace Aether {
    Text::Text(int x, int y, std::string s, unsigned int f, FontType t) : BaseText(x, y, s, f, t) {
        this->scroll_ = false;
        this->redrawTexture();
    }

    void Text::redrawTexture() {
        this->setScroll(this->scroll_);
        this->setTexture(SDLHelper::renderText(this->string_.c_str(), this->fontSize_, (this->fontType == FontType::Extended) ? true : false));
    }

    bool Text::scroll() {
        return this->scroll_;
    }

    void Text::setScroll(bool s) {
        this->scroll_ = s;
        this->scrollPauseTime = -DEFAULT_SCROLL_PAUSE;
        this->scrollPos = 0;
        this->scrollSpeed_ = DEFAULT_SCROLL_SPEED;
    }

    int Text::scrollSpeed() {
        return this->scrollSpeed_;
    }

    void Text::setScrollSpeed(int s) {
        this->scrollSpeed_ = s;
    }

    void Text::setString(std::string s) {
        BaseText::setString(s);
        this->redrawTexture();
    }

    void Text::setFontSize(unsigned int f) {
        BaseText::setFontSize(f);
        this->redrawTexture();
    }

    void Text::update(uint32_t dt) {
        Element::update(dt);

        // Check if need to scroll and do so
        if (this->scroll()) {
            if (this->texW() > this->w()) {
                if (this->scrollPos >= (this->texW() - this->w())) {
                    this->scrollPos = this->texW() - this->w();
                    if (this->scrollPauseTime > DEFAULT_SCROLL_PAUSE) {
                        this->scrollPos = 0;
                        this->scrollPauseTime = -DEFAULT_SCROLL_PAUSE;
                    } else {
                        this->scrollPauseTime += dt;
                    }
                } else {
                    if (this->scrollPauseTime < 0) {
                        this->scrollPauseTime += dt;
                        if (this->scrollPauseTime > 0) {
                            this->scrollPauseTime = 0;
                        }
                    } else {
                        this->scrollPos += DEFAULT_SCROLL_SPEED*(dt/1000.0);
                    }
                }
            }
        }
    }

    void Text::render() {
        if (this->hidden()) {
            return;
        }
        SDLHelper::drawTexture(this->texture, this->colour, this->x(), this->y(), this->w(), this->h(), this->scrollPos, 0, this->w(), this->h());
        Element::render();
    }
};