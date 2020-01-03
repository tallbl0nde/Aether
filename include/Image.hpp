#include "Texture.hpp"

namespace Aether {
    // An image is literally a texture that is created from an image.
    // Not much more to it than that!
    class Image : public Texture {
        private:
            // Does nothing
            void redrawTexture();

        public:
            // Takes path to image to read and render
            Image(Element *, std::string);
            // Takes pointer to image and size
            Image(Element *, u8 *, size_t);

            ~Image();
    };
};