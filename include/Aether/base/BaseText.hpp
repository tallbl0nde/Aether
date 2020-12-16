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
            /** @brief String matching rendered string */
            std::string string_;
            /** @brief Font size used for rendered text */
            unsigned int fontSize_;

        public:
            /**
             * @brief Construct a new Base Text object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param s string to render
             * @param f font size (in pixels)
             * @param t \ref ::RenderType to use for rendering
             */
            BaseText(int x, int y, std::string s, unsigned int f, RenderType t);

            /**
             * @brief Get the rendered string
             *
             * @return string matching rendered string
             */
            std::string string();

            /**
             * @brief Set new string to render
             *
             * @param s new string to render
             */
            virtual void setString(std::string s);

            /**
             * @brief Get the render font size for text
             *
             * @return render font size for text (in pixels)
             */
            unsigned int fontSize();

            /**
             * @brief Set the render font size for text
             *
             * @param s new render font size for text (in pixels)
             */
            virtual void setFontSize(unsigned int s);
    };
};

#endif