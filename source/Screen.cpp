#include "Screen.hpp"

// Size of highlight border
#define HIGHLIGHT_SIZE 5

namespace Aether {
    Screen::Screen() : Container(0, 0, 1280, 720) {

    }

    void Screen::render(Colour bg, Colour hi, Colour sel) {
        // Render children
        Container::render();

        // If there is a highlighted element draw behind it and then render it
        Element * el = getHighlightedElement(this);
        if (el != nullptr) {
            el->renderHighlighted(bg, hi, sel, HIGHLIGHT_SIZE);
        }
    }
};