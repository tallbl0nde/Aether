#ifndef AETHER_PROGRESSBAR_HPP
#define AETHER_PROGRESSBAR_HPP

#include "Aether/base/BaseProgress.hpp"
#include "Aether/primary/Box.hpp"
#include "Aether/primary/Rectangle.hpp"

namespace Aether {
    /**
     * @brief ProgressBar looks similar to Horizon's
     * progress bar shown in data management
     */
    class ProgressBar : public BaseProgress {
        private:
            /** @brief Border/Outline texture */
            Box * boxTex;
            /** @brief Actual progress bar texture */
            Rectangle * progressTex;

            /**
             * @brief Redraws the progress bar and
             * all elements associated with it
             */
            void redrawBar();

        public:
            /**
             * @brief Construct a new Progress Bar object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param w wdith of progress bar
             */
            ProgressBar(int x, int y, int w);

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
             * @brief Get the colour of the progress bar
             *
             * @return progress bar colour
             */
            Colour colour();

            /**
             * @brief Set the colour of the progress bar
             *
             * @param c new progress bar colour
             */
            void setColour(Colour c);
    };
};

#endif