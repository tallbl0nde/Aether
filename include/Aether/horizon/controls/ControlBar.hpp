#ifndef AETHER_CONTROLCONTAINER_HPP
#define AETHER_CONTROLCONTAINER_HPP

#include "Aether/base/Container.hpp"

// Forward declare
namespace Aether {
    class ControlItem;
};

namespace Aether {
    /**
     * @brief A container holding control elements (and nothing else)
     * which automatically arranges them at the bottom of the screen.
     */
    class ControlBar : public Container {
        private:
            // Hide the regular addElement methods in order to only allow Controls.
            using Container::addElement;
            using Container::removeElement;
            using Container::removeAllElements;
            using Container::returnElement;
            using Container::returnAllElements;

            /**
             * @brief Colour to tint disabled controls with.
             */
            Colour disabledColour;

            /**
             * @brief Colour to tint enabled controls with.
             */
            Colour enabledColour;

            /**
             * @brief Gets a child ControlItem for the given button.
             *
             * @param btn Button to get item for
             * @return ControlItem if one is found, or nullptr
             */
            ControlItem * getControl(const Button btn);

            /**
             * @brief Repositions the items based on their order in the
             * 'children' vector (0 = right-most).
             */
            void repositionItems();

        public:
            /**
             * @brief Constructs a new empty ControlBar.
             * It automatically positions itself at the bottom of the screen.
             * Use setX/Y/W/H to configure position + size if needed.
             */
            ControlBar();

            /**
             * @brief Adds a new control to the left of the bar with the
             * given attributes. Does nothing if one already exists with
             * the same button.
             *
             * When touched, a global button press event is sent out.
             *
             * @param btn Button to add control item for.
             * @param label Label to show adjacent to button.
             */
            void addControl(const Button btn, const std::string & label);

            /**
             * @brief Disables the requested control item.
             * Does nothing if the button has not been added.
             *
             * @param btn Button to disable.
             */
            void disableControl(const Button btn);

            /**
             * @brief Enables the requested control item.
             * Does nothing if the button has not been added.
             *
             * @param btn Button to enable.
             */
            void enableControl(const Button btn);

            /**
             * @brief Moves a control to another position.
             * Provide 'NO_BUTTON' as the 'before' button to move to the
             * very right of the bar.
             *
             * @param btn Button to move.
             * @param before Button to place 'before' (to the left of)
             */
            void moveControl(const Button btn, const Button before);

            /**
             * @brief Update the label associated with the button.
             *
             * @param btn Button to update.
             * @param label New label to set.
             */
            void updateControl(const Button btn, const std::string & label);

            /**
             * @brief Remove all controls from the container.
             */
            void removeAllControls();

            /**
             * @brief Remove the control for the given button.
             *
             * @param btn Control to remove.
             * @return True if removed, false if not found.
             */
            bool removeControl(const Button btn);

            /**
             * @brief Set the colour to tint disabled controls with.
             *
             * @param colour Colour used for tinting.
             */
            void setDisabledColour(const Colour colour);

            /**
             * @brief Set the colour to tint enabled controls with.
             *
             * @param colour Colour used for tinting.
             */
            void setEnabledColour(const Colour colour);

            /**
             * @brief Set the width of the container.
             *
             * @param w Width of container in pixels
             */
            void setW(int w);

            /**
             * @brief Set the height of the container.
             *
             * @param h Height of container in pixels
            */
            void setH(int h);
    };
};

#endif