#ifndef AETHER_ANIMATION_HPP
#define AETHER_ANIMATION_HPP

#include "Aether/base/Element.hpp"

namespace Aether {
    /**
     * @brief An animation is an element containing a list of textures which
     * are changed through to create the animation. Frames are managed by using
     * *Element calls.
     *
     * @note Removing an element will delete it!
     * @note Resizing this element does not resize the frames - it is up to you!
     */
    class Animation : public Element {
        private:
            /** @brief Index of currently shown texture */
            unsigned int idx;
            /** @brief Time each frame should be shown for in milliseconds */
            int frameTime;
            /** @brief Time spent on current frame in milliseconds */
            int currTime;
            /** @brief Pauses animation when set true */
            bool paused;

        public:
            /**
             * @brief Construct a new Animation object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param w width of animation
             * @param h height of animation
             */
            Animation(int x, int y, int w, int h);

            /**
             * @brief Updates handles switching between frames
             *
             * @param dt change in time
             */
            void update(unsigned int dt);

            /**
             * @brief All children aren't rendered - only the current one
             */
            void render();

            /**
             * @brief Pause the animation
             */
            void pause();

            /**
             * @brief Resume the animation
             */
            void resume();

            /**
             * @brief Check if animation is paused
             *
             * @return true if animation is paused
             * @return false otherwise
             */
            bool isPaused();

            /**
             * @brief Set time for each frame
             *
             * @param t time for each frame
             */
            void setAnimateSpeed(int t);

            /**
             * @brief Returns animation time
             *
             * @return animation time
             */
            unsigned int animateSpeed();

            /**
             * @brief Set current frame to element at given index
             *
             * @param i element index
             * @return true if within range
             * @return false otherwise
             */
            bool setFrameIndex(unsigned int i);

            /**
             * @brief Set current frame to given element
             *
             * @param e element to set to
             * @return true if element is not a part of the animation
             * @return false otherwise
             */
            bool setFrameElement(Element * e);
    };
};

#endif