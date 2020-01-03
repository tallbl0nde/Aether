#ifndef AETHER_ELEMENT_HPP
#define AETHER_ELEMENT_HPP

#include "InputEvent.hpp"
#include <functional>
#include <vector>

namespace Aether {
    // Element is the base class inherited from to form all other
    // types of elements. Thus, it contains positioning, rendering, callback, etc variables
    // Each element has a unique ID which can be used to delete/fetch it from another
    // element.
    class Element {
        private:
            // Positioning and size variables
            int x_;
            int y_;
            int w_;
            int h_;
            // Scale this element (and it's children) by this amount
            double scale_;

            // Parent element (obtained by passing to constructor)
            // Only nullptr if root element (ie. display)
            Element * parent;

            // Skip rendering this element
            bool hidden_;

            // Function to call when element is tapped/selected
            std::function<void()> callback_;
            // Can this element be selected?
            bool selectable_;

        protected:
            // Vector of child elements (used to call their methods)
            std::vector<Element *> children;

        public:
            // Constructor must be passed parent element (or nullptr if there is none)
            // Coordinates and size are optional, defaults to (0,0) with size (100, 100)
            Element(Element *, int = 0, int = 0, int = 100, int = 100);

            // Getters and setters for x, y, w, h + scale
            int x();
            int y();
            int w();
            int h();
            void setX(int);
            void setY(int);
            void setW(int);
            void setH(int);
            double scale();
            void setScale(double);

            // Add an element as a child
            void addElement(Element *);
            // Delete the given child, returns false if not a child
            bool removeElement(Element *);
            // Delete all children elements
            void removeAllElements();

            // Returns hidden
            bool hidden();
            // Hide/show this element
            void setHidden(bool);

            // Returns selectable
            bool selectable();
            // Set whether element is selectable
            void setSelectable(bool);

            // Returns callback function (nullptr if no callback assigned)
            std::function<void()> callback();
            // Set callback function (also marks element as selectable)
            void setCallback(std::function<void()>);

            // Handle the given event
            virtual void handleEvent(InputEvent *);
            // Update is passed time since last frame (for animations)
            virtual void update(uint32_t);
            // Render child elements
            virtual void render();

            // Destructor calls destructor of children
            virtual ~Element();
    };
};

#endif