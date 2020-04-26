#ifndef AETHER_EXP_THREADED_HPP
#define AETHER_EXP_THREADED_HPP

#include <atomic>
#include "utils/SDLHelper.hpp"
#include "utils/ThreadQueue.hpp"
#include "utils/Types.hpp"

namespace Aether::Exp {
    enum class ThreadedStatus {
        Empty,		// No texture stored/queued
        Queued,		// Texture queued/rendering
        Surface,	// Surface rendered and ready to convert
        Texture		// Texture rendered and ready to display
    };

    // Threaded allows a function (i.e. rendering a surface) to be run alongside
    // the UI thread by running on another thread. It cannot be instantiated.
    class Threaded {
        private:
            // Pointer to threadqueue to run rendering ops on
            ThreadQueue * tq;
            // Current status
            std::atomic<ThreadedStatus> status;
            // Function to generate surface (called in another thread!)
            virtual void generateSurface() = 0;
            // Function to call when surface is ready (called in main thread!)
            virtual void convertSurface() = 0;

            // Queues surface creation - calls generateSurface() when next in queue
            void createSurface();

        protected:
            // Rendering type (used in derived classes to determine when to queue generation)
            RenderType renderType;
            // Rendered surface
            SDL_Surface * surface;

        public:
            // Constructor takes pointer to Aether::ThreadQueue and RenderType
            Threaded(ThreadQueue *, RenderType);

            // Adds rendering operation to the queue (returns false if an operation is already queued/running)
            bool startRendering();

            // Returns true when the texture is ready to be drawn
            bool textureReady();

            // Check when surface is ready and calls convertSurface
            void updateState();

            ~Threaded();
    };
};

#endif