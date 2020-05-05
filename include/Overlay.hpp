#ifndef AETHER_OVERLAY_HPP
#define AETHER_OVERLAY_HPP

#include "Screen.hpp"

namespace Aether {
    /**
     * @brief An object representing an overlay.
     * 
     * Overlay is just a screem with a transparent black
     * background.
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
             * @brief Mark to overlay to be closed
             */
            void close();

            /**
             * @brief Check if overlay is marked to be closed
             * 
             * @return true if marked to close
             * @return false 
             */
            bool shouldClose();

            /**
             * @brief Draws background then renders screen
             */
            void render();
    };
};

#endif