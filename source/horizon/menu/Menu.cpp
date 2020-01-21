#include "Menu.hpp"

namespace Aether {
    Menu::Menu(int x, int y, int w, int h) : Scrollable(x, y, w, h) {
        this->setShowScrollBar(false);
        this->setCatchup(15);
    }

    void Menu::update(uint32_t dt) {
        Scrollable::update(dt);

        // Reposition based on highlighted element if not scrolling
        if (!this->isScrolling && !this->isTouched && !this->isTouch && this->maxScrollPos != 0 && this->focussed() != nullptr) {
            int sMid = this->y() + this->h()/2;
            int cMid = this->focussed()->y() + (this->focussed()->h()/2);
            this->setScrollPos(this->scrollPos + (this->scrollCatchup * (cMid - sMid) * (dt/1000.0)));
        }
    }
};