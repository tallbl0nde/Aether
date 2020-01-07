#ifndef AETHER_TEXTURE_HPP
#define AETHER_TEXTURE_HPP

#include "Element.hpp"
#include "Types.hpp"

namespace Aether {
    // A Texture is an element consisting of an SDL_Texture.
    // It stores the width and height of the texture and the
    // colour it's tinted with.
    class Texture : public Element {
        private:
            // Dimensions of the texture
            int texW_;
            int texH_;

            // Delete the stored texture (only called internally)
            void destroyTexture();

        protected:
            // Colour to tint the texture with when drawn
            // Defaults to white
            Colour colour;

            // Overriden by derived classes to redraw texture when
            // variables are changed
            virtual void redrawTexture() = 0;

            // The actual texture
            SDL_Texture * texture;

        public:
            // Constructor optionally takes the texture + sets dimensions to those of the texture
            Texture(SDL_Texture * = nullptr);

            // Getters for dimensions
            int texW();
            int texH();

            // Return the set colour
            Colour getColour();
            Color getColor();
            // Set the colour
            void setColour(Colour);
            void setColour(uint8_t, uint8_t, uint8_t, uint8_t);
            void setColor(Color);
            void setColor(uint8_t, uint8_t, uint8_t, uint8_t);

            // Set the texture and recalculate dimensions (also destroys previous one)
            void setTexture(SDL_Texture *);

            // Render the texture
            void render();

            // Destructor destroys stored texture
            ~Texture();
    };
};

#endif