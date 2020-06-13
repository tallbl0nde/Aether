#ifndef AETHER_CONTAINER_HPP
#define AETHER_CONTAINER_HPP

#include "Aether/base/Element.hpp"

namespace Aether {
    /**
     * @brief A container is an element that contains other elements.
     * It implements the required behaviour to navigate between children.
     * @note It must be used when controller navigation is required between children!
     * Using a standard Element will result in navigation being ignored.
     *
     * It allows the selection to automatically move between elements without
     * having to specify which ones are next to which.
     */
    class Container : public Element {
        protected:
             void addElementAt(Element * e, size_t i);

        public:
            /**
             * @brief Construct a new Container object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param w width of container
             * @param h height of container
             */
            Container(int x = 0, int y = 0, int w = 100, int h = 100);

            /**
             * @brief Re-highlight/focus the focused element
             */
            void setActive();

            /**
             * @brief Removes focus from the focused element
             */
            void setInactive();

            void addElement(Element * e);
            bool handleEvent(InputEvent * e);
            bool removeElement(Element * e);
            void removeAllElements();
            bool returnElement(Element * e);
            void returnAllElements();

            /**
             * @brief Determine which element to move to
             *
             * @param parent parent element (usually this)
             * @param check function which returns true if size between elements is valid
             * @param dist function to return distance between two elements
             * @return true if valid element found to move to
             * @return false otherwise
             */
            friend bool moveHighlight(Container * parent, std::function<bool(Element *, Element*)> check, std::function<int(Element *, Element *)> dist);
    };
};

#endif