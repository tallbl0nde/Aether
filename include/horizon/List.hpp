#ifndef AETHER_LIST_HPP
#define AETHER_LIST_HPP

#include "base/Scrollable.hpp"
#include "ListItem.hpp"

namespace Aether {
    // A list contains ListItems which are scrolled through by either touch
    // or controller input. Only visible elements are updated/rendered.
    class List : public Scrollable {
        private:
            // Vector of items IN ORDER
            std::vector<ListItem *> items;

            // Prevent interfering with elements directly
            using Element::addElement;
            using Element::removeElement;
            using Element::removeAllElements;

            void positionItems();

        public:
            // Constructor takes no parameters
            List();

            // Add an item to the menu (only accepts ListItems, not Elements)
            void addItem(ListItem *);
            // Remove an item from the menu
            // The provided item is deleted if found, so the pointer can be discarded
            bool removeItem(ListItem *);
            // Remove all items from the list
            void removeAllItems();
    };
};

#endif