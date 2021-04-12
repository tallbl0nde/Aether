#ifndef AETHER_UTILS_IMAGE_HPP
#define AETHER_UTILS_IMAGE_HPP

#include "Aether/types/ImageData.hpp"

namespace Aether::Utils::Image {
    /**
     * @brief Scales the provided image to the given dimensions using
     * bicubic interpolation.
     *
     * @param image Image to scale
     * @param width Width (in pixels) to scale to
     * @param height Height (in pixels) to scale to
     * @return Scaled image, or nullptr if an error occurred
     */
    ImageData * scaleBicubic(ImageData * image, const size_t width, const size_t height);

    /**
     * @brief Scales the provided image to the given dimensions using
     * box sampling.
     * @note Currently corrupts the image if scaling up.
     *
     * @param image Image to scale
     * @param width Width (in pixels) to scale to
     * @param height Height (in pixels) to scale to
     * @return Scaled image, or nullptr if an error occurred
     */
    ImageData * scaleBoxSampling(ImageData * image, const size_t width, const size_t height);

    /**
     * @brief Scales the provided image to the given dimensions, picking the optimal
     * scaling algorithm.
     *
     * @param image Image to scale
     * @param width Width (in pixels) to scale to
     * @param height Height (in pixels) to scale to
     * @return Scaled image, or nullptr if an error occurred
     */
    ImageData * scaleOptimal(ImageData * image, const size_t width, const size_t height);
};

#endif