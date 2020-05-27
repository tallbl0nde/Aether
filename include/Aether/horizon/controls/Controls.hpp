#ifndef AETHER_CONTROLS_HPP
#define AETHER_CONTROLS_HPP

#include "Aether/base/Container.hpp"
#include "Aether/horizon/controls/ControlItem.hpp"

namespace Aether {
    /**
     * @brief "Controls" contains the control scheme found at the bottom of a screen.
     * It can be positioned anywhere though.
     * 
     * @note x/y should be the top left-most point,
     * and the w/h should be set to the desired size.
     * Items are automatically right aligned.
     * First = right-most; Last = left-most
     */
    class Controls : public Container {
        private:
            /** @brief Colour to draw controls in */
            Colour colour;
            /** @brief Pointer to elements in order to access class-specific methods */
            std::vector<ControlItem *> items;

            /**
             * @brief Called to position all control items
             */
            void repositionElements();

            // Prevent interfering with elements directly
            using Element::addElement;
            using Element::removeElement;
            using Element::removeAllElements;

        public:
            /**
             * @brief Construct a new Controls object.
             * Controls are automatically positioned in place unless specified.
             * 
             * @param x x-coordinate of start position offset (default 45)
             * @param y y-coordinate of start position offset (default 647)
             * @param w width of control (default 1190)
             * @param h height of control (default 73)
             */
            Controls(int x = 45, int y = 647, int w = 1190, int h = 73);

            /**
             * @brief Add new control item
             * 
             * @param i pointer to control item
             */
            void addItem(ControlItem * i);

            /**
             * @brief Remove new control item
             * 
             * @param i pointer to control item
             * @return true if item successfully removed
             * @return false otherwise
             */
            bool removeItem(ControlItem * i);

            /**
             * @brief Remove all control items
             */
            void removeAllItems();

            /**
             * @brief Set new x-coordinate of start position offset
             * @note repositionElements() is called when value changes
             * @param x new x-coordinate of start position offset
             */
            void setX(int x);

            /**
             * @brief Set new y-coordinate of start position offset
             * @note repositionElements() is called when value changes
             * @param y new y-coordinate of start position offset
             */
            void setY(int y);

            /**
             * @brief Set new width for control
             * @note repositionElements() is called when value changes
             * @param w new width
             */
            void setW(int w);

            /**
             * @brief Set new height for control
             * @note repositionElements() is called when value changes
             * @param h new height
             */
            void setH(int h);

            /**
             * @brief Get the colour of controls
             * @note repositionElements
             * @return control colour 
             */
            Colour getColour();

            /**
             * @brief Set the colour of controls
             * 
             * @param c new colour
             */
            void setColour(Colour c);

            /**
             * @brief Set the colour of controls
             * 
             * @param r red value of colour
             * @param g green value of colour
             * @param b blue value of colour
             * @param a alpha value of colour
             */
            void setColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    };
};


#endif