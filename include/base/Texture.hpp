#ifndef AETHER_TEXTURE_HPP
#define AETHER_TEXTURE_HPP

#include <atomic>
#include "Element.hpp"

namespace Aether {
    // A Texture is an element consisting of an SDL_Texture.
    // It stores the width and height of the texture and the
    // colour it's tinted with.
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
            // Dimensions of the texture
            int texW_, texH_;

            // Coords + dimensions of OVERALL texture to draw
            int maskX, maskY, maskW, maskH;

            /**
             * @brief Status of texture with respect to generation
             */
            std::atomic<ThreadedStatus> status;

            /**
             * @brief Queues surface generation and sets status accordingly
             */
            void createSurface();

        protected:
            /**
             * @brief \ref ::RenderType which indicates how/when to generate texture
             */
            RenderType renderType;

            // Colour to tint the texture with when drawn
            // Defaults to white
            Colour colour;

            /**
             * @brief Pointer to rendered surface that is waiting to be converted to a texture
             */
            std::atomic<SDL_Surface *> surface;

            // The actual texture
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

            // Getters for dimensions
            int texW();
            int texH();

            // Return the set colour
            Colour getColour();
            // Set the colour
            void setColour(Colour);
            void setColour(uint8_t, uint8_t, uint8_t, uint8_t);

            // Set pointed values to values of mask
            void getMask(int *, int *, int *, int *);
            // Set the "mask" of the texture to draw
            void setMask(int, int, int, int);

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

            // Render the texture
            void render();

            // Destructor destroys stored texture
            ~Texture();
    };
};

#endif