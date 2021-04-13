#include "Aether/types/Colour.hpp"

namespace Aether {
    Colour::Colour() {
        this->setColour(255, 255, 255, 255);
    }

    Colour::Colour(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) {
        this->setColour(r, g, b, a);
    }

    unsigned char Colour::r() const {
        return this->red;
    }

    unsigned char Colour::g() const {
        return this->green;
    }

    unsigned char Colour::b() const {
        return this->blue;
    }

    unsigned char Colour::a() const {
        return this->alpha;
    }

    void Colour::setR(const unsigned char r) {
        this->red = r;
    }

    void Colour::setG(const unsigned char g) {
        this->green = g;
    }

    void Colour::setB(const unsigned char b) {
        this->blue = b;
    }

    void Colour::setA(const unsigned char a) {
        this->alpha = a;
    }

    void Colour::setColour(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) {
        this->red = r;
        this->green = g;
        this->blue = b;
        this->alpha = a;
    }
};