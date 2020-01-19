#ifndef AETHER_OVERLAY_HPP
#define AETHER_OVERLAY_HPP

#include "Screen.hpp"

namespace Aether {
    // An overlay is just a screen but with a transparent
    // black background.
    class Overlay : public Screen {
        private:
            // Background texture is shared between all overlays
            static SDL_Texture * background;

            // Set true when it should be closed
            bool close_;

        public:
            Overlay();

            // Call to mark the overlay to be closed
            void close();
            // Returns close
            bool shouldClose();

            // Render calls Screen but draws background first
            void render();

            ~Overlay();
    };
};

#endif