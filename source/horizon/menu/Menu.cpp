#include "Aether/horizon/menu/Menu.hpp"
#include <algorithm>

namespace Aether {
    Menu::Menu(int x, int y, int w, int h) : Scrollable(x, y, w, h) {
        this->active = nullptr;
        this->setShowScrollBar(false);
        this->setCatchup(15);
    }

    void Menu::update(unsigned int dt) {
        Scrollable::update(dt);

        // Reposition based on highlighted element if not scrolling
        if (!this->isScrolling && !this->isTouched && !this->isTouch && this->maxScrollPos() != 0 && this->focused() != nullptr) {
            int sMid = this->y() + this->h()/2;
            int cMid = this->focused()->y() + (this->focused()->h()/2);
            this->setScrollPos(this->scrollPos() + (this->scrollCatchup * (cMid - sMid) * (dt/1000.0)));
        }
    }

    void Menu::setActiveOption(MenuOption * o) {
        // Deactivate activated option
        if (this->active != nullptr) {
            this->active->setActive(false);
        }

        // Search children for option and activate
        std::vector<Element *>::iterator it = std::find(this->children.begin(), this->children.end(), o);
        if (it != this->children.end()) {
            this->active = static_cast<MenuOption *>(*it);
            this->active->setActive(true);
        }
    }
};