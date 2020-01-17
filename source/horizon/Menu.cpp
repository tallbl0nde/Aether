#include "Menu.hpp"

namespace Aether {
    Menu::Menu(int x, int y, int w, int h) : Scrollable(x, y, w, h) {
        this->setShowScrollBar(false);
        this->setCatchup(15);
    }
};