#ifndef AETHER_DRAWABLE_HPP
#define AETHER_DRAWABLE_HPP

#include "Aether/types/Colour.hpp"
#include "Aether/Renderer.hpp"

// Forward declare types as we only need a pointer here
namespace Aether {
    class Renderer;
};
struct SDL_Surface;
struct SDL_Texture;

namespace Aether {
    /**
     * @brief Stores either a surface or texture which can be drawn
     * on screen by providing a renderer to \ref render().
     * @note This class is not thread-safe by itself.
     * @note All instances of this should be deleted before deleting the main renderer.
     */
    class Drawable {
        public:
            /**
            * @brief Type of raw data stored
            */
            enum class Type {
                None,                       /**< Doesn't store any data that can be rendered */
                Surface,                    /**< Currently stores a SDL 'surface' */
                Texture                     /**< Currently stores a SDL 'texture' */
            };

        private:
            /**
             * @brief Union of types that can be stored
             */
            union Data {
                SDL_Surface * surface;      /**< Pointer to SDL 'surface' */
                SDL_Texture * texture;      /**< Pointer to SDL 'texture' */
            } data;

            Colour colour_;                 /** @brief Colour to tint with */
            Renderer * renderer;            /** @brief Renderer object */
            Type type_;                     /** @brief Type of stored data */

            unsigned int width_;            /** @brief Width of image (in pixels) */
            unsigned int height_;           /** @brief Height of image (in pixels) */

            int maskX;                      /** @brief X coordinate of mask (top-left) */
            int maskY;                      /** @brief Y coordinate of mask (top-left) */
            int maskW;                      /** @brief Width of mask */
            int maskH;                      /** @brief Height of mask */

        public:
            /**
             * @brief Default constructor. Initializes with no data.
             * @note This is effectively useless: it's here for default allocations.
             */
            Drawable();

            /**
             * @brief Create a Drawable from a surface
             *
             * @param renderer Renderer to draw/manipulate Drawable with
             * @param surf Surface to instantiate with
             * @param width Width of given surface (in pixels)
             * @param height Height of given surface (in pixels)
             */
            Drawable(Renderer * renderer, SDL_Surface * surf, const unsigned int width, const unsigned int height);

            /**
             * @brief Create a Drawable from a texture
             *
             * @param renderer Renderer to draw/manipulate Drawable with
             * @param tex Texture to instantiate with
             * @param width Width of given texture (in pixels)
             * @param height Height of given texture (in pixels)
             */
            Drawable(Renderer * renderer, SDL_Texture * tex, const unsigned int width, const unsigned int height);

            /**
             * @brief Set colour to tint Drawable with when rendered
             *
             * @param colour Colour to tint with
             */
            void setColour(const Colour & colour);

            /**
             * @brief Set the mask (area to draw)
             *
             * @param x X coordinate of mask (top-left)
             * @param y Y coordinate of mask (top-left)
             * @param width Width of mask
             * @param height Height of mask
             */
            void setMask(const int x, const int y, const unsigned int width, const unsigned int height);

            /**
             * @brief Render the Drawable on screen at the given coordinates
             *
             * @param x X coordinate (top-left)
             * @param y Y coordinate (top-left)
             * @param width Width to render at (optional, defaults to Drawable size)
             * @param height Height to render at (optional, defaults to Drawable size)
             */
            void render(const int x, const int y, const unsigned int width = 0, const unsigned int height = 0);

            /**
             * @brief Convert the contained surface to a texture. Has no
             * effect if the Drawable does not contain a surface.
             * @note This should be called by the same thread that instantiated the renderer!
             *
             * @return true if successful/not a surface, false if conversion failed
             */
            bool convertToTexture();

            /**
             * @brief Returns the type of the data stored
             *
             * @return Type of raw data stored (see \ref Type)
             */
            Type type();

            /**
             * @brief Returns the width of the Drawable in pixels
             *
             * @return Width of Drawable in pixels, 0 if no data stored
             */
            int width();

            /**
             * @brief Returns the height of the Drawable in pixels
             *
             * @return Height of Drawable in pixels, 0 if no data stored
             */
            int height();

            /**
             * @brief Destructor ensures contained data is deleted appropriately
             */
            ~Drawable();
    };
};

#endif