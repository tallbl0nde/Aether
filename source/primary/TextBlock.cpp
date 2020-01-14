#include "TextBlock.hpp"

namespace Aether {
    TextBlock::TextBlock(std::string s, unsigned int f, unsigned int w, FontType t) : BaseText(s, f, t) {
        this->setWrapWidth(w);
    }

    void TextBlock::redrawTexture() {
        this->setTexture(SDLHelper::renderTextWrapped(this->string_.c_str(), this->fontSize_, this->wrapWidth_, (this->fontType == FontType::Extended) ? true : false));
    }

    unsigned int TextBlock::wrapWidth() {
        return this->wrapWidth_;
    }

    void TextBlock::setWrapWidth(unsigned int w) {
        this->wrapWidth_ = w;
        this->redrawTexture();
    }
};