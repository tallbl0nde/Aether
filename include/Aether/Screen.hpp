#ifndef AETHER_SCREEN_HPP
#define AETHER_SCREEN_HPP

#include "Aether/base/Container.hpp"
#include <unordered_map>

namespace Aether {
    /**
     * @brief A class that represents a screen layout
     * Stores all screen elements for a specific screen.
     */
    class Screen : public Container {
        private:
            /** @brief Mappings for button presses to handlers */
            std::unordered_map<Button, std::function<void()> > onButtonPressFuncs;

            /** @brief Mappings for button releases to handlers */
            std::unordered_map<Button, std::function<void()> > onButtonReleaseFuncs;

        public:
            /**
             * @brief Constructs a new (empty) Screen object.
             */
            Screen();

            /**
             * @brief Invoked once when the screen is requested to be 'shown'.
             * This method should set up all child elements.
             */
            virtual void onLoad();

            /**
             * @brief Invoked once when the screen has been replaced by another screen.
             * This method should tidy up all child elements.
             */
            virtual void onUnload();

            /**
             * @brief Assigns a function to invoke when a button is pressed.
             * @note Setting a handler will block the button press event from travelling down to children.
             *
             * @param btn Button to assign function to
             * @param func Function to invoke when button pressed
             */
            void onButtonPress(Button btn, std::function<void()> func);

            /**
             * @brief Assigns a function to invoke when a button is released.
             * @note Setting a handler will block the button release event from travelling down to children.
             *
             * @param btn Button to assign function to
             * @param func Function to invoke when button released
             */
            void onButtonRelease(Button btn, std::function<void()> func);

            /**
             * @brief Attempts to handle the event if it is a button press/release.
             *
             * @param event event to handle
             * @return Whether the event was handled by the screen.
             */
            bool handleEvent(InputEvent * event);
    };
};

#endif