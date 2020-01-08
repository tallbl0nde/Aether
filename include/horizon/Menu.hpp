#ifndef AETHER_MENU_HPP
#define AETHER_MENU_HPP

#include "base/Scrollable.hpp"
#include "MenuItem.hpp"

namespace Aether {
    // A Menu only accepts MenuItems and presents them as a list.
    class Menu : public Scrollable {
        private:
            // Vector of items IN ORDER
            std::vector<MenuItem *> items;

            // Prevent interfering with elements directly
            using Element::addElement;
            using Element::removeElement;
            using Element::removeAllElements;

            // Positions each menuitem's Y coordinate
            void positionItems();

        public:
            Menu();

            // Add an item to the menu (only accepts MenuItems, not Elements)
            void addItem(MenuItem *);
            // Remove an item from the menu
            // The provided item is deleted if found, so the pointer can be discarded
            bool removeItem(MenuItem *);
            // Remove all items from the list
            void removeAllItems();

            // Set active element (should only be needed at creation)
            // Returns true on success, false otherwise
            bool setActiveItem(MenuItem *);

            // Setter for colours
            void setActiveColour(Colour);
            void setActiveColor(Color);
            void setInactiveColour(Colour);
            void setInactiveColor(Color);
    };
};

#endif