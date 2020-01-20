#ifndef AETHER_LIST_HPP
#define AETHER_LIST_HPP

#include "base/Scrollable.hpp"

namespace Aether {
    // A list is a scrollable with some values changed to match how a list functions
    // within Horizon.
    class List : public Scrollable {
        private:
            // SCROLLING VARS
            Button heldButton;
            bool scroll;

        public:
            // Constructor shows scrollbar
            List(int, int, int, int);

            // Monitors events in order to handle scrolling without a selectable element
            bool handleEvent(InputEvent *);
            // List has a custom update as it allows the selection to be
            // anywhere in view and will only scroll if it is going to move off
            void update(uint32_t);
    };
};

#endif