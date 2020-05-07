#ifndef AETHER_CONTAINER_HPP
#define AETHER_CONTAINER_HPP

#include "Element.hpp"

namespace Aether {
    /**
     * @brief A container is an element that contains other elements.
     * 
     * It is required to automatically move between elements without
     * having to specify which ones are next to which.
     */
    class Container : public Element {
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
             * @brief Add new element to container
             * 
             * @param e element to add
             */
            void addElement(Element * e);

            /**
             * @brief Handles passed event. Attemps to handle with children first.
             * If not handled by children, it will "deactivate" on element and
             * "activate" the one it moves to
             * 
             * @param e event to handle
             * @return true if event was handled
             * @return false otherwise
             */
            bool handleEvent(InputEvent * e);

            /**
             * @brief Re-highlight/focus the focused element
             */
            void setActive();

            /**
             * @brief Removes focus from the focused element
             */
            void setInactive();

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