#ifndef AETHER_SCREEN_HPP
#define AETHER_SCREEN_HPP

#include "base/Container.hpp"

namespace Aether {
    // A screen represents a literal screen/layout. It stores all elements
    // to be drawn/interacted with on a single screen.
    class Screen : public Container {
        public:
            // Constructor takes parent element and active/inactive functions
            Screen();

            // Render calls Element::render() but then also handles rendering
            // a highlighted element (in order to get colour behind)
            // Is passed highlight colours (background, border, selected)
            void render(Colour, Colour, Colour);
    };
};

#endif