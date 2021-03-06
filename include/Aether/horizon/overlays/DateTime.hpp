#ifndef AETHER_DATETIME_HPP
#define AETHER_DATETIME_HPP

#include <ctime>
#include "Aether/horizon/button/BorderButton.hpp"
#include "Aether/horizon/controls/ControlBar.hpp"
#include "Aether/horizon/input/Spinner.hpp"
#include "Aether/primary/Rectangle.hpp"
#include "Aether/Overlay.hpp"

namespace Aether {
    /**
     * @brief Spinners to show (create custom by passing bitwise OR of multiple flags!)
     * Each bit represents in the order - 0b00DMYHMS (Day / Month / Year / Hour / Minute / Second)
     */
    enum class DTFlag {
        DateTime = 0b00111111, /**< Shows all date and time */
        Date = 0b00111000, /**< Shows all date */
        Time = 0b00000111, /**< Shows all time */
        Day = 0b00100000, /**< Shows day */
        Month = 0b00010000, /**< Shows month */
        Year = 0b00001000, /**< Shows year */
        Hour = 0b00000100, /**< Shows hour */
        Min = 0b00000010, /**< Shows minute */
        Sec = 0b00000001 /**< Shows second */
    };

    /**
     * @brief Bitwise OR operator for DTFlag
     *
     * @param a first flag
     * @param b second flag
     * @return resultant flag
     */
    inline DTFlag operator|(DTFlag a, DTFlag b) {
        return static_cast<DTFlag>(static_cast<int>(a) | static_cast<int>(b));
    }

    /**
     * @brief The DateTime overlay is used to get the user to select a date/time.
     * It must be passed a tm struct on creation which has it's value changed
     * in place when closed (not updated when closed by pressing B!)
     *
     * @note Expect a segfault if the tm struct is deleted by the time this overlay is closed!
     */
    class DateTime : public Overlay {
        private:
            /** @brief Reference to time struct to set */
            struct tm & refTm;
            /** @brief Pointer to controls */
            ControlBar * ctrl;
            /** @brief Pointer to main rectangle */
            Rectangle * rect;
            /** @brief Pointer to top rectangle */
            Rectangle * top;
            /** @brief Pointer to bottom rectangle */
            Rectangle * bottom;
            /** @brief Pointer to title */
            Text * title;
            /** @brief Pointer to button */
            BorderButton * button;
            /** @brief Pointer to day selection spinner */
            Spinner * day;
            /** @brief Pointer to month selection spinner */
            Spinner * month;
            /** @brief Pointer to year selection spinner */
            Spinner * year;
            /** @brief Pointer to hour selection spinner */
            Spinner * hour;
            /** @brief Pointer to minute selection spinner */
            Spinner * min;
            /** @brief Pointer to second selection spinner */
            Spinner * sec;
            /** @brief Separating divider 1 */
            Text * div1;
            /** @brief Separating divider 2 */
            Text * div2;
            /** @brief Separating column 1 */
            Text * col1;
            /** @brief Separating column 2 */
            Text * col2;

            /**
             * @brief Updates value in tm struct
             */
            void setTmValues();

        public:
            /**
             * @brief Construct a new Date Time object.
             * Back/OK buttons have default text but can be changed using other functions
             *
             * @param s title
             * @param t reference for time struct to update
             * @param d pickers to show (shows complete date and time by default)
             */
            DateTime(std::string s, struct tm & t, DTFlag d = DTFlag::DateTime);

            /**
             * @brief Attempt to handle event.
             * Whenever an event is handled, update max days for selected month.
             *
             * @param e event to attempt handle
             * @return true if event was handled
             * @return false otherwise
             */
            bool handleEvent(InputEvent * e);

            /**
             * @brief Set the back button label
             *
             * @param s new back label
             */
            void setBackLabel(std::string s);

            /**
             * @brief Set the OK button label
             *
             * @param s new OK label
             */
            void setOKLabel(std::string s);

            /**
             * @brief Set the day spinner hint
             *
             * @param s new day hint string
             */
            void setDayHint(std::string s);

            /**
             * @brief Set the month spinner hint
             *
             * @param s new month hint string
             */
            void setMonthHint(std::string s);

            /**
             * @brief Set the year spinner hint
             *
             * @param s new year hint string
             */
            void setYearHint(std::string s);

            /**
             * @brief Set the hour spinner hint
             *
             * @param s new hour hint string
             */
            void setHourHint(std::string s);

            /**
             * @brief Set the minute spinner hint
             *
             * @param s new minute hint string
             */
            void setMinuteHint(std::string s);

            /**
             * @brief Set the second spinner hint
             *
             * @param s new second hint string
             */
            void setSecondHint(std::string s);

            /**
             * @brief Get the background colour
             *
             * @return background colour
             */
            Colour getBackgroundColour();

            /**
             * @brief Set the background colour
             *
             * @param c new background colour
             */
            void setBackgroundColour(Colour c);

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
             * @brief Get the inactive colour
             *
             * @return inactive colour
             */
            Colour getInactiveColour();

            /**
             * @brief Set the inactive colour
             *
             * @param c new inactive colour
             */
            void setInactiveColour(Colour c);

            /**
             * @brief Get the separator colour
             *
             * @return separator colour
             */
            Colour getSeparatorColour();

            /**
             * @brief Set the separator colour
             *
             * @param c new separator colour
             */
            void setSeparatorColour(Colour c);

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

            /**
             * @brief Set all colours
             *
             * @param c1 new background colour
             * @param c2 new highlight colour
             * @param c3 new inactive colour
             * @param c4 new separator colour
             * @param c5 new text colour
             */
            void setAllColours(Colour c1, Colour c2, Colour c3, Colour c4, Colour c5);
    };
};

#endif
