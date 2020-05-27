#ifndef AETHER_ROUNDPROGRESSBAR_HPP
#define AETHER_ROUNDPROGRESSBAR_HPP

#include "Aether/base/BaseProgress.hpp"
#include "Aether/primary/Rectangle.hpp"

namespace Aether {
    /**
     * @brief A RoundProgressBar is a completely filled
     * progress bar rounded at either end.
     */
    class RoundProgressBar : public BaseProgress {
        private:
            /** @brief Progress bar background colour */
            Colour bgColour;
            /** @brief Progress bar background texture */
            Rectangle * backTex;
            /** @brief Highlighted progress bar colour */
            Colour fgColour;
            /** @brief Highlighted progress bar texture */
            Rectangle * progressTex;

            /**
             * @brief Rerender progressTex (on value change)
             */
            void redrawBar();

        public:
            /**
             * @brief Construct a new Round Progress Bar object
             * 
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param w width of progress bar
             * @param h height of progress bar(default value is 12)
             */
            RoundProgressBar(int x, int y, int w, int h = 12);

            /**
             * @brief Set the value of the progress bar.
             * When value is changed, the mask of the
             * rectangle will also be changed.
             * 
             * @param f New progress bar value
             */
            void setValue(float f);

            /**
             * @brief Set new width for the progress bar
             * Textures will be redrawn when dimensions are changed.
             * 
             * @param w new width of progress bar
             */
            void setW(int w);

            /**
             * @brief Set new height for the progress bar
             * Textures will be redrawn when dimensions are changed.
             * 
             * @param h new height of progress bar
             */
            void setH(int h);

            /**
             * @brief Get the background colour of the progress bar
             * 
             * @return background colour 
             */
            Colour getBackgroundColour();

            /**
             * @brief Set the background colour of the progress bar
             * 
             * @param c new background colour
             */
            void setBackgroundColour(Colour c);

            /**
             * @brief Get the foreground colour of the progress bar
             * 
             * @return foreground colour 
             */
            Colour getForegroundColour();

            /**
             * @brief Set the foreground colour of the progress bar
             * 
             * @param c new foreground colour
             */
            void setForegroundColour(Colour c);
    };
};

#endif