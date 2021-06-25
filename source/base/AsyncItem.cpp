#include "Aether/base/AsyncItem.hpp"
#include <algorithm>

namespace Aether {
    AsyncItem::AsyncItem() : Element() {
        this->renderStatus = Status::NotRendered;
    }

    int AsyncItem::fadeSpeed() {
        return 500;
    }

    int AsyncItem::renderThreshold() {
        return 1000;
    }

    void AsyncItem::addTexture(Texture * texture) {
        this->textures.push_back(texture);

        // Ensure texture matches current state
        texture->setHidden(true);
        if (this->renderStatus == Status::NotRendered) {
            texture->destroy();
        } else {
            texture->renderAsync();
        }
    }

    bool AsyncItem::removeTexture(Texture * texture) {
        std::vector<Texture *>::iterator it = std::find(this->textures.begin(), this->textures.end(), texture);
        if (it != this->textures.end()) {
            delete (*it);
            this->textures.erase(it);
            return true;
        }
        return false;
    }

    void AsyncItem::update(uint32_t dt) {
        // Update children first
        Element::update(dt);

        // Update textures based on status
        int threshold = this->renderThreshold();
        switch (this->renderStatus) {
            // Wait until we're within the threshold to begin rendering
            case Status::NotRendered:
                if (this->y() + this->h() > -threshold && this->y() < AsyncItem::renderer->windowHeight() + threshold) {
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

                    this->alpha = 0;
                    this->positionElements();
                }
                break;
            }

            // Destroy textures to save memory when outside of threshold, but also fade in
            // if required
            case Status::Rendered: {
                bool destroyed = false;
                if (this->y() + this->h() < -threshold && this->y() > AsyncItem::renderer->windowHeight() + threshold) {
                    destroyed = true;
                    for (Texture * texture : this->textures) {
                        texture->destroy();
                        texture->setHidden(true);
                    }
                }

                if (!destroyed && this->alpha < 255) {
                    // Update 'global' alpha value
                    this->alpha += this->fadeSpeed() * (dt/1000.0);
                    if (this->alpha > 255) {
                        this->alpha = 255;
                    }

                    // Update each texture's alpha
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
