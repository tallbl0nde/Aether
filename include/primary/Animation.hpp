#ifndef AETHER_ANIMATION_HPP
#define AETHER_ANIMATION_HPP

#include "base/Element.hpp"

namespace Aether {
    // An animation is an element containing a list of textures which are changed
    // through to create the animation. Frames are managed by using *Element calls.
    // Note that removing an element will delete it!!
    // Resizing this element does not resize the frames - it is up to you!
    class Animation : public Element {
        private:
            // Index of currently shown texture
            unsigned int idx;
            // Time each frame should be shown for in milliseconds
            int frameTime;
            // Time spent on current frame in milliseconds
            int currTime;
            // Pauses animation when set true
            bool paused;

        public:
            // X, Y, W, H (all added textures should have same dimensions!!)
            Animation(int, int, int, int);

            // Updates handles switching between frames
            void update(uint32_t);
            // All children aren't rendered - only the current one
            void render();

            // Pause/resume the animation
            void pause();
            void resume();
            bool isPaused();

            // Set time for each frame
            void setAnimateSpeed(int);
            // Returns animation time
            unsigned int animateSpeed();

            // Set current frame to element at given index
            // Returns false if outside of range
            bool setFrameIndex(unsigned int);

            // Set current frame to given element
            // Returns false if element is not a part of the animation
            bool setFrameElement(Element *);
    };
};

#endif