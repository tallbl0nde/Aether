#include "Aether/base/AsyncItem.hpp"
#include <algorithm>
#include <iostream>

namespace Aether {
    AsyncItem::AsyncItem() : Element() {
        this->fadeAmount = 0;
        this->renderStatus = Status::NotRendered;
    }

    bool AsyncItem::canRender() {
        return true;
    }

    int AsyncItem::fadeSpeed() {
        return 150;
    }

    int AsyncItem::renderThreshold() {
        return 1000;
    }

    bool AsyncItem::withinThreshold() {
        if (this->parent() == nullptr) {
            return false;
        }

        int threshold = this->renderThreshold();
        return (this->x() + this->w() > this->parent()->x() - threshold && this->x() < this->parent()->x() + this->parent()->w() + threshold &&
                this->y() + this->h() > this->parent()->y() - threshold && this->y() < this->parent()->y() + this->parent()->h() + threshold);
    }

    void AsyncItem::addTexture(Texture * texture) {
        this->textures.push_back(texture);

        // Ensure texture matches current state
        texture->setHidden(true);
        if (this->renderStatus == Status::NotRendered) {
            texture->destroy();
        } else {
            texture->renderSync();

            if (this->renderStatus == Status::Rendered) {
                texture->setHidden(false);
                this->positionElements();
            }
        }
    }

    bool AsyncItem::removeTexture(Texture * texture) {
        std::vector<Texture *>::iterator it = std::find(this->textures.begin(), this->textures.end(), texture);
        if (it != this->textures.end()) {
            this->textures.erase(it);
            return true;
        }
        return false;
    }

    bool AsyncItem::ready() {
        return (this->renderStatus == Status::Rendered);
    }

    void AsyncItem::update(uint32_t dt) {
        // Instead of calling Element::update() we explcitly update
        // the child elements here as Element's version will do nothing
        // if not visible (which is very likely)
        for (Element * e : this->children) {
            e->update(dt);
        }

        // Update textures based on status
        int threshold = this->renderThreshold();
        switch (this->renderStatus) {
            // Wait until we're within the threshold to begin rendering
            case Status::NotRendered:
                if (this->withinThreshold() && this->canRender()) {
                    for (Texture * texture : this->textures) {
                        texture->renderAsync();
                    }
                    this->renderStatus = Status::Rendering;
                }
                break;

            // Check if all are ready
            case Status::Rendering: {
                bool done = true;
                for (Texture * texture : this->textures) {
                    if (!texture->ready()) {
                        done = false;
                        break;
                    }
                }

                // Invoke positionElements() once all are done and fade in
                if (done) {
                    Aether::Colour col;
                    for (Texture * texture : this->textures) {
                        col = texture->colour();
                        col.setA(0);
                        texture->setColour(col);
                        texture->setHidden(false);
                    }

                    this->fadeAmount = 0;
                    this->positionElements();
                    this->renderStatus = Status::Rendered;
                }
                break;
            }

            // Destroy textures to save memory when outside of threshold, but also fade in
            // if required
            case Status::Rendered: {
                if (!this->withinThreshold()) {
                    for (Texture * texture : this->textures) {
                        texture->destroy();
                        texture->setHidden(true);
                    }
                    this->renderStatus = Status::NotRendered;
                }

                int fadeDuration = this->fadeSpeed();
                if (this->renderStatus == Status::Rendered && this->fadeAmount < fadeDuration) {
                    // Update 'global' alpha value
                    this->fadeAmount += dt;
                    if (this->fadeAmount > fadeDuration) {
                        this->fadeAmount = fadeDuration;
                    }

                    // Update each texture's alpha
                    float alpha = (this->fadeAmount/fadeDuration) * 255;
                    Aether::Colour col;
                    for (Texture * texture : this->textures) {
                        col = texture->colour();
                        col.setA(alpha);
                        texture->setColour(col);
                    }
                }
                break;
            }
        }
    }

    void AsyncItem::setW(int w) {
        Element::setW(w);
        if (this->renderStatus == Status::Rendered) {
            this->positionElements();
        }
    }

    void AsyncItem::setH(int h) {
        Element::setH(h);
        if (this->renderStatus == Status::Rendered) {
            this->positionElements();
        }
    }
}
