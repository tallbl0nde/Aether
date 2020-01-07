#ifndef AETHER_BASETEXT_HPP
#define AETHER_BASETEXT_HPP

#include "Texture.hpp"

namespace Aether {
    enum FontType {
        Normal,     // Normal font
        Extended    // Icon/extended font
    };

    // BaseText is an abstract class storing relevant information to render
    // a text element.
    class BaseText : public Texture {
        protected:
            // String matching rendered string
            std::string string_;
            // Font size used for rendered text
            unsigned int fontSize_;
            // Type of font to use (stored for redrawing)
            FontType fontType;

        public:
            // Constructor accepts string, font size and font type (defaulting to normal)
            BaseText(std::string, unsigned int, FontType);

            // Getters + setters for variables
            std::string string();
            virtual void setString(std::string);
            unsigned int fontSize();
            virtual void setFontSize(unsigned int);
    };
};

#endif