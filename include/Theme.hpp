#ifndef AETHER_THEME_HPP
#define AETHER_THEME_HPP

#include "Types.hpp"

// These are a set of colours provided that match what is used
// within Horizon.
namespace Aether::Theme {
    // Dark theme (Basic Black)
    const Theme_T Dark = {
        Colour{0, 255, 203, 255},   // accent
        Colour{49, 49, 49, 255},    // altBG
        Colour{45, 45, 45, 255},    // BG
        Colour{164, 164, 164, 255}, // FG
        Colour{35, 35, 40, 255},    // highlightBG
        Colour{100, 100, 100, 255}, // mutedLine
        Colour{160, 160, 160, 255}, // mutedText
        Colour{0, 250, 200, 50},    // selected
        Colour{255, 255, 255, 255}  // text
    };

    // Light theme (Basic White)
    const Theme_T Light = {
        Colour{50, 80, 240, 255},   // accent
        Colour{231, 231, 231, 255}, // altBG
        Colour{235, 235, 235, 255}, // BG
        Colour{45, 45, 45, 255},    // FG
        Colour{253, 253, 253, 255}, // highlightBG
        Colour{200, 200, 200, 255}, // mutedLine
        Colour{130, 130, 130, 255}, // mutedText
        Colour{0, 250, 200, 50},    // selected
        Colour{0, 0, 0, 255}        // text
    };
};

#endif