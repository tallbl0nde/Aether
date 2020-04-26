#include "ThreadedTextBlock.hpp"

namespace Aether::Exp {
    ThreadedTextBlock::ThreadedTextBlock(ThreadQueue * q, int x, int y, std::string s, unsigned int f, unsigned int w, FontStyle l, RenderType r) : Threaded(q, r), TextBlock(x, y, s, f, w, l, RenderType::Deferred) {
        this->redrawTexture();
    }

    void ThreadedTextBlock::generateSurface() {
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

    void ThreadedTextBlock::convertSurface() {
        this->setTexture(SDLHelper::convertSurfaceToTexture(this->surface));
        this->surface = nullptr;
    }

    void ThreadedTextBlock::removeTexture() {
        this->destroyTexture();
    }

    void ThreadedTextBlock::redrawTexture() {
        if (this->renderType != RenderType::Deferred) {
            this->startRendering();
        }
    }

    void ThreadedTextBlock::update(uint32_t dt) {
        this->updateState();
        TextBlock::update(dt);
    }
};