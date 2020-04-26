#ifndef AETHER_EXP_THREADEDBOX_HPP
#define AETHER_EXP_THREADEDBOX_HPP

#include "experimental/base/Threaded.hpp"
#include "primary/Box.hpp"

namespace Aether::Exp {
    // Threaded version of Box
    // IMPORTANT: Does not handle rounding, the corner radius will be ignored at the moment
    class ThreadedBox : public Threaded, public Box {
        private:
            // Override threaded functions to handle box
            void generateSurface();
            void convertSurface();
            void removeTexture();

            // Don't actually create a texture when called - queue surface instead
            void redrawTexture();

        public:
            // Constructor accepts threadqueue, x, y, w, h, border, corner radius
            ThreadedBox(ThreadQueue *, int, int, int, int, unsigned int = 1, unsigned int = 0, RenderType = RenderType::OnCreateDeferred);

            // Check if rendering done
            void update(uint32_t);
    };
};

#endif