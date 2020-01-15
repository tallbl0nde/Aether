#ifndef AETHER_MENU_HPP
#define AETHER_MENU_HPP

#include "base/Scrollable.hpp"
#include "MenuItem.hpp"

namespace Aether {
    // A Menu only accepts MenuItems and presents them as a list.
    class Menu : public Scrollable {
        private:
            // Prevent interfering with elements directly
            using Element::addElement;
            using Element::removeElement;
            using Element::removeAllElements;

        public:
            Menu();

            // Add an item to the menu (only accepts MenuItems, not Elements)
            void addItem(MenuItem *);
            // Remove an item from the menu
            // The provided item is deleted if found, so the pointer can be discarded
            bool removeItem(MenuItem *);
            // Remove all items from the list
            void removeAllItems();
    };
};

#endif