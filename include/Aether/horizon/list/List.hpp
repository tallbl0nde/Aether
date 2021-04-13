#ifndef AETHER_LIST_HPP
#define AETHER_LIST_HPP

#include "Aether/base/Scrollable.hpp"

namespace Aether {
    /**
     * @brief A list is a scrollable with some values changed to match how a list functions
     * within Horizon.
     */
    class List : public Scrollable {
        private:
            /** @brief Button that is being currently held, if any */
            Button heldButton;
            /** @brief Indicator on whether list is being scrolled */
            bool scroll;
            bool waitUntilVisible;
            /** @brief Whether to wrap around */
            bool wrapAround_;

            /**
             * @brief Returns whether a selectable element is visible
             *
             * @return true if so, false otherwise
             */
            bool hasVisibleItem();

        public:
            /**
             * @brief Construct a new List object. The scrollbar is shown by default.
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param w width of list
             * @param h height of list
             * @param p type of padding to use (see \ref ::Padding)
             */
            List(int x, int y, int w, int h, Padding p = Padding::Default);

            /**
             * @brief A list needs to stop scrolling when it is set inactive
             */
            void setInactive();

            /**
             * @brief Returns whether the list is configured to wrap around
             *
             * @return true if it is
             * @return false otherwise
             */
            bool wrapAround();

            /**
             * @brief Set whether the list 'wraps around' from top to bottom and vice versa
             *
             * @param b true to wrap around, false to not
             */
            void setWrapAround(bool b);

            /**
             * @brief Attempts to handle event
             *
             * @param e event to attempt handle
             * @return true if event was handled
             * @return false otherwise
             */
            bool handleEvent(InputEvent * e);

            /**
             * @brief Updates info as necessary
             *
             * @param dt change in time
             */
            void update(unsigned int dt);
    };
};

#endif