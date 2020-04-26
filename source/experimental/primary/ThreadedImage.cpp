#include "ThreadedImage.hpp"

namespace Aether::Exp {
    ThreadedImage::ThreadedImage(ThreadQueue * q, int x, int y, std::string p, int f1, int f2, RenderType rt) : Threaded(q, rt), Image(x, y, p, f1, f2, RenderType::Deferred) {
        this->type = Path;
        this->path = p;
        this->f1 = f1;
        this->f2 = f2;
        if (rt != RenderType::Deferred) {
            this->startRendering();
        }
    }

    ThreadedImage::ThreadedImage(ThreadQueue * q, int x, int y, u8 * ptr, size_t size, int f1, int f2, RenderType rt) : Threaded(q, rt), Image(x, y, ptr, size, f1, f2, RenderType::Deferred) {
        this->type = Pointer;
        this->ptr = ptr;
        this->size = size;
        this->f1 = f1;
        this->f2 = f2;
        if (rt != RenderType::Deferred) {
            this->startRendering();
        }
    }

    void ThreadedImage::generateSurface() {
        switch (this->type) {
            case Path:
                this->surface = SDLHelper::renderImageS(this->path, this->f1, this->f2);
                break;

            case Pointer:
                this->surface = SDLHelper::renderImageS(this->ptr, this->size, this->f1, this->f2);
                break;
        }
    }

    void ThreadedImage::convertSurface() {
        this->setTexture(SDLHelper::convertSurfaceToTexture(this->surface));
        this->surface = nullptr;
    }

    void ThreadedImage::removeTexture() {
        this->destroyTexture();
    }

    void ThreadedImage::update(uint32_t dt) {
        this->updateState();
        Image::update(dt);
    }
};