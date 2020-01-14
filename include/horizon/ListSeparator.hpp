#ifndef AETHER_LISTSEPARATOR_HPP
#define AETHER_LISTSEPARATOR_HPP

#include "ListItem.hpp"

// Default height
#define DEFAULT_HEIGHT 45

namespace Aether {
    // ListSeparator is simply a blank element. It has a default height
    // which matches Horizon's but can be changed if necessary using setH()
    class ListSeparator : public ListItem {
        public:
            // Constructor optionally takes height of separator
            ListSeparator(unsigned int = 45);
    };
};

#endif