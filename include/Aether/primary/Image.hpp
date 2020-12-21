#ifndef AETHER_IMAGE_HPP
#define AETHER_IMAGE_HPP

#include "Aether/base/Texture.hpp"

namespace Aether {
    /**
     * @brief Element for rendering an image, either from memory or a file system.
     * @note If rendering without copying, ensure the data is still accessible
     * until rendering is complete.
     */
    class Image : public Texture {
        private:
            /**
             * @brief Enumeration for types of image
             */
            enum class Type {
                File,               /**< Read from file */
                Pointer,            /**< Read from raw memory */
                Vector,             /**< Read from copied vector */
            };

            /**
             * @brief Struct for keeping pointer to data and it's size
             */
            struct Ptr {
                unsigned char * ptr;
                size_t size;
            } mem;
            std::vector<unsigned char> copy;    /** @brief Copy of image data */
            std::string path;                   /** @brief Path to image file */
            Type type;                          /** @brief Type of image */

            /**
             * @brief Overrides Texture's method to render the image.
             */
            Drawable * renderDrawable();

            /**
             * @brief Small helper to render based on passed type.
             *
             * @param type Type of rendering to perform
             */
            void performRender(const Render type);

        public:
            /**
             * @brief Constructs a new Image element using a pointer to data.
             * @note This data is not copied, pass a vector if copying is preferred.
             *
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param data Pointer to image data
             * @param size Size of data in bytes
             * @param type \ref Render Type of rendering to perform
             */
            Image(const int x, const int y, unsigned char * data, const size_t size, const Render type = Render::Sync);

            /**
             * @brief Constructs a new Image element using a file.
             *
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param path Path to image file
             * @param type \ref Render Type of rendering to perform
             */
            Image(const int x, const int y, const std::string & path, const Render type = Render::Sync);

            /**
             * @brief Constructs a new Image element using a vector of image data, which is copied.
             *
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param data Vector of image data
             * @param type \ref Render Type of rendering to perform
             */
            Image(const int x, const int y, const std::vector<unsigned char> & data, const Render type = Render::Sync);

            /**
             * @brief Destroys the image element.
             */
            ~Image();
    };
};

#endif