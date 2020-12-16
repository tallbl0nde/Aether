#include "Aether/primary/TextBlock.hpp"

namespace Aether {
    TextBlock::TextBlock(int x, int y, std::string s, unsigned int f, unsigned int w, RenderType rt) : BaseText(x, y, s, f, rt) {
        this->wrapWidth_ = w;

        // Now check if we need to render immediately
        if (this->renderType == RenderType::OnCreate) {
            this->generateSurface();
            this->convertSurface();
        }
    }

    void TextBlock::generateSurface() {
        this->drawable = this->renderer->renderWrappedTextSurface(this->string_, this->fontSize_, this->wrapWidth());
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