#ifndef AETHER_IMAGEDATA_HPP
#define AETHER_IMAGEDATA_HPP

#include "Aether/types/Colour.hpp"
#include <cstdint>
#include <vector>

namespace Aether {
    /**
     * @brief Represents an object containing the raw pixel data for an image/texture
     * as an array, along with required metadata such as width and height.
     *
     * @note Only RGB and RGBA (i.e. 3 or 4 channels) are supported.
     */
    class ImageData {
        private:
            std::vector<Colour> pixels_;        /** @brief Vector of pixels from top-left to bottom-right */
            size_t width_;                      /** @brief Width of image in pixels */
            size_t height_;                     /** @brief Height of image in pixels */
            uint8_t channels_;                  /** @brief The number of channels in the image (usually 3 or 4) */

        public:
            /**
             * @brief Constructs a new (invalid) ImageData object.
             */
            ImageData();

            /**
             * @brief Constructs a new ImageData object from the given raw bytes.
             *
             * @param data Vector of bytes containing pixels in RGB(A) format.
             * @param width Width of image in pixels
             * @param height Height of image in pixels
             * @param channels Number of channels forming the image.
             */
            ImageData(const std::vector<uint8_t> & data, const size_t width, const size_t height, const uint8_t channels);

            /**
             * @brief Constructs a new ImageData object from the given raw bytes.
             *
             * @param pixels Vector of pixels.
             * @param width Width of image in pixels
             * @param height Height of image in pixels
             * @param channels Number of channels forming the image.
             */
            ImageData(const std::vector<Colour> & pixels, const size_t width, const size_t height, const uint8_t channels);

            /**
             * @brief Returns whether the associated image is considered 'valid' (i.e. has some pixel data).
             *
             * @return true if the stored image and metadata are valid values, false wtherwise.
             */
            bool valid() const;

            /**
             * @brief Returns the width of the stored image in pixels.
             *
             * @return Width of the stored image in pixels.
             */
            size_t width() const;

            /**
             * @brief Returns the height of the stored image in pixels.
             *
             * @return Height of the stored image in pixels.
             */
            size_t height() const;

            /**
             * @brief Returns the number of channels used within the stored image.
             * This will usually return 3 (RGB) or 4 (RGBA).
             *
             * @return Number of channels used in the stored image.
             */
            uint8_t channels() const;

            /**
             * @brief Returns a pointer to the colour of a pixel at the given coordinate.
             *
             * @param x x-coordinate of pixel
             * @param y y-coordinate of pixel
             * @return Pointer to pixel colour, or nullptr if outside of image
             */
            Colour * colourAt(const size_t x, const size_t y) const;

            /**
             * @brief Returns the stored image as a vector of bytes (i.e. one byte per channel).
             * e.g. For an RGB image: [r0, g0, b0, r1, g1, b1, etc...]
             *
             * @return Vector of image pixels in raw byte format.
             */
            std::vector<uint8_t> toByteVector() const;

            /**
             * @brief Returns the stored image as a vector of colours.
             *
             * @return Vector of image pixels.
             */
            std::vector<Colour> toColourVector() const;
    };
};

#endif