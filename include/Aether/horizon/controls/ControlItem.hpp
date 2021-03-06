#ifndef AETHER_CONTROLS_CONTROLITEM_HPP
#define AETHER_CONTROLS_CONTROLITEM_HPP

#include "Aether/primary/Text.hpp"

namespace Aether {
    /**
     * @brief An item used by and updated by the \ref ControlBar element.
     * It shows a button icon and label.
     */
    class ControlItem : public Element {
        private:
            Button button_; /** @brief Associated button. */

            Text * icon;    /** @brief Button icon. */
            Text * label;   /** @brief Label text. */

        public:
            /**
             * @brief Construct a new ControlItem.
             *
             * @param btn Button to show/send event for.
             * @param label Label to display.
             */
            ControlItem(const Button btn, const std::string & label);

            /**
             * @brief Returns the control's associated button.
             *
             * @return Button that the control is associated with.
             */
            Button button();

            /**
             * @brief Set the colour to tint the element with.
             *
             * @param colour Colour used for tinting.
             */
            void setColour(const Colour & colour);

            /**
             * @brief Set the control's label.
             *
             * @param label New label to display.
             */
            void setLabel(const std::string & label);
    };
};

#endif