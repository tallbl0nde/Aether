#ifndef AETHER_SCROLLABLE_HPP
#define AETHER_SCROLLABLE_HPP

#include "Container.hpp"

namespace Aether {
    // A scrollable element has it's children rendered onto a
    // texture, which is then "scrolled" and only a portion of
    // it rendered (useful for lists)
    class Scrollable : public Container {
        private:
            // Is the element scrolling? (used for touch events)
            bool isScrolling;
            // Amount to "catchup" by
            int scrollCatchup;
            // Amount to decrease velocity by (per second)
            float scrollDampening;
            // Scroll velocity (amount to scroll per second)
            float scrollVelocity;

            // Scroll bar texture
            SDL_Texture * scrollBar;
            // Colour to tint scroll bar
            Colour scrollBarColour;
            // Offset (y) in pixels
            unsigned int scrollPos;
            // Maximum Y offset in pixels
            unsigned int maxScrollPos;
            // Show the scrollbar?
            bool showScrollBar_;

            // Set scrollPos but check if going to be outside of range
            // and if so set to min/max
            void setScrollPos(int);
            // Check all children and determine maximum height
            void updateMaxScrollPos();

        public:
            Scrollable();

            // Setting height needs to recalculate scroll bar
            void setH(int);

            // Getter + setter for catchup
            int catchup();
            void setCatchup(int);

            // Getter + Setter for dampening
            float dampening();
            void setDampening(float);

            // Getter + Setter for showing scroll bar
            bool showScrollBar();
            void setShowScrollBar(bool);
            // Set scroll bar colour
            void setScrollBarColour(Colour);
            void setScrollBarColour(uint8_t, uint8_t, uint8_t, uint8_t);

            // (Re-)calculate maximum scroll position whenever an element is added/removed
            void addElement(Element *);
            bool removeElement(Element *);
            void removeAllElements();

            // Update handles scrolling after touch up
            void update(uint32_t);
            // Render also draws scroll bar if applicable
            void render();

            // Delete scroll bar texture
            ~Scrollable();
    };
};

#endif