#ifndef AETHER_BASETEXT_HPP
#define AETHER_BASETEXT_HPP

#include "Texture.hpp"

namespace Aether {
    enum FontStyle {
        Regular,
        Bold,
        Italic,
        Underline,
        Strikethrough
    };

    // BaseText is an abstract class storing relevant information to render
    // a text element.
    class BaseText : public Texture {
        protected:
            // String matching rendered string
            std::string string_;
            // Font size used for rendered text
            unsigned int fontSize_;
            // Font style (stored for redrawing)
            std::atomic<FontStyle> fontStyle;

        public:
            // Constructor accepts string, font size and font type (defaulting to normal) and render type
            BaseText(int, int, std::string, unsigned int, FontStyle, RenderType);

            // Getters + setters for variables
            std::string string();
            virtual void setString(std::string);
            unsigned int fontSize();
            virtual void setFontSize(unsigned int);
    };
};

#endif