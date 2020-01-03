#include "BaseText.hpp"

namespace Aether {
    BaseText::BaseText(Element * e, std::string s, unsigned int f, FontType t) : Texture(e) {
        this->fontSize_ = f;
        this->fontType = t;
        this->string_ = s;
    }

    std::string BaseText::string() {
        return this->string_;
    }

    void BaseText::setString(std::string s) {
        this->string_ = s;
    }

    unsigned int BaseText::fontSize() {
        return this->fontSize_;
    }

    void BaseText::setFontSize(unsigned int s) {
        this->fontSize_ = s;
    }

    // Destructor destroys texture
    BaseText::~BaseText() {

    }
};