#ifndef AETHER_TYPES_HPP
#define AETHER_TYPES_HPP

#include <functional>
#include "SDLHelper.hpp"

namespace Aether {
    // Enums for buttons
    // Avoids confusion with SDL/libnx names
    enum Button {
        A,
        B,
        X,
        Y,
        LSTICK,
        RSTICK,
        L,
        R,
        ZL,
        ZR,
        PLUS,
        MINUS,
        DPAD_LEFT,
        DPAD_UP,
        DPAD_RIGHT,
        DPAD_DOWN,
        LSTICK_LEFT,
        LSTICK_UP,
        LSTICK_RIGHT,
        LSTICK_DOWN,
        RSTICK_LEFT,
        RSTICK_UP,
        RSTICK_RIGHT,
        RSTICK_DOWN,
        SL_LEFT,
        SR_LEFT,
        SL_RIGHT,
        SR_RIGHT,
        NO_BUTTON      // Dummy button used for symbolising no button
    };

    // Whether to render element or not (essentially just a bool)
    enum class RenderType {
        OnCreate,               // Render as part of construction (this is the same as OnCreateDeferred in threaded elements)
        OnCreateDeferred,       // Render in background as soon as constructed (this is the same as OnCreate for non-threaded elements)
                                // RECOMMENDED FOR STRINGS THAT DON'T CHANGE
        Deferred                // No rendering will be done until explicitly started
                                // In threaded elements: any update to values will not be rendered until calling startRendering();
                                // In non-threaded elements: any update to initial value will be rendered on main thread
                                // RECOMMENDED FOR STRINGS THAT CHANGE
    };

    // SDL_Color but it's not
    typedef SDL_Color Colour;

    // Theme (used for provided themes)
    typedef struct {
        Colour accent;          // "Selected" colour
        Colour altBG;           // Lighter/darker shade of bg
        Colour bg;              // Background
        Colour fg;              // Foreground (ie. lines)
        Colour highlightBG;     // Highlight background (usually darker/lighter than bg)
        Colour mutedLine;       // Usually grey; used for lines that separate
        Colour mutedText;       // Also grey; used for less important text
        Colour selected;        // Colour to draw on top of element when selected
        Colour text;            // Text colour

        std::function<Colour(uint32_t)> highlightFunc;      // Function returning a colour (for highlight border) based on current time
    } Theme_T;
};

#endif