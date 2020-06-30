#ifndef AETHER_TICK_HPP
#define AETHER_TICK_HPP

#include "Aether/primary/Ellipse.hpp"
#include "Aether/primary/Text.hpp"

namespace Aether {
    /**
     * @brief A Tick is an element that looks the same
     * as the tick shown in Horizon - a coloured circle
     * with a tick on top
     */
    class Tick : public Element {
        private:
            /** @brief Tick's circle element */
            Ellipse * circle;
            /** @brief Tick's text element (which will just be a unicode tick) */
            Text * tick;

        public:
            /**
             * @brief Construct a new Tick object
             *
             * @param x x-coordinate of start position offset
             * @param y x-coordinate of start position offset
             * @param d circle diameter
             */
            Tick(int x, int y, unsigned int d);

            /**
             * @brief Get the diameter of the circle element
             *
             * @return circle diameter
             */
            unsigned int size();

            /**
             * @brief Set the diamater of the circle element
             *
             * @param s new circle diameter
             */
            void setSize(unsigned int s);

            /**
             * @brief Get the circle element's colour
             *
             * @return circle colour
             */
            Colour getCircleColour();

            /**
             * @brief Set the circle element's colour
             *
             * @param c new circle colour
             */
            void setCircleColour(Colour c);

            /**
             * @brief Get the tick's text colour
             *
             * @return text colour
             */
            Colour getTickColour();

            /**
             * @brief Set the tick's text colour
             *
             * @param c new text colour
             */
            void setTickColour(Colour c);
    };
};

#endif