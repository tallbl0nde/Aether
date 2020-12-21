#ifndef AETHER_SCROLLABLE_HPP
#define AETHER_SCROLLABLE_HPP

#include "Aether/base/Container.hpp"

namespace Aether {
    /**
     * @brief A scrollable element arranges all of it's children as a list.
     * It's children are rendered and 'cut off' outside the dimensions of the scrollable object.
     * Note that added elements will have their width changed to match the list!
     * Also note that elements are placed directly below the previous element (in terms of y-coords)!
     */
    class Scrollable : public Container {
        private:
            /** @brief Whether to have padding */
            Padding paddingType;
            /** @brief Scroll bar texture */
            Drawable * scrollBar;
            /** @brief Colour to tint scroll bar */
            Colour scrollBarColour;
            /** @brief Whether to show the scroll bar */
            bool showScrollBar_;

            /**
             * @brief Returns amount of padding for one side based on padding type
             */
            int paddingAmount();

            /**
             * @brief Stops scrolling and sets first selectable element as active
             */
            void stopScrolling();

        protected:
            /** @brief Indicator on whether scrolling is allowed */
            bool canScroll_;
            /** @brief Indicator on whether this scrollable has been touched (used for scrolling when touch is outside) */
            bool isTouched;
            /** @brief Indicator on whether is the element is scrolling (used for touch events) */
            bool isScrolling;
            /** @brief Amount to decrease velocity by (oixels per second) */
            float scrollDampening;
            /** @brief Scroll velocity (pixels to scroll per second) */
            float scrollVelocity;
            /** @brief Start of touch Y coord (used to touch element instead of scroll briefly) */
            int touchY;
            /** @brief Maximum Y offset in pixels */
            unsigned int maxScrollPos_;
            /** @brief Amount to "catchup" by */
            float scrollCatchup;
            /** @brief Offset (y) in pixels */
            unsigned int scrollPos_;

            void addElementAt(Element * e, size_t i);

            /**
             * @brief Sums up height of children and sets maximum scroll value
             */
            void updateMaxScrollPos();

        public:
            /**
             * @brief Construct a new Scrollable object
             *
             * @param x x-coordinate of scrollable element
             * @param y y-coordinate of scrollable element
             * @param w width of scrollable element
             * @param h height of scrollable element
             * @param p type of padding to use (see \ref ::Padding)
             */
            Scrollable(int x, int y, int w, int h, Padding p = Padding::Default);

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
             * @brief Get amount to decrease velocity by (pixels per second)
             *
             * @return decrease velocity
             */
            float dampening();

            /**
             * @brief Set amount to decrease velocity by (pixels per second)
             *
             * @param d new decrease velocity
             */
            void setDampening(float d);

            /**
             * @brief Returns if the scroll bar is being shown
             *
             * @return true if scroll bar is shown
             * @return false otherwise
             */
            bool showScrollBar();

            /**
             * @brief Set whether to show scroll bar
             *
             * @param b true to show scroll bar, false otherwise
             */
            void setShowScrollBar(bool b);

            /**
             * @brief Set scroll bar colour
             *
             * @param c colour to set to
             */
            void setScrollBarColour(Colour c);

            /**
             * @brief Check whether this element can be scrolled (by touch/drag)
             *
             * @return true if element is scrollable
             * @return false otherwise
             */
            bool canScroll();

            /**
             * @brief Set whether this element can be scrolled by touching and dragging
             *
             * @param b true if element is scrollable, false otherwise
             */
            void setCanScroll(bool b);

            /**
             * @brief Returns the maximum value permitted for scrollPos (i.e. the limit).
             * @note It cannot be set as it is done so by the object when items are changed
             *
             * @return int specifying maximum scroll position allowed
             */
            int maxScrollPos();

            /**
             * @brief Returns current scroll position of list (0 indicates at the top)
             *
             * @return int specifying scroll position
             */
            int scrollPos();

            /**
             * @brief Sets scrollPos. Set to min/max if outside valid range
             * @note This should only need to be used in rare-ish cases (such as jumping to the top of a list after pressing a button)
             *
             * @param pos new scroll position
             */
            void setScrollPos(int pos);

            /**
             * @brief Add the given element to the list
             * @note The element's width will be set to match the width of the scrollable object
             *
             * @param e element to add
             */
            void addElement(Element * e);

            /**
             * @brief Adds an element to the scrollable object after the specified element.
             * Pass nullptr to insert at the start
             * @note The element to add is not deleted if it was not added!
             *
             * @param e element to add
             * @param a element to insert after
             *
             * @return true if the preceding element was found and the element added
             * @return false if the preceding element was not found
             */
            bool addElementAfter(Element * e, Element * a);

            /**
             * @brief Adds an element to the scrollable object before the specified element.
             * Pass nullptr to insert at the end (note: this is the same as \ref addElement())
             * @note The element to add is not deleted if it was not added!
             *
             * @param e element to add
             * @param b element to insert before
             *
             * @return true if the succeeding element was found and the element added
             * @return false if the succeeding element was not found
             */
            bool addElementBefore(Element * e, Element * b);

            /**
             * @brief Deletes any elements succeeding the given element
             *
             * @param e element to start from
             *
             * @return true if following elements were found for the given element
             * @return false otherwise
             */
            bool removeElementsAfter(Element * e);

            /**
             * @brief Deletes any elements preceding the given element
             *
             * @param e element to start from
             *
             * @return true if prior elements were found for the given element
             * @return false otherwise
             */
            bool removeElementsBefore(Element * e);

            void setW(int w);
            void setH(int h);

            bool removeElement(Element * e);
            void removeAllElements();
            bool returnElement(Element * e);
            void returnAllElements();

            bool handleEvent(InputEvent * e);
            void update(unsigned int dt);
            void render();

            /**
             * @brief Delete the scrollable object
             */
            ~Scrollable();
    };
};

#endif