#ifndef AETHER_SCREEN_HPP
#define AETHER_SCREEN_HPP

#include "base/Element.hpp"

namespace Aether {
    // A screen represents a literal screen/layout. It stores all elements
    // to be drawn/interacted with on a single screen. It's similar to a
    // container object.
    class Screen : public Element {
        private:
            // Function to call when screen is switched to
            std::function<void()> active_func;
            // Function to call when screen is switched off of
            std::function<void()> inactive_func;

        public:
            // Constructor takes parent element and active/inactive functions
            Screen(Element *);

            // These should be called when switching screens
            // They finish setting up the screen/free memory
            void callActive();
            void callInactive();

            ~Screen();
    };
};

#endif