#ifndef AETHER_IMAGE_HPP
#define AETHER_IMAGE_HPP

#include "base/Texture.hpp"

namespace Aether {
    /**
     * @brief An image is literally a texture that is created from
     * an image. Not much more to it than that!
     * @note If the render type is set to Deferred make sure the path/pointer is still available when
     * you want to render the image!
     */
    class Image : public Texture {
        /**
         * @brief Enum class for type of image
         */
        enum class Type {
            Path,       /*< Created from path string */
            Pointer     /*< Created from pointer */
        };

        private:
            /** @brief Type of image */
            Type type;

            /**
             * @brief Path to image file
             * @note Undefined value if created with a pointer to image!
             */
            std::string path;

            /**
             * @brief Pointer to image data
             * @note Undefined value if created with a file path!
             */
            u8 * ptr;

            /**
             * @brief Size of data pointed to by ptr
             * @note Undefined value if created with a file path!
             */
            size_t size;

            /** @brief x scaling factor */
            int xF;

            /** @brief y scaling factor */
            int yF;

            /** @brief Creates the image as a surface */
            void generateSurface();

        public:
            /**
             * @brief Construct a new Image object.
             * Image is rendered when object is first created.
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param p path to image file
             * @param xF x scaling factor
             * @param yF y scaling factor
             * @param t \ref ::RenderType to use for texture generation
             */
            Image(int x, int y, std::string p, int xF = 1, int yF = 1, RenderType t = RenderType::OnCreate);

            /**
             * @brief Construct a new Image object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param p pointer to image data start
             * @param s image data size
             * @param xF x scaling factor
             * @param yF y scaling factor
             * @param t \ref ::RenderType to use for texture generation
             */
            Image(int x, int y, u8 * p, size_t s, int xF = 1, int yF = 1, RenderType t = RenderType::OnCreate);
    };
};

#endif