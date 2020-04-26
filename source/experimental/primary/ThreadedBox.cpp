#include "ThreadedBox.hpp"

namespace Aether::Exp {
    ThreadedBox::ThreadedBox(ThreadQueue * q, int x, int y, int w, int h, unsigned int b, unsigned int r, RenderType rt) : Threaded(q, rt), Box(x, y, w, h, b, r, RenderType::Deferred) {
        this->redrawTexture();
    }

    void ThreadedBox::generateSurface() {
        this->surface = SDLHelper::renderRectS(this->w(), this->h(), this->border());
    }

    void ThreadedBox::convertSurface() {
        this->setTexture(SDLHelper::convertSurfaceToTexture(this->surface));
        this->surface = nullptr;
    }

    void ThreadedBox::removeTexture() {
        this->destroyTexture();
    }

    void ThreadedBox::redrawTexture() {
        if (this->renderType != RenderType::Deferred) {
            this->startRendering();
        }
    }

    void ThreadedBox::update(uint32_t dt) {
        this->updateState();
        Box::update(dt);
    }
};