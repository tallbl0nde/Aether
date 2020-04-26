#ifndef AETHER_EXP_THREADEDTEXTBLOCK_HPP
#define AETHER_EXP_THREADEDTEXTBLOCK_HPP

#include "experimental/base/Threaded.hpp"
#include "primary/TextBlock.hpp"

namespace Aether::Exp {
    // Threaded version of TextBlock
    class ThreadedTextBlock : public Threaded, public TextBlock {
        private:
            // Override threaded functions to handle text
            void generateSurface();
            void convertSurface();

            // Don't actually create a texture when called - queue surface instead
            void redrawTexture();

        public:
            // Constructor accepts threadqueue, x, y, string, font size wrap width and font type
            ThreadedTextBlock(ThreadQueue *, int, int, std::string, unsigned int, unsigned int, FontStyle = FontStyle::Regular, RenderType = RenderType::OnCreateDeferred);

            // Check if rendering done
            void update(uint32_t);
    };
};

#endif