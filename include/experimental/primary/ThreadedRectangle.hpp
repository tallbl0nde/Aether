#ifndef AETHER_EXP_THREADEDRECTANGLE_HPP
#define AETHER_EXP_THREADEDRECTANGLE_HPP

#include "experimental/base/Threaded.hpp"
#include "primary/Rectangle.hpp"

namespace Aether::Exp {
    // Threaded version of Rectangle
    // IMPORTANT: Does not handle rounding, the corner radius will be ignored at the moment
    class ThreadedRectangle : public Threaded, public Rectangle {
        private:
            // Override threaded functions to handle rectangle
            void generateSurface();
            void convertSurface();
            void removeTexture();

            // Don't actually create a texture when called - queue surface instead
            void redrawTexture();

        public:
            // Constructor accepts threadqueue, x, y, w, h, corner radius
            ThreadedRectangle(ThreadQueue *, int, int, int, int, unsigned int = 0, RenderType = RenderType::OnCreateDeferred);

            // Check if rendering done
            void update(uint32_t);
    };
};

#endif