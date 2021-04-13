#include "Aether/horizon/controls/ControlItem.hpp"
#include "Aether/utils/Utils.hpp"

// Font size for icon character in pixels.
static constexpr int iconFontSize = 25;

// Font size for label in pixels.
static constexpr int labelFontSize = 22;

// Padding around item.
static constexpr int padding = 20;

// Gap between icon and label.
static constexpr int textureGap = 13;

namespace Aether {
    ControlItem::ControlItem(const Button btn, const std::string & label) : Element() {
        // Create and add textures
        this->button_ = btn;
        this->icon = new Text(padding, 0, Utils::buttonToCharacter(btn), iconFontSize);
        this->label = new Text(0, 0, label, labelFontSize);
        this->addElement(this->icon);
        this->addElement(this->label);

        // Set size of this element
        this->setW(padding*2 + this->icon->w() + textureGap + this->label->w());
        this->setH(std::max(this->icon->h(), this->label->h()) + padding*2);

        // Situate children
        this->label->setX(this->icon->x() + this->icon->w() + textureGap);
        this->icon->setY(this->y() + (this->h() - this->icon->h())/2);
        this->label->setY(this->y() + (this->h() - this->label->h())/2);

        // Adding a handler automatically makes this item selectable and touchable
        this->onPress([btn](){
            // Send pushed event
            SDL_Event event;
            event.type = SDL_JOYBUTTONDOWN;
            event.jbutton.which = FAKE_ID;
            event.jbutton.button = (uint8_t)btn;
            event.jbutton.state = SDL_PRESSED;
            SDL_PushEvent(&event);

            // Send released event (so basically a verrry fast button press)
            SDL_Event event2;
            event2.type = SDL_JOYBUTTONUP;
            event2.jbutton.which = FAKE_ID;
            event2.jbutton.button = (uint8_t)btn;
            event2.jbutton.state = SDL_RELEASED;
            SDL_PushEvent(&event2);
        });
        this->setSelectable(false);
    }

    Button ControlItem::button() {
        return this->button_;
    }

    void ControlItem::setColour(const Colour & colour) {
        this->icon->setColour(colour);
        this->label->setColour(colour);
    }

    void ControlItem::setLabel(const std::string & label) {
        this->label->setString(label);
    }
};
