#ifndef AETHER_TEXTURE_HPP
#define AETHER_TEXTURE_HPP

#include "Element.hpp"

namespace Aether {
    /**
     * @brief A Texture is an element consisting of an SDL_Texture.
     * It stores the width and height of the texture and the
     * colour it's tinted with.
     * @note Colour defaults to white
     */
    class Texture : public Element {
        private:
            /** @brief Width of texture */
            int texW_;
            /** @brief Height of texture */
            int texH_;
            /** @brief x-coordinate of mask offset start position */
            int maskX;
            /** @brief y-coordinate of mask offset start position */
            int maskY;
            /** @brief width of mask */
            int maskW;
            /** @brief height of mask */
            int maskH;

            /**
             * @brief Delete the stored texture (only called internally)
             */
            void destroyTexture();

        protected:
            /** @brief Colour to tint the texture with when drawn */
            Colour colour;
            /** @brief The actual texture */
            SDL_Texture * texture;

            /**
             * @brief Overriden by derived classes to redraw texture when
             * variables are changed.
             */
            virtual void redrawTexture() = 0;

        public:
            /**
             * @brief Construct a new Texture object
             * 
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param t texture pointer
             */
            Texture(int x, int y, SDL_Texture * t = nullptr);

            /**
             * @brief Get texture width
             * 
             * @return texture width
             */
            int texW();

            /**
             * @brief Get texture height
             * 
             * @return texture height
             */
            int texH();

            /**
             * @brief Get the colour of texture
             * 
             * @return colour of texture
             */
            Colour getColour();

            /**
             * @brief Set the colour of texture
             * 
             * @param c new colour of texture
             */
            void setColour(Colour c);

            /**
             * @brief Set the colour of texture
             * 
             * @param r Red value of colour
             * @param g Green value of colour
             * @param b Blue value of colour
             * @param a Alpha value of colour
             */
            void setColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

            /**
             * @brief Set pointed values to values of mask
             * 
             * @param dx pointer to mask offset x-coordinate
             * @param dy pointer to mask offset y-coordinate
             * @param dw pointer to mask width
             * @param dh pointer to mask height
             */
            void getMask(int * dx, int * dy, int * dw, int * dh);

            /**
             * @brief Set the mask values for the texture
             * 
             * @param dx mask offset x-coordinate
             * @param dy mask offset y-coordinate
             * @param dw mask width
             * @param dh mask height
             */
            void setMask(int dx, int dy, int dw, int dh);

            /**
             * @brief Set the texture and recalculate dimensions (also destroys previous one)
             * 
             * @param t new texture to set
             */
            void setTexture(SDL_Texture * t);

            /**
             * @brief Render the texture
             */
            void render();

            /**
             * @brief Destroy the Texture object.
             * Also, destroys stored texture.
             */
            ~Texture();
    };
};

#endif