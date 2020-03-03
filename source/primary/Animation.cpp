#include "Animation.hpp"

namespace Aether {
    Animation::Animation(int x, int y, int w, int h) : Element(x, y, w, h) {
        this->idx = 0;
        this->frameTime = 1000;     // Default of 1 second per frame
        this->currTime = 0;
        this->paused = false;
    }

    void Animation::update(uint32_t dt) {
        // Update children
        Element::update(dt);

        // Increment time and change frame if need be
        if (!this->paused) {
            this->currTime += dt;
            while (this->currTime >= this->frameTime) {
                this->currTime -= this->frameTime;
                this->idx++;
                if (this->idx >= this->children.size()) {
                    this->idx = 0;
                }
            }
        }
    }

    void Animation::render() {
        // Do nothing if hidden or off-screen
        if (!this->isVisible()) {
            return;
        }

        SDL_BlendMode bld = SDLHelper::getBlendMode();
        SDLHelper::setBlendMode(SDL_BLENDMODE_BLEND);
        if (this->highlighted() && !this->isTouch) {
            this->renderHighlighted();
        }

        // Draw active child
        if (this->idx < this->children.size()) {
            this->children[this->idx]->render();
        }
        SDLHelper::setBlendMode(bld);

        if (this->selected()) {
            this->renderSelected();
        }
    }

    void Animation::pause() {
        this->paused = true;
    }

    void Animation::resume() {
        this->paused = false;
    }

    bool Animation::isPaused() {
        return this->paused;
    }

    void Animation::setAnimateSpeed(int t) {
        this->frameTime = t;
    }

    unsigned int Animation::animateSpeed() {
        return this->frameTime;
    }

    bool Animation::setFrameIndex(unsigned int i) {
        if (i < this->children.size()) {
            this->currTime = 0;
            this->idx = i;
            return true;
        }

        return false;
    }

    bool Animation::setFrameElement(Element * e) {
        for (size_t i = 0; i < this->children.size(); i++) {
            if (this->children[i] == e) {
                return this->setFrameIndex(i);
            }
        }

        return false;
    }
};