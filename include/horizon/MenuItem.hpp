#ifndef AETHER_MENUITEM_HPP
#define AETHER_MENUITEM_HPP

#include "base/Element.hpp"

namespace Aether {
    // A MenuItem is a placeholder for valid Menu elements. It cannot be
    // instantiated directly due to the constructor being protected.
    class MenuItem : public Element {
        protected:
            // Prevent interfering with elements directly
            using Element::addElement;
            using Element::removeElement;
            using Element::removeAllElements;
            using Element::setSelectable;

            MenuItem();
    };
};

#endif