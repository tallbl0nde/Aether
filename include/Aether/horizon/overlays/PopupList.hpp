#ifndef AETHER_POPUPLIST_HPP
#define AETHER_POPUPLIST_HPP

#include "Aether/Overlay.hpp"
#include "Aether/horizon/controls/ControlBar.hpp"
#include "Aether/horizon/list/List.hpp"
#include "Aether/horizon/list/ListButton.hpp"
#include "Aether/primary/Rectangle.hpp"

namespace Aether {
    /**
     * @brief A "PopupList" is an overlay at the bottom of the screen containing
     * a list of items to select from. A handler must be specified for each
     * item, and the overlay is closed when any item is selected.
     */
    class PopupList : public Overlay {
        private:
            /** @brief Pointer to controls */
            ControlBar * ctrl;
            /** @brief Pointer to list */
            List * list;
            /** @brief Pointer to main rectangle */
            Rectangle * rect;
            /** @brief Pointer to top rectangle */
            Rectangle * top;
            /** @brief Pointer to bottom rectangle */
            Rectangle * bottom;
            /** @brief Title */
            Text * title;
            /** @brief Pointers to list buttons */
            std::vector<ListButton *> items;
            /** @brief List Highlight COloue */
            Colour hiColour;
            /** @brief List Line Colour */
            Colour llColour;
            /** @brief List Text Colour */
            Colour txColour;

        public:
            /**
             * @brief Construct a new Popup List object
             *
             * @param s title string
             */
            PopupList(std::string s);

            /**
             * @brief Add new entry to popup list
             * @note Back/OK buttons have default text but can be changed using other functions
             * @param s entry item string
             * @param f function to invoke if entry is selected
             * @param t indicator on whether to show entry as ticked
             */
            void addEntry(std::string s, std::function<void()> f, bool t = false);

            /**
             * @brief Set the title label
             *
             * @param s new label
             */
            void setTitleLabel(std::string s);

            /**
             * @brief Set the back button label
             *
             * @param s new label
             */
            void setBackLabel(std::string s);

            /**
             * @brief Set the OK button label
             *
             * @param s new label
             */
            void setOKLabel(std::string s);

            /**
             * @brief Removes all entries from list
             */
            void removeEntries();

            /**
             * @brief Get the background colour
             *
             * @return background colour
             */
            Colour getBackgroundColour();

            /**
             * @brief Set the background colour
             *
             * @param c new colour
             */
            void setBackgroundColour(Colour c);

            /**
             * @brief Get the text colour
             *
             * @return text colour
             */
            Colour getTextColour();

            /**
             * @brief Set the text colour
             *
             * @param c new colour
             */
            void setTextColour(Colour c);

            /**
             * @brief Get the line colour
             *
             * @return line colour
             */
            Colour getLineColour();

            /**
             * @brief Set the line colour
             *
             * @param c new colour
             */
            void setLineColour(Colour c);

            /**
             * @brief Get the highlight colour
             *
             * @return highlight colour
             */
            Colour getHighlightColour();

            /**
             * @brief Set the highlight colour
             *
             * @param c new colour
             */
            void setHighlightColour(Colour c);

            /**
             * @brief Get the list line colour
             *
             * @return list line colour
             */
            Colour getListLineColour();

            /**
             * @brief Set the list line colour
             *
             * @param c new colour
             */
            void setListLineColour(Colour c);

            /**
             * @brief Set the all colours
             *
             * @param bg new background colour
             * @param hi new highlight colour
             * @param li new line colour
             * @param lli new list line colour
             * @param tx new text colour
             */
            void setAllColours(Colour bg, Colour hi, Colour li, Colour lli, Colour tx);
    };
};

#endif