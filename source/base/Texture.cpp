#include "Aether/base/Texture.hpp"
#include "Aether/base/Texture.RenderJob.hpp"
#include "Aether/ThreadPool.hpp"

namespace Aether {
    Texture::Texture(const int x, const int y) : Element(x, y, 0, 0) {
        this->asyncID = 0;
        this->callback = nullptr;
        this->status = AsyncStatus::Waiting;

        this->colour_ = Colour(255, 255, 255, 255);
        this->drawable = new Drawable();
        this->tmpDrawable = nullptr;
    }

    void Texture::setupDrawable() {
        this->drawable->convertToTexture();
        this->drawable->setColour(this->colour_);

        this->setW(this->drawable->width());
        this->setH(this->drawable->height());

        if (this->callback != nullptr) {
            this->callback();
        }
    }

    void Texture::setRenderCallback(const std::function<void()> func) {
        this->callback = func;
    }

    Colour Texture::colour() {
        return this->colour_;
    }

    void Texture::setColour(const Colour & col) {
        this->colour_ = col;
        this->drawable->setColour(this->colour_);
    }

    int Texture::textureWidth() {
        return this->drawable->width();
    }

    int Texture::textureHeight() {
        return this->drawable->height();
    }

    void Texture::setMask(const int x, const int y, const unsigned int w, const unsigned int h) {
        this->drawable->setMask(x, y, w, h);
    }

    void Texture::destroy() {
        if (this->status == AsyncStatus::Rendering) {
            ThreadPool::getInstance()->removeOrWaitForJob(this->asyncID);
            this->asyncID = 0;
        }

        delete this->drawable;
        this->drawable = new Drawable();
        this->status = AsyncStatus::Waiting;
    }

    bool Texture::ready() {
        return (this->drawable->type() == Drawable::Type::Texture);
    }

    void Texture::renderSync() {
        if (this->status != AsyncStatus::Waiting) {
            return;
        }

        delete this->drawable;
        this->drawable = this->renderDrawable();
        this->setupDrawable();
        this->status = AsyncStatus::Done;
    }

    void Texture::renderAsync() {
        if (this->status != AsyncStatus::Waiting) {
            return;
        }

        this->status = AsyncStatus::Rendering;
        this->asyncID = ThreadPool::getInstance()->queueJob(new RenderJob(this), ThreadPool::Importance::Normal);
    }

    void Texture::update(unsigned int dt) {
        if (this->status == AsyncStatus::NeedsConvert) {
            delete this->drawable;
            this->drawable = this->tmpDrawable;
            this->tmpDrawable = nullptr;

            this->setupDrawable();
            this->status = AsyncStatus::Done;
        }
        Element::update(dt);
    }

    void Texture::render() {
        if (this->hidden()) {
            return;
        }

        this->drawable->render(this->x(), this->y(), this->w(), this->h());
        Element::render();
    }

    Texture::~Texture() {
        if (this->status == AsyncStatus::Rendering) {
            ThreadPool::getInstance()->removeOrWaitForJob(this->asyncID);
        }
        delete this->drawable;
        delete this->tmpDrawable;
    }
};