#ifndef AETHER_DISPLAY_HPP
#define AETHER_DISPLAY_HPP

#include "Overlay.hpp"
#include "Screen.hpp"

namespace Aether {
    // The Display represents the "root" element. It stores and handles different screens
    // and polling/passing events
    class Display : public Element {
        private:
            // Set to false to indicate termination of app
            bool loop_;

            // Colour to clear screen with
            Colour bg;

            // Colours to highlight elements with
            std::function<Colour(uint32_t)> hiAnim;

            // Variables to handle held buttons
            Button heldButton;
            int heldTime;

            // Vector of overlays, drawn from start -> end but only last one gets events!
            std::vector<Overlay *> overlays;

            // Vector of screens, should be populated before the main loop!
            std::vector<Screen *> screens;
            size_t screen;

            // These functions are private members of a display
            // as they are called by loop()
            using Element::addElement;
            using Element::handleEvent;
            using Element::update;
            using Element::render;

        public:
            // Constructor takes no parameters
            // Initializes SDL
            Display();

            // Set colour to clear screen with
            void setBackgroundColour(uint8_t, uint8_t, uint8_t);
            // Set colours to highlight elements with
            void setHighlightColours(Colour, Colour);
            void setHighlightAnimation(std::function<Colour(uint32_t)>);

            // Add an overlay
            void addOverlay(Overlay *);

            // Add a screen to the display
            void addScreen(Screen *);

            // Set screen to given pointer
            bool setScreen(Screen *);

            // Executes the main loop (events + rendering)
            // Returns false when the app should be terminated
            bool loop();

            // Sets loop to false and thus exit
            void exit();

            ~Display();
    };
};

#endif