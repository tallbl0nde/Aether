#include "TextBlock.hpp"

namespace Aether {
    TextBlock::TextBlock(int x, int y, std::string s, unsigned int f, unsigned int w, FontStyle l, RenderType rt) : BaseText(x, y, s, f, l, rt) {
        this->wrapWidth_ = w;

        // Now check if we need to render immediately
        if (this->renderType == RenderType::OnCreate) {
            this->generateSurface();
            this->convertSurface();
        }
    }

    void TextBlock::generateSurface() {
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
        this->surface = SDLHelper::renderTextWrappedS(this->string_, this->fontSize_, this->wrapWidth(), style);
    }

    unsigned int TextBlock::wrapWidth() {
        return this->wrapWidth_;
    }

    void TextBlock::setWrapWidth(unsigned int w) {
        if (w == this->wrapWidth_) {
            return;
        }
        this->wrapWidth_ = w;

        if (this->renderType == RenderType::OnCreate) {
            this->regenerate();
        }
    }
};