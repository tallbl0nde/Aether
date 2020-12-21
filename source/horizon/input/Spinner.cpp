#include "Aether/horizon/input/Spinner.hpp"

// Default size + dimensions
#define ARROW_FONT_SIZE 30
#define ARROW_PADDING_X 30
#define ARROW_PADDING_Y 15
#define HOLD_DELAY 400      // ms to wait before repeating when held
#define REPEAT_DELAY 100    // repeat input every ms
#define LABEL_FONT_SIZE 18
#define LABEL_PADDING 30
#define VALUE_FONT_SIZE 40
#define VALUE_PADDING 20
#define HEIGHT 200  // Height without label

namespace Aether {
    Spinner::Spinner(int x, int y, int w) : Container(x, y, w, HEIGHT) {
        // Up arrow
        this->upContainer = new Element(0, this->y(), ARROW_FONT_SIZE + (2 * ARROW_PADDING_X), ARROW_FONT_SIZE + ARROW_PADDING_Y);
        this->upContainer->setX(this->x() + (this->w() - this->upContainer->w())/2);
        this->upContainer->setSelectable(false);
        this->upContainer->setTouchable(true);
        this->up = new Text(0, this->y(), "\uE147", ARROW_FONT_SIZE);
        this->up->setXY(this->upContainer->x() + (this->upContainer->w() - this->up->w())/2, this->upContainer->y() + (this->upContainer->h() - this->up->h())/2);
        this->upContainer->addElement(this->up);
        this->addElement(this->upContainer);

        // Down arrow
        this->downContainer = new Element(0, this->y() + this->h(), ARROW_FONT_SIZE + (2 * ARROW_PADDING_X), ARROW_FONT_SIZE + ARROW_PADDING_Y);
        this->downContainer->setX(this->x() + (this->w() - this->downContainer->w())/2);
        this->downContainer->setY(this->downContainer->y() - this->downContainer->h());
        this->downContainer->setSelectable(false);
        this->downContainer->setTouchable(true);
        this->down = new Text(0, 0, "\uE148", ARROW_FONT_SIZE);
        this->down->setXY(this->downContainer->x() + (this->downContainer->w() - this->down->w())/2, this->downContainer->y() + (this->downContainer->h() - this->down->h())/2);
        this->downContainer->addElement(this->down);
        this->addElement(this->downContainer);

        // Value string
        Element * e = new Element(0, 0, this->w(), VALUE_FONT_SIZE + VALUE_PADDING * 2);
        e->setXY(this->x() + (this->w() - e->w())/2, this->y() + (this->h() - e->h())/2);
        e->setSelectable(true);
        this->str = new Text(0, 0, "0", VALUE_FONT_SIZE);
        this->str->setXY(e->x() + (e->w() - this->str->w())/2, e->y() + (e->h() - this->str->h())/2);
        e->addElement(this->str);
        this->addElement(e);
        this->setFocused(e);

        // Label string (set hidden)
        this->label_ = new Text(this->x() + this->w()/2, this->y() + this->h() + LABEL_PADDING, "", LABEL_FONT_SIZE);
        this->label_->setHidden(true);
        this->addElement(this->label_);

        this->held = SpinnerHoldAction::NoHeld;
        this->holdTime = 0;
        this->wrap = false;
        this->padding = 0;
        this->value_ = 0;
        this->amount = 1;
        this->min_ = 0;
        this->max_ = 0;
        this->arrowC = Colour{255, 255, 255, 255};
        this->highlightC = Colour{255, 255, 255, 255};
        this->textC = Colour{255, 255, 255, 255};
    }

    void Spinner::incrementVal() {
        // Wrap around if enabled
        if (this->wrap && (this->value_ == this->max_)) {
            this->value_ = this->min_;
        } else {
            // Increase by set amount
            this->value_ += this->amount;
            if (this->value_ > this->max_) {
                this->value_ = this->max_;
            }
        }

        // Update string
        this->setValue(this->value_);
    }

    void Spinner::decrementVal() {
        // Wrap around if enabled
        if (this->wrap && (this->value_ == this->min_)) {
            this->value_ = this->max_;
        } else {
            // Decrease by set amount
            this->value_ -= this->amount;
            if (this->value_ < this->min_) {
                this->value_ = this->min_;
            }
        }

        // Update string
        this->setValue(this->value_);
    }

    bool Spinner::handleEvent(InputEvent * e) {
        bool b = false;

        // Inc/dec on dpad press
        if (e->type() == EventType::ButtonPressed) {
            switch (e->button()) {
                case Button::DPAD_UP:
                    this->incrementVal();
                    return true;
                    break;

                case Button::DPAD_DOWN:
                    this->decrementVal();
                    return true;
                    break;

                default:
                    break;
            }

        // Inc/dec on touch press, not release
        } else if (e->type() == EventType::TouchPressed) {
            // Don't return as the event still needs to passed to the children
            if ((e->touchX() >= this->upContainer->x() && e->touchX() < this->upContainer->x() + this->upContainer->w())
            && (e->touchY() >= this->upContainer->y() && e->touchY() < this->upContainer->y() + this->upContainer->h())) {
                this->incrementVal();
                b = true;
            } else if ((e->touchX() >= this->downContainer->x() && e->touchX() < this->downContainer->x() + this->downContainer->w())
              && (e->touchY() >= this->downContainer->y() && e->touchY() < this->downContainer->y() + this->downContainer->h())) {
                this->decrementVal();
                b = true;
            }
        }

        // Set this element focused if arrows touched (they're the only children that can handle events)
        if (Container::handleEvent(e)) {
            this->parent()->setFocused(this);
            b = true;
        }

        return b;
    }

    void Spinner::update(unsigned int dt) {
        Container::update(dt);

        // Set colours
        if (this->isFocused && !this->isTouch) {
            this->label_->setColour(this->highlightC);
            this->up->setColour(this->highlightC);
            this->down->setColour(this->highlightC);
            this->str->setColour(this->highlightC);
        } else {
            this->label_->setColour(this->arrowC);
            this->up->setColour(this->arrowC);
            this->down->setColour(this->arrowC);
            this->str->setColour(this->textC);
        }

        // Check if containers are held
        if (this->held == SpinnerHoldAction::NoHeld) {
            if (this->upContainer->selected()) {
                this->held = SpinnerHoldAction::UpHeld;
                this->holdTime = 0;
            } else if (this->downContainer->selected()) {
                this->held = SpinnerHoldAction::DownHeld;
                this->holdTime = 0;
            }

        // Otherwise handle repeated input
        } else {
            this->holdTime += dt;
            // Still within HOLD_DELAY
            if (this->held == SpinnerHoldAction::UpHeld || this->held == SpinnerHoldAction::DownHeld) {
                if (this->holdTime >= HOLD_DELAY) {
                    // Now change to tepeat mode
                    this->held = (this->held == SpinnerHoldAction::UpHeld ? SpinnerHoldAction::UpHeldRepeat : SpinnerHoldAction::DownHeldRepeat);
                    this->holdTime = 0;
                }

            // Now in REPEAT_DELAY
            } else {
                if (this->holdTime >= REPEAT_DELAY) {
                    if (this->held == SpinnerHoldAction::UpHeldRepeat) {
                        this->incrementVal();
                    } else {
                        this->decrementVal();
                    }
                    this->holdTime = 0;
                }
            }

            // Stop repeating when let go
            if ((this->held == SpinnerHoldAction::UpHeldRepeat && !this->upContainer->selected()) || (this->held == SpinnerHoldAction::DownHeldRepeat && !this->downContainer->selected())) {
                this->held = SpinnerHoldAction::NoHeld;
            }
        }
    }

    void Spinner::setActive() {
        this->isFocused = true;
        Container::setActive();
    }

    void Spinner::setInactive() {
        this->isFocused = false;
        Container::setInactive();
    }

    bool Spinner::wrapAround() {
        return this->wrap;
    }

    void Spinner::setWrapAround(bool b) {
        this->wrap = b;
    }

    unsigned int Spinner::digits() {
        return this->padding;
    }

    void Spinner::setDigits(unsigned int p) {
        this->padding = p;
        this->setValue(this->value_);
    }

    void Spinner::setLabel(std::string s) {
        this->label_->setString(s);

        // Remove if empty
        if (s == "") {
            this->label_->setHidden(true);
            this->setH(HEIGHT);
            return;
        }

        // Otherwise show and adjust height
        this->label_->setHidden(false);
        this->label_->setX(this->x() + (this->w() - this->label_->w())/2);
        this->setH(this->h() + LABEL_PADDING + this->label_->h());
    }

    std::string Spinner::label() {
        return this->label_->string();
    }

    int Spinner::changeAmount() {
        return this->amount;
    }

    void Spinner::setChangeAmount(int a) {
        this->amount = a;
    }

    int Spinner::value() {
        return this->value_;
    }

    void Spinner::setValue(int v) {
        this->value_ = v;
        std::string tmp = std::to_string(this->value_);
        while (tmp.length() < this->padding) {
            tmp = "0" + tmp;
        }
        this->str->setString(tmp);
        this->str->setX(this->x() + (this->w() - this->str->w())/2);
    }

    int Spinner::min() {
        return this->min_;
    }

    void Spinner::setMin(int m) {
        this->min_ = m;
        if (this->value_ < m) {
            this->setValue(m);
        }
    }

    int Spinner::max() {
        return this->max_;
    }

    void Spinner::setMax(int m) {
        this->max_ = m;
        if (this->value_ > m) {
            this->setValue(m);
        }
    }

    Colour Spinner::getArrowColour() {
        return this->arrowC;
    }

    void Spinner::setArrowColour(Colour c) {
        this->arrowC = c;
    }

    Colour Spinner::getHighlightColour() {
        return this->highlightC;
    }

    void Spinner::setHighlightColour(Colour c) {
        this->highlightC = c;
    }

    Colour Spinner::getTextColour() {
        return this->textC;
    }

    void Spinner::setTextColour(Colour c) {
        this->textC = c;
    }
}