#include "Aether/types/Drawable.hpp"
#include "Aether/Renderer.hpp"

namespace Aether {
    Drawable::Drawable() {
        this->data.surface = nullptr;
        this->colour_ = Colour(255, 255, 255, 255);
        this->type_ = Type::None;
        this->width_ = 0;
        this->height_ = 0;
        this->renderer = nullptr;
        this->setMask(0, 0, 0, 0);
    }

    Drawable::Drawable(Renderer * renderer, SDL_Surface * surf, const unsigned int width, const unsigned int height) {
        this->data.surface = surf;
        this->colour_ = Colour(255, 255, 255, 255);
        this->type_ = Type::Surface;
        this->width_ = width;
        this->height_ = height;
        this->renderer = renderer;
        this->setMask(0, 0, width, height);
    }

    Drawable::Drawable(Renderer * renderer, SDL_Texture * tex, const unsigned int width, const unsigned int height) {
        this->data.texture = tex;
        this->colour_ = Colour(255, 255, 255, 255);
        this->type_ = Type::Texture;
        this->width_ = width;
        this->height_ = height;
        this->renderer = renderer;
        this->setMask(0, 0, width, height);
    }

    void Drawable::setColour(const Colour & colour) {
        this->colour_ = colour;
    }

    void Drawable::setMask(const int x, const int y, const unsigned int width, const unsigned int height) {
        this->maskX = x;
        this->maskY = y;
        this->maskW = width;
        this->maskH = height;
    }

    void Drawable::render(const int x, const int y, const unsigned int width, const unsigned int height) {
        // Don't draw "nothing" or surfaces
        if (this->type_ != Type::Texture) {
            return;
        }

        this->renderer->drawTexture(this->data.texture, this->colour_, x, y, width == 0 ? this->width_ : width, height == 0 ? this->height_ : height, this->maskX, this->maskY, this->maskW, this->maskH);
    }

    bool Drawable::convertToTexture() {
        // Sanity check
        if (this->type_ != Type::Surface) {
            return true;
        }

        // Attempt conversion and return result
        this->data.texture = this->renderer->convertSurfaceToTexture(this->data.surface);
        if (this->data.texture == nullptr) {
            this->type_ = Type::None;
        } else {
            this->type_ = Type::Texture;
        }

        return (this->type_ == Type::Texture);
    }

    Drawable::Type Drawable::type() {
        return this->type_;
    }

    int Drawable::width() {
        return static_cast<int>(this->width_);
    }

    int Drawable::height() {
        return static_cast<int>(this->height_);
    }

    Drawable::~Drawable() {
        switch (this->type_) {
            case Type::None:
                break;

            case Type::Surface:
                this->renderer->destroySurface(this->data.surface, true);
                break;

            case Type::Texture:
                this->renderer->destroyTexture(this->data.texture, true);
                break;
        }
    }
};
