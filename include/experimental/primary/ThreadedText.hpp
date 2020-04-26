#ifndef AETHER_EXP_THREADEDTEXT_HPP
#define AETHER_EXP_THREADEDTEXT_HPP

#include "experimental/base/Threaded.hpp"
#include "primary/Text.hpp"

namespace Aether::Exp {
    // Threaded version of Text
    class ThreadedText : public Threaded, public Text {
        private:
            // Override threaded functions to handle text
            void generateSurface();
            void convertSurface();

            // Don't actually create a texture when called - queue surface instead
            void redrawTexture();

        public:
            // Constructor accepts threadqueue, x, y, string, font size, and font type
            ThreadedText(ThreadQueue *, int, int, std::string, unsigned int, FontStyle = FontStyle::Regular, RenderType = RenderType::OnCreateDeferred);

            // Check if rendering done
            void update(uint32_t);
    };
};

#endif