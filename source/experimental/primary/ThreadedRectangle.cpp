#include "ThreadedRectangle.hpp"

namespace Aether::Exp {
    ThreadedRectangle::ThreadedRectangle(ThreadQueue * q, int x, int y, int w, int h, unsigned int r, RenderType rt) : Threaded(q, rt), Rectangle(x, y, w, h, r, RenderType::Deferred) {
        this->redrawTexture();
    }

    void ThreadedRectangle::generateSurface() {
        this->surface = SDLHelper::renderFilledRectS(this->w(), this->h());
    }

    void ThreadedRectangle::convertSurface() {
        this->setTexture(SDLHelper::convertSurfaceToTexture(this->surface));
        this->surface = nullptr;
    }

    void ThreadedRectangle::redrawTexture() {
        if (this->renderType != RenderType::Deferred) {
            this->startRendering();
        }
    }

    void ThreadedRectangle::update(uint32_t dt) {
        this->updateState();
        Rectangle::update(dt);
    }
};