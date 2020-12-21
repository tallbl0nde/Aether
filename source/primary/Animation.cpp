#include "Aether/primary/Animation.hpp"

namespace Aether {
    Animation::Animation(int x, int y, int w, int h) : Element(x, y, w, h) {
        this->idx = 0;
        this->frameTime = 1000;     // Default of 1 second per frame
        this->currTime = 0;
        this->paused = false;
    }

    void Animation::update(unsigned int dt) {
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

        // Render highlight background if highlighted
        if (this->highlighted() && !this->isTouch) {
            this->renderHighlightTextures();
            this->hiBGTex->setColour(this->hiBGColour);
            this->hiBGTex->render(this->x() + (this->w() - this->hiBGTex->width())/2, this->y() + (this->h() - this->hiBGTex->height())/2);
        }

        // Render active child
        if (this->idx < this->children.size()) {
            this->children[this->idx]->render();
        }

        // Render selected/held layer
        if (this->selected()) {
            this->renderSelectionTexture();
            this->selTex->setColour(this->selColour);
            this->selTex->render(this->x() + (this->w() - this->selTex->width())/2, this->y() + (this->h() - this->selTex->height())/2);
        }

        // Finally render highlight border if needed
        if (this->highlighted() && !this->isTouch) {
            this->hiBorderTex->setColour(this->hiBorderColour);
            this->hiBorderTex->render(this->x() + (this->w() - this->hiBorderTex->width())/2, this->y() + (this->h() - this->hiBorderTex->height())/2);
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