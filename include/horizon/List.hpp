#ifndef AETHER_LIST_HPP
#define AETHER_LIST_HPP

#include "base/Scrollable.hpp"

namespace Aether {
    // A list is a scrollable with some values changed to match how a list functions
    // within Horizon.
    class List : public Scrollable {
        public:
            // Constructor shows scrollbar
            List(int, int, int, int);
    };
};

#endif