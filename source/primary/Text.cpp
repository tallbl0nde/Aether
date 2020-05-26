#include "Text.hpp"

// Time to pause after text scroll
#define DEFAULT_SCROLL_PAUSE 700
// Default scrolling speed (pps)
#define DEFAULT_SCROLL_SPEED 120

namespace Aether {
    Text::Text(int x, int y, std::string s, unsigned int f, FontStyle l, RenderType r) : BaseText(x, y, s, f, l, r) {
        this->scroll_ = false;
        this->scrollSpeed_ = DEFAULT_SCROLL_SPEED;

        // Now check if we need to render immediately
        if (this->renderType == RenderType::OnCreate) {
            this->generateSurface();
            this->convertSurface();
        }

        this->setScroll(this->scroll_);
    }

    void Text::generateSurface() {
        int style;
        switch (this->fontStyle) {
            case FontStyle::Regular:
                style = TTF_STYLE_NORMAL;
                break;

            case FontStyle::Bold:
                style = TTF_STYLE_BOLD;
                break;

            case FontStyle::Italic:
                style = TTF_STYLE_ITALIC;
                break;

            case FontStyle::Underline:
                style = TTF_STYLE_UNDERLINE;
                break;

            case FontStyle::Strikethrough:
                style = TTF_STYLE_STRIKETHROUGH;
                break;
        }
        this->surface = SDLHelper::renderTextS(this->string_, this->fontSize_, style);
    }

    bool Text::scroll() {
        return this->scroll_;
    }

    void Text::setScroll(bool s) {
        this->scroll_ = s;
        this->scrollPauseTime = -DEFAULT_SCROLL_PAUSE;
        this->setMask(0, 0, this->w(), this->texH());
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
                int tmp, xPos;
                this->getMask(&xPos, &tmp, &tmp, &tmp);
                if (xPos >= (this->texW() - this->w())) {
                    xPos = this->texW() - this->w();
                    if (this->scrollPauseTime > DEFAULT_SCROLL_PAUSE) {
                        this->setMask(0, 0, this->w(), this->texH());
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
                        this->setMask(xPos + this->scrollSpeed_*(dt/1000.0), 0, this->w(), this->texH());
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