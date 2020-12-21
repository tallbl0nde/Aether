#ifndef AETHER_BASETEXT_HPP
#define AETHER_BASETEXT_HPP

#include "Aether/base/Texture.hpp"
#include <string>

namespace Aether {
    /**
     * @brief BaseText is an abstract class storing relevant information to render
     * a text element.
     */
    class BaseText : public Texture {
        protected:
            unsigned int fontSize_;     /** @brief Font size used for rendered text */
            std::string string_;        /** @brief String matching rendered string */

        public:
            /**
             * @brief Constructs a new BaseText object
             *
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param str String to render
             * @param size Font size (in pixels)
             */
            BaseText(const int x, const int y, const std::string & str, const unsigned int size);

            /**
             * @brief Get the rendered string
             *
             * @return string matching rendered string
             */
            std::string string();

            /**
             * @brief Set a new string. Will cause an immediate redraw.
             *
             * @param str New string to render
             */
            virtual void setString(const std::string & str);

            /**
             * @brief Get the font size of the rendered text
             *
             * @return Font size (in pixels)
             */
            unsigned int fontSize();

            /**
             * @brief Set the render font size for text
             *
             * @param size Render font size (in pixels)
             */
            virtual void setFontSize(const unsigned int size);
    };
};

#endif