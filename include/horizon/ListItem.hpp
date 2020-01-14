#ifndef AETHER_LISTITEM_HPP
#define AETHER_LISTITEM_HPP

#include "base/Element.hpp"

namespace Aether {
    // A ListItem is just an element but is used as a container. It is specified
    // to restrict adding any kind of element to a list by instead only allowing
    // a "container"/group of elements to be added
    class ListItem : public Element {

    };
};

#endif