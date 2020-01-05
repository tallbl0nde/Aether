#ifndef AETHER_TYPES_HPP
#define AETHER_TYPES_HPP

namespace Aether {
    // SDL_Color but it's not
    typedef SDL_Color Colour, Color;

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
    } Theme_T;
};

#endif