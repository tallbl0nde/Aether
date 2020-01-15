#ifndef AETHER_ELEMENT_HPP
#define AETHER_ELEMENT_HPP

#include "InputEvent.hpp"
#include "Types.hpp"
#include <functional>
#include <vector>

namespace Aether {
    // Element is the base class inherited from to form all other
    // types of elements. Thus, it contains positioning, rendering, callback, etc variables
    class Element {
        private:
            // Positioning and size variables
            int x_;
            int y_;
            int w_;
            int h_;

            // Parent element
            // Only nullptr if root element (ie. display)
            Element * parent;

            // Skip rendering this element + it's children
            bool hidden_;

            // Function to call when element is tapped/selected
            std::function<void()> callback_;
            // True if a child element is highlighted
            bool hasHighlighted_;
            // True if a child element is selectable
            bool hasSelectable_;
            // Texture to draw behind when highlighted
            SDL_Texture * highlightTex;
            // Is this element highlighted
            bool highlighted_;
            // Texture to draw on top when selected
            SDL_Texture * selectedTex;
            // Can this element be selected?
            bool selectable_;
            // Is this element selected? (ie. button held/touch held)
            bool selected_;
            // Can this element be touched?
            bool touchable_;

        protected:
            // Vector of child elements (used to call their methods)
            std::vector<Element *> children;

            // Function to regenerate highlighted texture using dimensions
            void generateHighlight();

            bool hasHighlighted();
            // Function to recursively set "hasHighlighted" on parent elements
            void setHasHighlighted(bool);

        public:
            // Constructor must be passed parent element (or nullptr if there is none)
            // Coordinates and size are optional, defaults to (0,0) with size (100, 100)
            Element(int = 0, int = 0, int = 100, int = 100);

            // Getters and setters for x, y, w, h + scale
            int x();
            int y();
            int w();
            int h();
            virtual void setX(int);
            virtual void setY(int);
            virtual void setW(int);
            virtual void setH(int);
            // Combines functions into one
            virtual void setXY(int, int);
            virtual void setWH(int, int);
            virtual void setXYWH(int, int, int, int);

            // Set given element as parent
            void setParent(Element *);
            // Add an element as a child
            virtual void addElement(Element *);
            // Delete the given child, returns false if not a child
            virtual bool removeElement(Element *);
            // Delete all children elements
            virtual void removeAllElements();

            // Returns whether this element is onscreen
            bool isVisible();

            // Returns hidden
            bool hidden();
            // Hide/show this element
            void setHidden(bool);

            // Returns selectable
            bool selectable();
            // Set whether element is selectable
            void setSelectable(bool);

            // Returns touchable
            bool touchable();
            // Set whether element is touchable
            void setTouchable(bool);

            bool highlighted();

            bool selected();
            void setSelected(bool);

            virtual void setActive();
            virtual void setInactive();

            // Returns callback function (nullptr if no callback assigned)
            std::function<void()> callback();
            // Set callback function (also marks element as selectable)
            void setCallback(std::function<void()>);

            // Handle the given event
            virtual bool handleEvent(InputEvent *);
            // Update is passed time since last frame (for animations)
            virtual void update(uint32_t);
            // Render child elements
            virtual void render();

            void setHighlighted(bool);

            bool hasSelectable();
            void setHasSelectable(bool);

            // Destructor calls destructor of children
            virtual ~Element();

            // Friend functions used to search through parents + children
            // Returns the element to "move" to given selected element and a condition. If there are multiple, it calls the provided
            // function to determine which one to select (returns element which evaluates given function closest to 0)
            friend Element * findElementToMoveTo(Element *, std::function<bool(Element *, Element *)>);
            // Recursively searches all children of given element and returns a vector containing all pointers.
            friend std::vector<Element *> getAllChildren(Element *, bool);
            // Return element that is highlighted within given element (returns nullptr if none found)
            friend Element * getHighlightedElement(Element *);

    };
};

#endif