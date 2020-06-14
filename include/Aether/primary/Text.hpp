#ifndef AETHER_TEXT_HPP
#define AETHER_TEXT_HPP

#include "Aether/base/BaseText.hpp"

namespace Aether {
    /**
     * @brief Text extends BaseText by implementing scrolling when the text overflows.
     *
     * It's for single-line text.
     */
    class Text : public BaseText {
        private:
            /** @brief Indicator on whether the text is scrollable */
            bool scroll_;
            /** @brief Pixels to scroll per second */
            int scrollSpeed_;
            /** @brief Time since scroll finished (in ms) (used internally) */
            int scrollPauseTime;
            /** @brief Time to wait after scrolling to the end (in ms) */
            unsigned int scrollWaitTime_;

            /** @brief Generate a text surface */
            void generateSurface();

        public:
            /**
             * @brief Construct a new Text object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param s text string
             * @param f font size
             * @param l font style
             * @param t \ref ::RenderType to use for texture generation
             */
            Text(int x, int y, std::string s, unsigned int f, FontStyle l = FontStyle::Regular, RenderType t = RenderType::OnCreate);

            /**
             * @brief Indicator on whether the text is scrollable
             *
             * @return true if it is scrollable
             * @return false otherwise
             */
            bool scroll();

            /**
             * @brief Set whether the text is scrollable
             *
             * @param s true if text is scrollable, false otherwise
             */
            void setScroll(bool s);

            /**
             * @brief Get the time to pause after scrolling to the end
             *
             * @return time to pause (in ms) after finished scrolling
             */
            int scrollWaitTime();

            /**
             * @brief Set the time to pause after scrolling to the end
             *
             * @param t time to pause (in ms)
             */
            void setScrollWaitTime(unsigned int t);

            /**
             * @brief Get the scroll speed for text
             *
             * @return scroll speed for text
             */
            int scrollSpeed();
            /**
             * @brief Set the scroll speed for text
             *
             * @param s new scroll speed for text
             */
            void setScrollSpeed(int s);

            /**
             * @brief Set the font size for the text
             *
             * @param f new font size
             */
            void setFontSize(unsigned int f);

            /**
             * @brief Set text
             *
             * @param s new text to set
             */
            void setString(std::string s);

            /**
             * @brief Updates the text.
             *
             * Update handles animating the scroll if necessary.
             *
             * @param dt change in time
             */
            void update(uint32_t dt);

            /**
             * @brief Adjusts the text width.
             *
             * Adjusting width may need to adjust amount of text shown.
             *
             * @param w new width
             */
            void setW(int w);
    };
};

#endif