#ifndef AETHER_SCROLLABLE_HPP
#define AETHER_SCROLLABLE_HPP

#include "Element.hpp"

namespace Aether {
    // A scrollable element has it's children rendered onto a
    // texture, which is then "scrolled" and only a portion of
    // it rendered (useful for lists)
    class Scrollable : public Element {
        private:
            // Is the element scrolling? (used for touch events)
            bool isScrolling;
            // Amount to decrease velocity by (per second)
            float scrollDampening;
            // Scroll velocity (amount to scroll per second)
            float scrollVelocity;

            // Offset (y) in pixels
            unsigned int scrollPos;
            // Maximum Y offset in pixels
            unsigned int maxScrollPos;

            // Check all children and determine maximum height
            void updateMaxScrollPos();

        public:
            // Must pass parent
            Scrollable(Element *);

            // Getter + Setter for dampening
            float dampening();
            void setDampening(float);

            // (Re-)calculate maximum scroll position whenever an element is added/removed
            void addElement(Element *);
            bool removeElement(Element *);
            void removeAllElements();

            // Update handles scrolling after touch up
            void update(uint32_t);
            // Render also draws scroll bar if applicable
            void render();

            // Destructor simply calls element's destructor
            ~Scrollable();
    };
};

#endif