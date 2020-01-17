#ifndef AETHER_BOX_HPP
#define AETHER_BOX_HPP

#include "base/Texture.hpp"

namespace Aether {
    // A Box is a rectangle outline with no fill.
    class Box : public Texture {
        private:
            // Size of border
            unsigned int border_;

            // Redraw the box texture
            void redrawTexture();

        public:
            // Size is set to 1 by default
            Box(int, int, int, int, unsigned int = 1);

            // Getter + setter for border size
            unsigned int border();
            void setBorder(unsigned int);

            // Adjust box size
            void setBoxSize(int, int);
    };
};

#endif