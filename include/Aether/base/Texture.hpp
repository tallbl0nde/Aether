#ifndef AETHER_TEXTURE_HPP
#define AETHER_TEXTURE_HPP

#include <atomic>
#include "Aether/base/Element.hpp"

namespace Aether {
    /**
     * @brief A Texture is an element consisting of an SDL_Texture.
     * It stores the width and height of the texture and the
     * colour it's tinted with.
     * @note Colour defaults to white
     */
    class Texture : public Element {
        /**
         * @brief Enum class for status of texture generation
         */
        enum class ThreadedStatus {
            Empty,		/**< No texture/surface stored or rendering */
            Queued,		/**< Surface generation is queued or rendering */
            Surface,	/**< Surface has ben rendered and can be converted to a texture */
            Texture		/**< Texture has been rendered and the element can be displayed */
        };

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

            /** @brief Status of texture with respect to generation */
            std::atomic<ThreadedStatus> status;

            /** @brief Queues surface generation and sets status accordingly */
            void createSurface();

        protected:
            /** @brief \ref ::RenderType which indicates how/when to generate texture */
            RenderType renderType;

        protected:
            /** @brief Colour to tint the texture with when drawn */
            Colour colour;

            /** @brief Pointer to rendered surface that is waiting to be converted to a texture */
            std::atomic<SDL_Surface *> surface;

            /** @brief Pointer to texture to be drawn */
            std::atomic<SDL_Texture *> texture;

            /**
             * @brief Function which must generate the appropriate surface
             * @note Called in another thread!
             */
            virtual void generateSurface() = 0;

            /**
             * @brief Converts surface to texture and sets variables to match
             */
            void convertSurface();

            /**
             * @brief Regenerate the texture based on render type.
             * Calls a combination of available functions based on said type.
             */
            void regenerate();

        public:
            /**
             * @brief Construct a new Texture object
             * @note Width and height will be set when a texture is created
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param t \ref ::RenderType indicating when to generate texture
             */
            Texture(int x, int y, RenderType t);

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
             * @brief Delete the rendered texture/surface
             */
            void destroyTexture();

            /**
             * @brief Start rendering texture in the background
             * @note This function does nothing if the element was created with \ref ::RenderType::OnCreate
             *
             * @return true if queued successfully
             * @return false if already queued
             */
            bool startRendering();

            /**
             * @brief Returns whether the surface is finished generating
             * @note This function returns false if the element was created with \ref ::RenderType::OnCreate
             *
             * @return true if the surface is ready
             * @return false otherwise
             */
            bool surfaceReady();

            /**
             * @brief Returns whether a texture is ready to be drawn
             *
             * @return true if the texture is ready
             * @return false otherwise
             */
            bool textureReady();

            /**
             * @brief Extends the update method to check if there is a surface that needs converting
             */
            void update(uint32_t);

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