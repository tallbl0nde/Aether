#include "Aether/types/Colour.hpp"

namespace Aether {
    Colour::Colour() {
        this->setColour(255, 255, 255, 255);
    }

    Colour::Colour(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) {
        this->setColour(r, g, b, a);
    }

    unsigned char Colour::r() {
        return this->red;
    }

    unsigned char Colour::g() {
        return this->green;
    }

    unsigned char Colour::b() {
        return this->blue;
    }

    unsigned char Colour::a() {
        return this->alpha;
    }

    void setR(const unsigned char r) {
        this->red = r;
    }

    void setG(const unsigned char g) {
        this->green = g;
    }

    void setB(const unsigned char b) {
        this->blue = b;
    }

    void setA(const unsigned char a) {
        this->alpha = a;
    }

    void setColour(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) {
        this->red = r;
        this->green = g;
        this->blue = b;
        this->alpha = a;
    }
};