#ifndef AETHER_SCROLLABLE_HPP
#define AETHER_SCROLLABLE_HPP

#include "Container.hpp"

namespace Aether {
    /**
     * @brief A scrollable element has it's children rendered 
     * onto a texture, which is then "scrolled" and only a
     * portion of it rendered (useful for lists)
     */
    class Scrollable : public Container {
        private:
            /** @brief Texture to render to */
            SDL_Texture * renderTex;
            /** @brief Scroll bar texture */
            static SDL_Texture * scrollBar;
            /** @brief Colour to tint scroll bar */
            Colour scrollBarColour;
            /** @brief Indicator on whether to show the scroll bar */
            bool showScrollBar_;

            /**
             * @brief Check all children and determine maximum height
             */
            void updateMaxScrollPos();

            /**
             * @brief Stops scrolling and sets active element
             */
            void stopScrolling();

        protected:
            /** @brief Indicator on whether scrolling is allowed */
            bool canScroll_;
            /** @brief Indicator on whether this scrollable has been touched (used for scrolling when touch is outside) */
            bool isTouched;
            /** @brief Indicator on whether is the element is scrolling (used for touch events) */
            bool isScrolling;
            /** @brief Amount to decrease velocity by (per second) */
            float scrollDampening;
            /** @brief Scroll velocity (amount to scroll per second) */
            float scrollVelocity;
            /** @brief Start of touch Y coord (used to touch instead of scroll briefly) */
            int touchY;
            /** @brief Maximum Y offset in pixels */
            unsigned int maxScrollPos;
            /** @brief Amount to "catchup" by */
            float scrollCatchup;
            /** @brief Offset (y) in pixels */
            unsigned int scrollPos;

            /**
             * @brief Set scrollPos but check if going to be outside of range
             * and if so set to min/max
             * 
             * @param pos new scroll position
             */
            void setScrollPos(int pos);

        public:
            /**
             * @brief Construct a new Scrollable object
             * 
             * @param x x-coordinate of scrollable element
             * @param y y-coordinate of scrollable element
             * @param w width of scrollable element
             * @param h height of scrollable element
             */
            Scrollable(int x, int y, int w, int h);

            /**
             * @brief Set new width for scrollable element
             * 
             * Setting width needs to adjust width of elements.
             * @param w 
             */
            void setW(int w);

            /**
             * @brief Set new height for scrollable element
             * 
             * Setting height needs to recalculate scroll bar
             * @param h 
             */
            void setH(int h);

            /**
             * @brief Get amount to "catchup" by
             * 
             * @return amount to catchup by 
             */
            int catchup();

            /**
             * @brief Set amount to "catchup" by
             * 
             * @param c new catchup value
             */
            void setCatchup(int c);

            /**
             * @brief Get amount to decrease velocity by (per second)
             * 
             * @return decrease velocity
             */
            float dampening();

            /**
             * @brief Set amount to decrease velocity by (per second)
             * 
             * @param d new decrease velocity
             */
            void setDampening(float d);

            /**
             * @brief Check if scroll bar is shown
             * 
             * @return true if scroll bar is shown
             * @return false otherwise
             */
            bool showScrollBar();

            /**
             * @brief Set whether scroll bar is shown
             * 
             * @param b true if scroll bar is shown, false otherwise
             */
            void setShowScrollBar(bool b);

            /**
             * @brief Set scroll bar colour
             * 
             * @param c colour to set to
             */
            void setScrollBarColour(Colour c);

            /**
             * @brief Set scroll bar colour
             * 
             * @param r Red value of colour
             * @param g Green value of colour
             * @param b Blue value of colour
             * @param a Alpha value of colour
             */
            void setScrollBarColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

            /**
             * @brief Check whether this element can scrolled (by touch/drag)
             * 
             * @return true if element is scrollable
             * @return false otherwise
             */
            bool canScroll();

            /**
             * @brief Set whether this element can be scrolled
             * 
             * @param b true if element is scrollable, false otherwise
             */
            void setCanScroll(bool b);

            /**
             * @brief Add new children element to this scrollable
             * element.
             * 
             * (Re-)calculate maximum scroll position whenever an element is added/removed.
             * 
             * @param e element to add
             */
            void addElement(Element * e);

            /**
             * @brief Attempt to a children element from 
             * this scrollable element
             * 
             * @param e element to remove
             * @return true if removed successfully
             * @return false otherwise
             */
            bool removeElement(Element * e);

            /**
             * @brief Removes all elements associated with this scrollable element
             */
            void removeAllElements();

            /**
             * @brief Removes any elements added **AFTER** the given element, disregarding position in list!
             * @note Advanced Usage! Make sure to know what you are doing!
             * 
             * @param e element to start from
             * @return true if following elements were found for the given element
             * @return false otherwise
             */
            bool removeFollowingElements(Element * e);

            /**
             * @brief Handles scrolling
             * 
             * @param e scrolling event to handle
             * @return true if handled successfully
             * @return false otherwise
             */
            bool handleEvent(InputEvent * e);

            /**
             * @brief Update handles scrolling after touch up
             * 
             * @param dt change in time
             */
            void update(uint32_t dt);

            /**
             * @brief Render also draws scroll bar if applicable
             */
            void render();

            /**
             * @brief Delete scroll bar texture
             */
            ~Scrollable();
    };
};

#endif