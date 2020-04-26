#ifndef AETHER_EXP_THREADEDIMAGE_HPP
#define AETHER_EXP_THREADEDIMAGE_HPP

#include "experimental/base/Threaded.hpp"
#include "primary/Image.hpp"

namespace Aether::Exp {
    // Threaded version of Image
    class ThreadedImage : public Threaded, public Image {
        enum Type {
            Path,       // Created from path string
            Pointer     // Created from pointer
        };

        private:
            // Override threaded functions to handle box
            void generateSurface();
            void convertSurface();
            void removeTexture();

            // Type variables
            Type type;
            std::string path;
            u8 * ptr;
            size_t size;
            int f1, f2;

        public:
            // Both constructors take coordinates and threadqueue
            // Takes path to image to read and render
            // Note the file must exist until the image is rendered!
            ThreadedImage(ThreadQueue *, int, int, std::string, int = 1, int = 1, RenderType = RenderType::OnCreate);
            // Takes pointer to image and size + optional factors to scale down (advanced!)
            // Note that the data pointed to must be available until the image is rendered!
            ThreadedImage(ThreadQueue *, int, int, u8 *, size_t, int = 1, int = 1, RenderType = RenderType::OnCreate);

            // Check if rendering done
            void update(uint32_t);
    };
};

#endif