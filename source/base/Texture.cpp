#include "Aether/base/Texture.hpp"
#include "Aether/ThreadPool.hpp"
#include <limits>

namespace Aether {
    Texture::Texture(int x, int y, RenderType t) : Element(x, y) {
        // Initialize everything
        this->colour = Colour(255, 255, 255, 255);
        this->drawable = new Drawable();
        this->renderType = t;
        this->status = ThreadedStatus::Empty;
        this->taskID = 0;       // This may overlap but it shouldn't matter
    }

    void Texture::createSurface() {
        this->status = ThreadedStatus::Queued;
        this->taskID = ThreadPool::addTask([this]() {
            this->generateSurface();
            this->status = ThreadedStatus::Surface;
        });
    }

    void Texture::convertSurface() {
        this->drawable.load()->convertToTexture();
        this->setW(this->drawable.load()->width());
        this->setH(this->drawable.load()->height());
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
        return this->drawable.load()->width();
    }

    int Texture::texH() {
        return this->drawable.load()->height();
    }

    Colour Texture::getColour() {
        return this->colour;
    }

    void Texture::setColour(Colour c) {
        this->colour = c;
        this->drawable.load()->setColour(c);
    }

    void Texture::setColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->setColour(Colour(r, g, b, a));
    }

    void Texture::setMask(int dx, int dy, int dw, int dh) {
        this->drawable.load()->setMask(dx, dy, dw, dh);
    }

    void Texture::destroyTexture() {
        delete this->drawable;
        this->drawable = new Drawable();
        this->status = ThreadedStatus::Empty;
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

        this->drawable.load()->setColour(this->colour);
        this->drawable.load()->render(this->x(), this->y(), this->w(), this->h());
        Element::render();
    }

    Texture::~Texture() {
        if (this->status == ThreadedStatus::Queued) {
            ThreadPool::removeTaskWithID(this->taskID);
        }
        delete this->drawable;
    }
};