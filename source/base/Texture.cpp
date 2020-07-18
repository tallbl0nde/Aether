#include "Aether/base/Texture.hpp"
#include "Aether/ThreadPool.hpp"
#include <limits>

namespace Aether {
    Texture::Texture(int x, int y, RenderType t) : Element(x, y) {
        // Initialize everything
        this->colour = Colour{255, 255, 255, 255};
        this->renderType = t;
        this->status = ThreadedStatus::Empty;
        this->taskID = 0;       // This may overlap but it shouldn't matter
        this->surface = nullptr;
        this->texture = nullptr;
        this->texH_ = 0;
        this->texW_ = 0;
        this->setMask(0, 0, 0, 0);
    }

    void Texture::createSurface() {
        this->status = ThreadedStatus::Queued;
        this->taskID = ThreadPool::addTask([this]() {
            this->generateSurface();
            this->status = ThreadedStatus::Surface;
        });
    }

    void Texture::convertSurface() {
        if (this->surface != nullptr) {
            this->texture = SDLHelper::convertSurfaceToTexture(this->surface);
            this->surface = nullptr;
        }

        if (this->texture != nullptr) {
            SDLHelper::getDimensions(this->texture, &this->texW_, &this->texH_);
            this->setW(this->texW_);
            this->setH(this->texH_);
            this->setMask(0, 0, this->texW_, this->texH_);
        }
        this->status = ThreadedStatus::Texture;
    }

    void Texture::regenerate() {
        // Regenerate immediately if needed
        if (this->renderType == RenderType::OnCreate) {
            this->destroyTexture();
            this->generateSurface();
            this->convertSurface();

        // Otherwise queue generation
        } else {
            this->startRendering();
        }
    }

    int Texture::texW() {
        return this->texW_;
    }

    int Texture::texH() {
        return this->texH_;
    }

    Colour Texture::getColour() {
        return this->colour;
    }

    void Texture::setColour(Colour c) {
        this->colour = c;
    }

    void Texture::setColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->setColour(Colour{r, g, b, a});
    }

    void Texture::getMask(int * dx, int * dy, int * dw, int * dh) {
        *dx = this->maskX;
        *dy = this->maskY;
        *dw = this->maskW;
        *dh = this->maskH;
    }

    void Texture::setMask(int dx, int dy, int dw, int dh) {
        this->maskX = dx;
        this->maskY = dy;
        this->maskW = dw;
        this->maskH = dh;
    }

    void Texture::destroyTexture() {
        ThreadedStatus st = this->status;

        // Delete texture if present
        if (st == ThreadedStatus::Texture) {
            if (this->texture != nullptr) {
                SDLHelper::destroyTexture(this->texture);
            }
            this->texture = nullptr;
            this->texW_ = 0;
            this->texH_ = 0;
            this->status = ThreadedStatus::Empty;

        // Otherwise delete surface if it hasn't been converted yet
        } else if (st == ThreadedStatus::Surface) {
            if (this->surface != nullptr) {
                SDLHelper::freeSurface(this->surface);
                this->surface = nullptr;
                this->texW_ = 0;
                this->texH_ = 0;
            }
            this->status = ThreadedStatus::Empty;
        }
    }

    bool Texture::startRendering() {
        ThreadedStatus st = this->status;
        if (this->renderType == RenderType::Deferred && (st == ThreadedStatus::Empty || st == ThreadedStatus::Texture)) {
            this->createSurface();
            return true;
        }
        return false;
    }

    bool Texture::surfaceReady() {
        return (this->status == ThreadedStatus::Surface);
    }

    bool Texture::textureReady() {
        return (this->status == ThreadedStatus::Texture);
    }

    void Texture::update(uint32_t dt) {
        // Convert surface to texture when it is ready
        if (this->status == ThreadedStatus::Surface) {
            this->convertSurface();
        }

        Element::update(dt);
    }

    void Texture::render() {
        if (this->hidden()) {
            return;
        }

        SDLHelper::drawTexture(this->texture, this->colour, this->x(), this->y(), this->w(), this->h(), this->maskX, this->maskY, this->maskW, this->maskH);
        Element::render();
    }

    Texture::~Texture() {
        if (this->status == ThreadedStatus::Queued) {
            ThreadPool::removeTaskWithID(this->taskID);
        }
        this->destroyTexture();
    }
};