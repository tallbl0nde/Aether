#include "Aether/horizon/controls/ControlBar.hpp"
#include "Aether/horizon/controls/ControlItem.hpp"
#include <algorithm>

namespace Aether {
    ControlBar::ControlBar() : Container(45, 647, 1190, 73) {
        this->disabledColour = Aether::Colour{255, 255, 255, 150};
        this->enabledColour = Aether::Colour{255, 255, 255, 255};
    }

    ControlItem * ControlBar::getControl(const Button btn) {
        // Search for matching button in children
        std::vector<Element *>::iterator it = std::find_if(this->children.begin(), this->children.end(), [btn](Element * e) {
            return static_cast<ControlItem *>(e)->button() == btn;
        });

        // Return nullptr if past the end
        return (it == this->children.end() ? nullptr : static_cast<ControlItem *>(*it));
    }

    void ControlBar::repositionItems() {
        // Iterate over children and position with first element being right-most
        int nextX = this->x() + this->w();
        for (Element * item : this->children) {
            item->setX(nextX - item->w());
            nextX = item->x();
            item->setY(this->y() + (this->h() - item->h())/2);
        }
    }

    void ControlBar::addControl(const Button btn, const std::string & label) {
        // Don't permit adding 'NO_BUTTON'
        if (btn == Button::NO_BUTTON) {
            return;
        }

        // Stop if already added
        ControlItem * item = this->getControl(btn);
        if (item != nullptr) {
            return;
        }

        // Add as child
        item = new ControlItem(btn, label);
        item->setColour(this->enabledColour);
        this->addElement(item);
        this->repositionItems();
    }

    void ControlBar::disableControl(const Button btn) {
        ControlItem * item = this->getControl(btn);
        if (item != nullptr) {
            item->setColour(this->disabledColour);
            item->setTouchable(false);
        }
    }

    void ControlBar::enableControl(const Button btn) {
        ControlItem * item = this->getControl(btn);
        if (item != nullptr) {
            item->setColour(this->enabledColour);
            item->setTouchable(true);
        }
    }

    void ControlBar::moveControl(const Button btn, const Button before) {
        // Make sure we're not trying to move the same one
        if (btn == before) {
            return;
        }

        // Find button to move
        ControlItem * item = this->getControl(btn);
        if (item == nullptr) {
            return;
        }

        // Find button to move before
        size_t beforeIdx = -1;
        if (before != Button::NO_BUTTON) {
            std::vector<Element *>::iterator it2 = std::find_if(this->children.begin(), this->children.end(), [before](Element * e) {
                return static_cast<ControlItem *>(e)->button() == before;
            });

            if (it2 == this->children.end()) {
                return;
            }
            beforeIdx = std::distance(this->children.begin(), it2);
        }

        // Move element
        this->returnElement(item);
        this->addElementAt(item, beforeIdx+1);
        this->repositionItems();
    }

    void ControlBar::updateControl(const Button btn, const std::string & label) {
        ControlItem * item = this->getControl(btn);
        if (item != nullptr) {
            item->setLabel(label);
            this->repositionItems();
        }
    }

    void ControlBar::removeAllControls() {
        this->removeAllElements();
    }

    bool ControlBar::removeControl(const Button btn) {
        ControlItem * item = this->getControl(btn);
        bool removed = this->removeElement(item);

        if (removed) {
            this->repositionItems();
        }

        return removed;
    }

    void ControlBar::setDisabledColour(const Colour colour) {
        this->disabledColour = colour;
        for (Element * e : this->children) {
            if (!e->touchable()) {
                static_cast<ControlItem *>(e)->setColour(this->disabledColour);
            }
        }
    }

    void ControlBar::setEnabledColour(const Colour colour) {
        this->enabledColour = colour;
        for (Element * e : this->children) {
            if (e->touchable()) {
                static_cast<ControlItem *>(e)->setColour(this->enabledColour);
            }
        }
    }

    void ControlBar::setW(int w) {
        Container::setW(w);
        this->repositionItems();
    }

    void ControlBar::setH(int h) {
        Container::setH(h);
        this->repositionItems();
    }
};
