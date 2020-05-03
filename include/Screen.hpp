#ifndef AETHER_SCREEN_HPP
#define AETHER_SCREEN_HPP

#include "base/Container.hpp"

namespace Aether {
    /**
     * @brief A class that represents a screen layout
     * Stores all element for for a specific screen.
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
             * @param btn Button to assign callback to
             * @param func Function to assign as callback for button press
             */
            void onButtonPress(Button btn, std::function<void()> func);

            /**
             * @brief Assigns callback function for button release
             * @note Setting a button callback will block the event from
             *       going to any other elements
             * 
             * @param btn Button to assign callback to
             * @param func Function to assign as callback for button release
             */
            void onButtonRelease(Button btn, std::function<void()> func);

            /**
             * @brief Check for callback and execute if there is one
             * 
             * @param event Event to handle
             * @return true if callback for button was present and executed
             * @return false if no callback was set for button
             */
            bool handleEvent(InputEvent *event);
    };
};

#endif