#ifndef AETHER_TEXT_HPP
#define AETHER_TEXT_HPP

#include "Aether/base/BaseText.hpp"

// Forward declare the timer class
namespace Aether {
    class Timer;
};

namespace Aether {
    /**
     * @brief Element for rendering a single line of text. It can optionally scroll when overflowing.
     */
    class Text : public BaseText {
        private:
            /**
             * @brief Collection of scrolling related variables.
             */
            struct ScrollVars {
                bool allowed;           /** @brief Can the text scroll if it's overflowing? */
                float position;         /** @brief Drawable x coordinate offset */
                int speed;              /** @brief Pixels to scroll per second */
                Timer * timer;          /** @brief Timer to track how long we've 'paused' for */
                int waitInterval;       /** @brief How long to wait once we reach the end */
            } scroll;

            /**
             * @brief Override in order to render a line of text.
             */
            Drawable * renderDrawable();

        public:
            /**
             * @brief Constructs a new Text element.
             *
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param str String to render
             * @param size Font size to render with
             * @param type Type of rendering to perform
             */
            Text(const int x, const int y, const std::string & str, const unsigned int size, const Render type = Render::Sync);

            /**
             * @brief Returns the dimensions of the given string if it were to be rendered, without
             * performing any slow rendering.
             *
             * @param str String to measure
             * @param size Font size to measure text with
             * @return Pair of dimensions, with the first value being
             * the width and second being the height.
             */
            static std::pair<int, int> getDimensions(const std::string & str, const unsigned int size);

            /**
             * @brief Returns whether the text is allowed to scroll when needed.
             *
             * @return If the text can scroll.
             */
            bool canScroll();

            /**
             * @brief Set whether the text will scroll when overflowing.
             *
             * @param scroll Whether the text can scroll
             */
            void setCanScroll(const bool scroll);

            /**
             * @brief Set how long the text should 'pause' for when it finishes scrolling.
             *
             * @param ms Time to pause (in ms)
             */
            void setScrollPause(const unsigned int ms);

            /**
             * @brief Set the scroll speed of the text.
             *
             * @param pps Pixels to scroll in one second.
             */
            void setScrollSpeed(const int pps);

            /**
             * @brief Set a new string. Will cause an immediate redraw.
             *
             * @param str New string to render
             */
            void setString(const std::string & str);

            /**
             * @brief Set the render font size for text
             *
             * @param size Render font size (in pixels)
             */
            void setFontSize(const unsigned int size);

            /**
             * @brief Called internally. Overrides Element's update in order to scroll
             * the text if needed.
             *
             * @param dt Delta time since last frame in ms
             */
            void update(unsigned int dt);

            /**
             * @brief Adjust the overall element's width. This will cause the text to
             * clip if it doesn't fit and scrolling isn't enabled.
             *
             * @param w New width
             */
            void setW(const int w);

            /**
             * @brief Destroys the Text element.
             */
            ~Text();
    };
};

#endif