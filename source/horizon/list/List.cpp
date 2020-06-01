#include "Aether/horizon/list/List.hpp"
#include "Aether/utils/Utils.hpp"

// Same as Scrollable.hpp
#define PADDING 40

namespace Aether {
    List::List(int x, int y, int w, int h, Padding p) : Scrollable(x, y, w, h, p) {
        this->setShowScrollBar(true);
        this->setCatchup(13.5);
        this->heldButton = Button::NO_BUTTON;
        this->scroll = false;
        this->wrapAround_ = false;
    }

    bool List::wrapAround() {
        return this->wrapAround_;
    }

    void List::setWrapAround(bool b) {
        this->wrapAround_ = b;
    }

    bool List::handleEvent(InputEvent * e) {
        // Store result of event
        bool res = Scrollable::handleEvent(e);

        // If button event isn't handled...
        if (!res && this->canScroll_) {
            if (e->type() == EventType::ButtonPressed) {
                if (e->button() == Button::DPAD_DOWN) {
                    // Wrap around if needed
                    if (this->wrapAround_ && e->id() != FAKE_ID) {
                        for (size_t i = 0; i < this->children.size(); i++) {
                            if (this->children[i]->selectable()) {
                                this->setFocused(this->children[i]);
                                break;
                            }
                        }
                        this->setScrollPos(0);
                        return true;

                    // Scroll down until at the bottom
                    } else if (this->scrollPos() < this->maxScrollPos()) {
                        this->heldButton = e->button();
                        this->scroll = true;
                        return true;
                    }
                }

                if (e->button() == Button::DPAD_UP) {
                    // Scroll up until at the top
                    if (this->wrapAround_ && e->id() != FAKE_ID) {
                        for (size_t i = this->children.size(); i > 0; i--) {
                            if (this->children[i-1]->selectable()) {
                                this->setFocused(this->children[i-1]);
                                break;
                            }
                        }
                        this->setScrollPos(this->maxScrollPos());
                        return true;

                    // Otherwise wrap around if needed
                    } else if (this->scrollPos() > 0) {
                        this->heldButton = e->button();
                        this->scroll = true;
                        return true;
                    }
                }

            } else if (e->type() == EventType::ButtonReleased) {
                // Remove held button on release
                if (e->button() == this->heldButton && e->id() != FAKE_ID) {
                    this->heldButton = Button::NO_BUTTON;
                    return true;
                }

            // Stop scrolling if not a relevant event
            } else {
                this->scroll = false;
            }
        }

        return res;
    }

    void List::update(uint32_t dt) {
        Scrollable::update(dt);

        // Allow "manual" scrolling at top and bottom
        if (this->scroll) {
            if (this->heldButton == Button::DPAD_DOWN) {
                this->setScrollPos(this->scrollPos() + (500 * (dt/1000.0)));
                return;
            } else if (this->heldButton == Button::DPAD_UP) {
                this->setScrollPos(this->scrollPos() - (500 * (dt/1000.0)));
                return;
            }
        }

        // If focused element is not completely inside list scroll to it
        if (!this->isScrolling && !this->isTouched && !this->isTouch && this->maxScrollPos() != 0 && this->focused() != nullptr) {
            // Check if above
            if (this->focused()->y() < this->y() + PADDING) {
                this->setScrollPos(this->scrollPos() + (this->scrollCatchup * (this->focused()->y() - (this->y() + PADDING)) * (dt/1000.0)));

            // And below ;)
            } else if (this->focused()->y() + this->focused()->h() > this->y() + this->h() - (PADDING*2)) {
                this->setScrollPos(this->scrollPos() - (this->scrollCatchup * ((this->y() + this->h() - (PADDING*2)) - (this->focused()->y() + this->focused()->h())) * (dt/1000.0)));
            }
        }
    }
};