#ifndef AETHER_DISPLAY_HPP
#define AETHER_DISPLAY_HPP

#include "Screen.hpp"

namespace Aether {
    // The Display represents the "root" element. It stores and handles different screens
    // and polling/passing events
    class Display : public Element {
        private:
            // Set to false to indicate termination of app
            bool loop_;

            // Overlay stuff
            // HERE

            // Vector of screens, should be populated before the main loop!
            std::vector<Screen *> screens;
            size_t screen;

            // These functions are private members of a display
            // as they are called by loop()
            using Element::handleEvent;
            using Element::update;
            using Element::render;

        public:
            // Constructor takes no parameters
            // Initializes SDL
            Display();

            // Add a screen to the display
            void addScreen(Screen *);

            // Set screen to given pointer
            // bool setScreen(Screen *)

            // Executes the main loop (events + rendering)
            // Returns false when the app should be terminated
            bool loop(uint32_t);

            ~Display();
    };
};

#endif