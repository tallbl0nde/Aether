#include "Aether/primary/TextBlock.hpp"

namespace Aether {
    TextBlock::TextBlock(const int x, const int y, const std::string & str, const unsigned int size, const unsigned int wrap, const Render type) : BaseText(x, y, str, size) {
        this->wrapWidth_ = wrap;

        // Render based on requested type
        if (type == Render::Sync) {
            this->renderSync();

        } else if (type == Render::Async) {
            this->renderAsync();
        }
    }

    std::pair<int, int> TextBlock::getDimensions(const std::string & str, const unsigned int size, const unsigned int width) {
        std::tuple<std::vector<std::string>, int, int> dims = TextBlock::renderer->calculateWrappedTextDimensions(str, size, width);
        return std::pair<int, int>(std::get<1>(dims), std::get<2>(dims));
    }

    Drawable * TextBlock::renderDrawable() {
        return this->renderer->renderWrappedTextSurface(this->string_, this->fontSize_, this->wrapWidth_);
    }

    unsigned int TextBlock::wrapWidth() {
        return this->wrapWidth_;
    }

    void TextBlock::setWrapWidth(const unsigned int wrap) {
        if (wrap == this->wrapWidth_) {
            return;
        }

        this->wrapWidth_ = wrap;
        this->destroy();
        this->renderSync();
    }
};