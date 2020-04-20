#include "BaseText.hpp"

namespace Aether {
    BaseText::BaseText(int x, int y, std::string s, unsigned int f, FontStyle l) : Texture(x, y) {
        this->fontSize_ = f;
        this->fontStyle = l;
        this->string_ = s;
    }

    std::string BaseText::string() {
        return this->string_;
    }

    void BaseText::setString(std::string s) {
        this->string_ = s;
        this->redrawTexture();
    }

    unsigned int BaseText::fontSize() {
        return this->fontSize_;
    }

    void BaseText::setFontSize(unsigned int s) {
        this->fontSize_ = s;
    }
};