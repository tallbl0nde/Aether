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
            void update(uint32_t dt);
    };
};

#endif