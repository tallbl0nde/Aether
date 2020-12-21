#ifndef AETHER_SPINNER_HPP
#define AETHER_SPINNER_HPP

#include "Aether/base/Container.hpp"
#include "Aether/primary/Text.hpp"
#include "Aether/utils/Types.hpp"

/**
 * @brief Enum class showing all available hold actions for spinner
 */
enum class SpinnerHoldAction {
    UpHeld,         /**< Indicates increase value is being held (but not repeating yet)*/
    UpHeldRepeat,   /**< Indicates increase value is being held (and increasing value)*/
    DownHeld,       /**< Indicates decrease value is being held (but not repeating yet)*/
    DownHeldRepeat, /**< Indicates decrease value is being held (and decreasing value)*/
    NoHeld          /**< Indicates nothing is held*/
};

namespace Aether {
    /**
     * @brief A Spinner contains a number which can be increased/decreased
     * by pressing the associated up/down buttons or the dpad.
     * @note This element has a default height and should not be changed!
     */
    class Spinner : public Container {
        private:
            /** @brief Up arrow text */
            Text * up;
            /** @brief Down arrow text */
            Text * down;
            /** @brief Actual value/text */
            Text * str;
            /** @brief Label text (hidden if not set) */
            Text * label_;
            /** @brief Up Arrow container (for larger touch area) */
            Element * upContainer;
            /** @brief Down Arrow container (for larger touch area) */
            Element * downContainer;
            /** @brief Indicator on whether spinner is focused (needed for colours) */
            bool isFocused;
            /** @brief Indicator on whether to allow values to wrap from max - min and vice versa */
            bool wrap;
            /** @brief Number of zeroes to pad number with */
            unsigned int padding;
            /** @brief Spinner step size */
            int amount;
            /** @brief Spinner minimum value */
            int min_;
            /** @brief Spinner maximum value */
            int max_;
            /** @brief Current spinner value */
            int value_;
            /** @brief Indicator on whether hold action is being performed currently */
            SpinnerHoldAction held;
            /** @brief Total time hold action was performed for */
            uint32_t holdTime;
            /** @brief Arrow colour */
            Colour arrowC;
            /** @brief Highlight colour */
            Colour highlightC;
            /** @brief Text colour */
            Colour textC;

            /**
             * @brief Increase value once by step size specified until max is reached.
             * Wraps overflowed increased value if wrap is set true.
             */
            void incrementVal();

            /**
             * @brief Decrease value once by step size specified until min is reached.
             * Wraps underflowed decreased value if wrap is set true.
             */
            void decrementVal();

        public:
            /**
             * @brief Construct a new Spinner object.
             * Value is initialized to 0.
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param w spinner width (default 90)
             */
            Spinner(int x, int y, int w = 90);

            /**
             * @brief Attempt to handle given event
             *
             * @param e event to attempt handle
             * @return true if event was handled
             * @return false otherwise
             */
            bool handleEvent(InputEvent * e);

            /**
             * @brief Updates spinner container.
             * Updates textures and values as necessary.
             *
             * @param dt change in time
             */
            void update(unsigned int dt);

            /**
             * @brief Set the spinner as active (focused)
             */
            void setActive();

            /**
             * @brief Set the spinner as inactive (un-focused)
             */
            void setInactive();

            /**
             * @brief Get whether to wrap around value if is overflowed/underflowed
             *
             * @return true if value should be wrapped
             * @return false otherwise
             */
            bool wrapAround();

            /**
             * @brief Set whether to wrap around value if is overflowed/underflowed
             *
             * @param b true if value should be wrapped, false otherwise
             */
            void setWrapAround(bool b);

            /**
             * @brief Get current value padding (number of zeros to pad value with)
             *
             * @return number of zeros to pad with
             */
            unsigned int digits();

            /**
             * @brief Set current value padding (number of zeros to pad value with)
             * @note 0 to disable padding
             * @param p new number of zeros to pad with
             */
            void setDigits(unsigned int p);

            /**
             * @brief Set the current label string
             * @note Set as empty string to remove label
             * @param s new label string
             */
            void setLabel(std::string s);

            /**
             * @brief Get the current label string
             *
             * @return label string
             */
            std::string label();

            /**
             * @brief Get the step size to change value by
             *
             * @return step size
             */
            int changeAmount();

            /**
             * @brief Set the step size to change value by
             *
             * @param a new step size
             */
            void setChangeAmount(int a);

            /**
             * @brief Get the spinner value
             *
             * @return value
             */
            int value();

            /**
             * @brief Set the spinner value
             *
             * @param v new value
             */
            void setValue(int v);

            /**
             * @brief Get the minimum value
             *
             * @return min value
             */
            int min();

            /**
             * @brief Set the minimum value
             *
             * @param m new min value
             */
            void setMin(int m);

            /**
             * @brief Get the maximum value
             *
             * @return max value
             */
            int max();

            /**
             * @brief Set the maximum value
             *
             * @param m new max value
             */
            void setMax(int m);

            /**
             * @brief Get the arrow colour
             *
             * @return arrow colour
             */
            Colour getArrowColour();

            /**
             * @brief Set the arrow colour
             *
             * @param c new arrow colour
             */
            void setArrowColour(Colour c);

            /**
             * @brief Get the highlight colour
             *
             * @return highlight colour
             */
            Colour getHighlightColour();

            /**
             * @brief Set the highlight colour
             *
             * @param c new highlight colour
             */
            void setHighlightColour(Colour c);

            /**
             * @brief Get the text colour
             *
             * @return text colour
             */
            Colour getTextColour();

            /**
             * @brief Set the text colour
             *
             * @param c new text colour
             */
            void setTextColour(Colour c);
    };
};

#endif