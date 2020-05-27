#ifndef AETHER_OVERLAY_HPP
#define AETHER_OVERLAY_HPP

#include "Aether/Screen.hpp"

namespace Aether {
    /**
     * @brief An object representing an overlay.
     *
     * An Overlay is a screen that draws a transparent black background before any
     * children elements. While Aether handles displaying and closing overlays it
     * does not delete them. That part is left to you!
     */
    class Overlay : public Screen {
        private:
            /** @brief Store value to check if overlay should be closed or not. */
            bool close_;

        public:
            /**
             * @brief Construct a new Overlay object
             */
            Overlay();

            /**
             * @brief Mark overlay to be closed
             */
            void close();

            /**
             * @brief Mark overlay to be able to used (readded) again
             * @note This shouldn't ever needed to be called apart from internally!
             */
            void reuse();

            /**
             * @brief Check if overlay is marked to be closed
             *
             * @return true if marked to close
             * @return false otherwise
             */
            bool shouldClose();

            /**
             * @brief Draws background then renders overlay
             */
            void render();
    };
};

#endif