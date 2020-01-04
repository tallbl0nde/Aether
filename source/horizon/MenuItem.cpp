#include "MenuItem.hpp"

// Default dimensions
#define DEFAULT_WIDTH 300
#define DEFAULT_HEIGHT 70
#define DEFAULT_FONT_SIZE 22

namespace Aether {
    MenuItem::MenuItem(Element * e, std::string s, std::function<void()> f) : Element(e) {
        // Create children
        this->rect = new Rectangle(this, 4, 52);
        this->addElement(this->rect);
        this->text = new Text(this, s, DEFAULT_FONT_SIZE);
        this->text->setScroll(true);
        this->addElement(this->text);

        // Position children
        this->setXYWH(this->x(), this->y(), DEFAULT_WIDTH, DEFAULT_HEIGHT);
        if (this->text->w() > this->w() - this->text->x()) {
            this->text->setW(this->w() - this->text->x());
        }

        this->setCallback(f);
        this->setSelectable(true);
    }

    void MenuItem::setX(int x) {
        Element::setX(x);
        this->rect->setX(this->x() + 8);
        this->text->setX(this->x() + 24);
    }

    void MenuItem::setY(int y) {
        Element::setY(y);
        this->rect->setY(this->y() + 9);
        this->text->setY(this->y() + this->h()/2 - this->text->texH()/2);
    }

    void MenuItem::setXY(int x, int y) {
        this->setX(x);
        this->setY(y);
    }

    void MenuItem::setXYWH(int x, int y, int w, int h) {
        this->setXY(x, y);
        this->setW(w);
        this->setH(h);
    }

    MenuItem::~MenuItem() {
        // Note children aren't deleted as they are deleted
        // by the Element destructor
    }
};