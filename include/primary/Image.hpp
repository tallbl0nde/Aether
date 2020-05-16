#ifndef AETHER_IMAGE_HPP
#define AETHER_IMAGE_HPP

#include "base/Texture.hpp"

namespace Aether {
    // An image is literally a texture that is created from an image.
    // Not much more to it than that!
    // If the render type is set to Deferred make sure path/pointer is still available when
    // you want to render the image!
    class Image : public Texture {
        /**
         * @brief Enum class for type of image
         */
        enum class Type {
            Path,       /*< Created from path string */
            Pointer     /*< Created from pointer */
        };

        private:
            /**
             * @brief Type of image
             */
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

            /**
             * @brief x scaling factor
             */
            int xF;

            /**
             * @brief y scaling factor
             */
            int yF;

            /**
             * @brief Creates the image as a surface
             */
            void generateSurface();

        public:
            // Both constructors take coordinates
            // Takes path to image to read and render
            Image(int, int, std::string, int = 1, int = 1, RenderType = RenderType::OnCreate);
            // Takes pointer to image and size + optional factors to scale down (advanced!)
            Image(int, int, u8 *, size_t, int = 1, int = 1, RenderType = RenderType::OnCreate);
    };
};

#endif