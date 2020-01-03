#include "Texture.hpp"

namespace Aether {

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