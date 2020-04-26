#include "ThreadedText.hpp"

namespace Aether::Exp {
    ThreadedText::ThreadedText(ThreadQueue * q, int x, int y, std::string s, unsigned int f, FontStyle l, RenderType r) : Threaded(q, r), Text(x, y, s, f, l, RenderType::Deferred) {
        this->redrawTexture();
    }

    void ThreadedText::generateSurface() {
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

    void ThreadedText::convertSurface() {
        this->setTexture(SDLHelper::convertSurfaceToTexture(this->surface));
        this->surface = nullptr;
    }

    void ThreadedText::removeTexture() {
        this->destroyTexture();
    }

    void ThreadedText::redrawTexture() {
        if (this->renderType != RenderType::Deferred) {
            this->startRendering();
        }
    }

    void ThreadedText::update(uint32_t dt) {
        this->updateState();
        Text::update(dt);
    }
};