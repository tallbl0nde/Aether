#ifndef AETHER_IMAGE_HPP
#define AETHER_IMAGE_HPP

#include "base/Texture.hpp"

namespace Aether {
    /**
     * @brief An image is literally a texture that is created from
     * an image. Not much more to it than that!
     */
    class Image : public Texture {
        private:
            /**
             * @brief Does nothing. Image textures doesn't
             * need to be redrawn.
             */
            void redrawTexture();

        public:
            /**
             * @brief Construct a new Image object.
             * Image is rendered when object is first created.
             * 
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param p path to image file
             */
            Image(int x, int y, std::string p);

            /**
             * @brief Construct a new Image object
             * 
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param p pointer to image data start
             * @param s image data size
             * @param f1 shrink factor for x-coordinate
             * @param f2 shrink factor for y-coordinate
             */
            Image(int x, int y, u8 * p, size_t s, int f1 = 1, int f2 = 1);
    };
};

#endif