#include <algorithm>
#include "Aether/horizon/controls/ControlItem.hpp"
#include "Aether/utils/Utils.hpp"

// Font sizes
#define HINT_FONT_SIZE 22
#define ICON_FONT_SIZE 25

// "Padding" around actual textures
#define PADDING 20
// Gap between icon + hint
#define TEXT_GAP 13

namespace Aether {
    ControlItem::ControlItem(Button k, std::string s) : Element() {
        // Create and add elements
        this->icon = new Text(PADDING, 0, Utils::buttonToCharacter(k), ICON_FONT_SIZE);
        this->hint = new Text(0, 0, s, HINT_FONT_SIZE);
        this->addElement(this->icon);
        this->addElement(this->hint);

        // Set size of this element
        this->setW(PADDING*2 + this->icon->w() + TEXT_GAP + this->hint->w());
        this->setH(std::max(this->icon->h(), this->hint->h()) + PADDING*2);

        // Situate children
        this->hint->setX(this->icon->x() + this->icon->w() + TEXT_GAP);
        this->icon->setY(this->y() + (this->h() - this->icon->h())/2);
        this->hint->setY(this->y() + (this->h() - this->hint->h())/2);

        // Adding a callback automatically makes this item selectable and touchable
        this->setCallback([k](){
            // Send pushed event
            SDL_Event event;
            event.type = SDL_JOYBUTTONDOWN;
            event.jbutton.which = FAKE_ID;
            event.jbutton.button = (uint8_t)k;
            event.jbutton.state = SDL_PRESSED;
            SDL_PushEvent(&event);
            // Send released event (so basically a verrry fast button press)
            SDL_Event event2;
            event2.type = SDL_JOYBUTTONUP;
            event2.jbutton.which = FAKE_ID;
            event2.jbutton.button = (uint8_t)k;
            event2.jbutton.state = SDL_RELEASED;
            SDL_PushEvent(&event2);
        });
        this->setSelectable(false);
    }

    Colour ControlItem::colour() {
        return this->colour_;
    }

    void ControlItem::setColour(Colour c) {
        this->colour_ = c;
        this->icon->setColour(c);
        this->hint->setColour(c);
    }
};