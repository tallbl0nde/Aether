#include "Aether/horizon/button/FilledButton.hpp"

// Corner radius of rectangle
#define CORNER_RAD 8
// Horizontal padding for text
#define PADDING 20

namespace Aether {
    FilledButton::FilledButton(int x, int y, int w, int h, std::string t, unsigned int s, std::function<void()> f) : Element(x, y, w, h) {
        this->rect = new Rectangle(this->x(), this->y(), this->w(), this->h(), CORNER_RAD);
        this->text = new Text(this->x() + this->w()/2, this->y() + this->h()/2, "", s);
        this->setString(t);
        this->text->setCanScroll(true);
        this->text->setScrollPause(1500);
        this->text->setScrollSpeed(40);
        this->addElement(this->rect);
        this->addElement(this->text);
        this->onPress(f);
    }

    Colour FilledButton::getFillColour() {
        return this->rect->colour();
    }

    void FilledButton::setFillColour(Colour c) {
        this->rect->setColour(c);
    }

    Colour FilledButton::getTextColour() {
        return this->text->colour();
    }

    void FilledButton::setTextColour(Colour c) {
        this->text->setColour(c);
    }

    std::string FilledButton::getString() {
        return this->text->string();
    }

    void FilledButton::setString(std::string s) {
        this->text->setString(s);
        if (this->text->textureWidth() > this->w() - 1.5*PADDING) {
            this->text->setW(this->w() - 2*PADDING);
        }
        this->text->setXY(this->x() + (this->w() - this->text->w())/2, this->y() + (this->h() - this->text->h())/2);
    }

    void FilledButton::setW(int w) {
        Element::setW(w);
        this->rect->setRectSize(this->w(), this->h());
        this->setString(this->text->string());
    }

    void FilledButton::setH(int h) {
        Element::setH(h);
        this->rect->setRectSize(this->w(), this->h());
        this->text->setY(this->y() + (this->h() - this->text->h())/2);
    }

    Drawable * FilledButton::renderHighlightBG() {
        return this->renderer->renderFilledRoundRectTexture(this->w(), this->h(), this->rect->cornerRadius() + 5);
    }

    Drawable * FilledButton::renderHighlight() {
        return this->renderer->renderRoundRectTexture(this->w() + 2*this->hiSize + 4, this->h() + 2*this->hiSize + 4, this->rect->cornerRadius() + 5, this->hiSize);
    }

    Drawable * FilledButton::renderSelection() {
        // There is no selected texture
        return new Drawable();
    }
};