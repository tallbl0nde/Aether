#ifndef AETHER_RECTANGLE_HPP
#define AETHER_RECTANGLE_HPP

#include "Texture.hpp"

namespace Aether {
    class Rectangle : public Texture {
        private:
            // Radius of each corner (draws rounded rectangle when > 0)
            unsigned int cornerRadius_;

            // Called to regenerate texture
            void renderTexture();

        public:
            // Constructor takes parent element and optional corner radius (defaults to 0)
            Rectangle(Element *, int, int, unsigned int = 0);

            // Getter + setter for cornerRadius
            unsigned int cornerRadius();
            void setCornerRadius(unsigned int);

            // Override setW and setH to re-render texture
            void setW(int);
            void setH(int);

            ~Rectangle();
    };
};

#endif