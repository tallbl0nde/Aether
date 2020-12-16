#ifndef AETHER_BASEPROGRESS_HPP
#define AETHER_BASEPROGRESS_HPP

#include "Aether/base/Element.hpp"

namespace Aether {
    /**
     * @brief Base element for progress bars that handles the value
     */
    class BaseProgress : public Element {
        private:
            /** @brief Value of progress bar */
            float value_;

            /**
             * @brief Redraw texture(s)
             */
            virtual void redrawBar() = 0;

        public:
            /**
             * @brief Construct a new Base Progress object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param w width of progressbar
             * @param h height of progressbar
             */
            BaseProgress(int x, int y, int w, int h);

            /**
             * @brief Get the value of progress bar
             *
             * @return value of progress bar
             */
            float value();

            /**
             * @brief Set the value of progress bar
             * @note Value of progress bar from 0.0 to 100.0.
             * @note Any less/greater will be rounded down/up
             *
             * @param v new value of progress bar
             */
            virtual void setValue(float v);
    };
};

#endif