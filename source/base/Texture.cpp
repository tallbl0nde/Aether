#include "Texture.hpp"

namespace Aether {
    Texture::Texture(SDL_Texture * t) : Element() {
        this->texture = nullptr;
        this->setTexture(t);
        this->colour = Colour{255, 255, 255, 255};
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

    void Texture::setTexture(SDL_Texture * t) {
        this->destroyTexture();
        this->texture = t;
        SDLHelper::getDimensions(this->texture, &this->texW_, &this->texH_);
        this->setW(this->texW_);
        this->setH(this->texH_);
    }

    void Texture::destroyTexture() {
        if (this->texture != nullptr) {
            SDLHelper::destroyTexture(this->texture);
        }
        this->texture = nullptr;
        this->texW_ = 0;
        this->texH_ = 0;
    }

    void Texture::render() {
        if (this->hidden()) {
            return;
        }

        SDLHelper::drawTexture(this->texture, this->colour, this->x(), this->y(), this->w(), this->h());
        Element::render();
    }

    Texture::~Texture() {
        this->destroyTexture();
    }
};