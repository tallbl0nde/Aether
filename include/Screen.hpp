#ifndef AETHER_SCREEN_HPP
#define AETHER_SCREEN_HPP

#include "base/Container.hpp"

namespace Aether {
    // A screen represents a literal screen/layout. It stores all elements
    // to be drawn/interacted with on a single screen.
    class Screen : public Container {
        private:
            // Map of key > func for custom callbacks on button presses
            std::unordered_map<Key, std::function<void()> > callbacks;

        public:
            // Constructor takes parent element and active/inactive functions
            Screen();

            // Set a the given callback to the given key press
            // Note that setting a key callback will block the event from
            // going to any other elements!!
            void setKeyCallback(Key, std::function<void()>);

            // Check if callback is set and execute, otherwise pass event
            // to elements
            bool handleEvent(InputEvent *);

            // Render calls Element::render() but then also handles rendering
            // a highlighted element (in order to get colour behind)
            // Is passed highlight colours (background, border, selected)
            void render(Colour, Colour, Colour);
    };
};

#endif