#include "Texture.hpp"

namespace Aether {
    Texture::Texture(Element * e, SDL_Texture * t) : Element(e) {
        this->texture = t;
        this->colour = SDL_Color{255, 255, 255, 255};
    }

    int Texture::texW() {
        return this->texW_;
    }

    int Texture::texH() {
        return this->texH_;
    }

    void Texture::getColour(uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a) {
        *r = this->colour.r;
        *g = this->colour.g;
        *b = this->colour.b;
        *a = this->colour.a;
    }

    void Texture::setColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->colour.r = r;
        this->colour.g = g;
        this->colour.b = b;
        this->colour.a = a;
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