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
            /** @brief Mappings for button presses to callback functions */
            std::unordered_map<Button, std::function<void()> > pressFuncs;

            /** @brief Mappings for button releases to callback functions */
            std::unordered_map<Button, std::function<void()> > releaseFuncs;

        public:
            /**
             * @brief Construct a new Screen object
             */
            Screen();

            /**
             * @brief Callback function when the screen is loaded
             */
            virtual void onLoad();

            /**
             * @brief Callback function when the screen is unloaded
             */
            virtual void onUnload();

            /**
             * @brief Assigns callback function for button press
             * @note Setting a button callback will block the event from
             *       going to any other elements
             *
             * @param btn button to assign callback to
             * @param func function to assign as callback for button press
             */
            void onButtonPress(Button btn, std::function<void()> func);

            /**
             * @brief Assigns callback function for button release
             * @note Setting a button callback will block the event from
             *       going to any other elements
             *
             * @param btn button to assign callback to
             * @param func function to assign as callback for button release
             */
            void onButtonRelease(Button btn, std::function<void()> func);

            /**
             * @brief Attempt event handling for an event that occured
             *
             * @param event event to handle
             * @return true if event was handled
             * @return false if event was not handled
             */
            bool handleEvent(InputEvent *event);
    };
};

#endif