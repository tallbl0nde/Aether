#include "Screen.hpp"

#include "Theme.hpp"

namespace Aether {
    Screen::Screen() : Container(0, 0, 1280, 720) {
        this->active_func = nullptr;
        this->inactive_func = nullptr;
    }

    void Screen::callActive() {

    }

    void Screen::callInactive() {

    }

    void Screen::render() {
        // Render children
        Container::render();

        // If there is a highlighted element draw behind it and then render it
        // Element * el = getHighlightedElement(this);
        // if (el != nullptr) {
        //     // Draw behind
        //     SDLHelper::drawRect(Theme::Dark.accent, el->x() - 5, el->y() - 5, el->w() + 10, el->h() + 10);
        //     SDLHelper::drawRect(Theme::Dark.highlightBG, el->x(), el->y(), el->w(), el->h());

        //     // Render element
        //     el->render();
        // }
    }
};