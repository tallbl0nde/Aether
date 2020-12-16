#ifndef AETHER_EVENT_HPP
#define AETHER_EVENT_HPP

#include <SDL2/SDL.h>
#include "Aether/utils/Types.hpp"

namespace Aether {
    /**
     * @brief Enum class that contains all events types supported by Aether for handling
     */
    enum EventType {
        ButtonPressed, /**< Button press event */
        ButtonReleased, /**< Button release event */
        TouchPressed, /**< Touch press event */
        TouchMoved, /**< Touch move event */
        TouchReleased /**< Touch release event */
    };

    /**
     * @brief A class that represents an Aether input event.
     *
     * This is basic stripped down version of SDL_Event only containing
     * information that will be handled by Aether.
     */
    class InputEvent {
        private:
            /** @brief Type of input event */
            EventType type_;
            /** @brief Button information for button event */
            Button button_;
            /** @brief ID of the event */
            int id_;
            /** @brief Horizontal touch position for touch events (Min = 0, Max = 1280) */
            int touchX_;
            /** @brief Vertical touch position for touch events (Min = 0, Max = 720) */
            int touchY_;
            /** @brief Change in horizonal touch position for touch move events (Min = 0, Max = 1280) */
            int touchDX_;
            /** @brief Change in vertical touch position for touch move events (Min = 0, Max = 720) */
            int touchDY_;

        public:
            /**
             * @brief Construct a new Input Event object from a SDL_Event
             *
             * @param event SDL_Event to make InputEvent from
             */
            InputEvent(SDL_Event event);

            /**
             * @brief Getter function to get event type
             *
             * @return EventType type for event that occured
             */
            EventType type();

            /**
             * @brief Getter function to get button information for event
             *
             * @return Button button information for event that occured
             */
            Button button();

            /**
             * @brief Getter function to get event id
             *
             * @return int event id of event
             */
            int id();

            /**
             * @brief Getter function to get the horizontal position for a touch event
             *
             * @return int horizontal position for a touch event
             */
            int touchX();

            /**
             * @brief Getter function to get the vertical position for a touch event
             *
             * @return int vertical position for a touch event
             */
            int touchY();

            /**
             * @brief Getter function to get the change of horizontal position for a touch move event
             *
             * @return int change in horizontal position
             */
            int touchDX();

            /**
             * @brief Getter function to get the change of vertical position for a touch move event
             *
             * @return int change in vertical position
             */
            int touchDY();

            /**
             * @brief Destroy the Input Event object
             */
            ~InputEvent();
    };
};

#endif