#ifndef AETHER_TYPES_HPP
#define AETHER_TYPES_HPP

#include "Aether/types/Colour.hpp"
#include <functional>

namespace Aether {
    /**
     * @brief Enum class for buttons
     * Avoids confusion with SDL/libnx names
     */
    enum Button {
        A,              /**< Button A */
        B,              /**< Button B */
        X,              /**< Button X */
        Y,              /**< Button Y */
        LSTICK,         /**< Button Left Stick Click */
        RSTICK,         /**< Button Right Stick Click */
        L,              /**< Button L */
        R,              /**< Button R */
        ZL,             /**< Button ZL */
        ZR,             /**< Button ZR */
        PLUS,           /**< Button + */
        MINUS,          /**< Button - */
        DPAD_LEFT,      /**< Button D-Pad Left */
        DPAD_UP,        /**< Button D-Pad Up */
        DPAD_RIGHT,     /**< Button D-Pad Right */
        DPAD_DOWN,      /**< Button D-Pad Down */
        LSTICK_LEFT,    /**< Button Left Stick Left */
        LSTICK_UP,      /**< Button Left Stick Up */
        LSTICK_RIGHT,   /**< Button Left Stick Right */
        LSTICK_DOWN,    /**< Button Left Stick Down */
        RSTICK_LEFT,    /**< Button Right Stick Left */
        RSTICK_UP,      /**< Button Right Stick Up */
        RSTICK_RIGHT,   /**< Button Right Stick Right */
        RSTICK_DOWN,    /**< Button Right Stick Down */
        SL_LEFT,        /**< Left Joycon SL Button */
        SR_LEFT,        /**< Left Joycon SR Button */
        SL_RIGHT,       /**< Right Joycon SL Button */
        SR_RIGHT,       /**< Right Joycon SR Button */
        NO_BUTTON       /**< Dummy button used for symbolising no button */
    };

    /**
     * @brief Enum class for amount of padding in a scrollable
     */
    enum class Padding {
        Default,            /**< Default amount of padding (~5% of list width either side of item) */
        FitScrollbar,       /**< No padding except to fit the scrollbar */
        None           /**< No padding whatsoever (item width = list width) */
    };

    /**
     * @brief Struct for UI Theme
     */
    typedef struct {
        /** @brief "Selected" colour */
        Colour accent;
        /** @brief Lighter/darker shade of bg */
        Colour altBG;
        /** @brief Background */
        Colour bg;
        /** @brief Foreground (ie. lines) */
        Colour fg;
        /** @brief Highlight background (usually darker/lighter than bg) */
        Colour highlightBG;
        /** @brief Usually grey; used for lines that separate */
        Colour mutedLine;
        /** @brief Also grey; used for less important text */
        Colour mutedText;
        /** @brief Colour to draw on top of element when selected */
        Colour selected;
        /** @brief Text colour */
        Colour text;
        /** @brief Function returning a colour (for highlight border) based on current time */
        std::function<Colour(uint32_t)> highlightFunc;
    } Theme_T;
};

#endif