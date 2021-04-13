#include "Aether/base/BaseText.hpp"

namespace Aether {
    BaseText::BaseText(const int x, const int y, const std::string & str, const unsigned int size) : Texture(x, y) {
        this->fontSize_ = size;
        this->string_ = str;
    }

    std::string BaseText::string() {
        return this->string_;
    }

    void BaseText::setString(const std::string & str) {
        if (str == this->string()) {
            return;
        }

        this->string_ = str;
        this->destroy();
        this->renderSync();
    }

    unsigned int BaseText::fontSize() {
        return this->fontSize_;
    }

    void BaseText::setFontSize(const unsigned int size) {
        if (size == this->fontSize_) {
            return;
        }

        this->fontSize_ = size;
        this->destroy();
        this->renderSync();
    }
};