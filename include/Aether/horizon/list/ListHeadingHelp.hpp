#ifndef AETHER_LISTHEADINGHELP_HPP
#define AETHER_LISTHEADINGHELP_HPP

#include "Aether/base/Container.hpp"
#include "Aether/horizon/button/HelpButton.hpp"
#include "Aether/horizon/list/ListHeading.hpp"

namespace Aether {
    /**
     * @brief A ListHeading but with a help icon that's right aligned
     */
    class ListHeadingHelp : public Container {
        private:
            /** @brief  */
            ListHeading * heading;
            /** @brief  */
            HelpButton * help;

            /**
             * @brief Reposition elements. Usually called after size change.
             */
            void positionElements();

        public:
            /**
             * @brief Construct a new List Heading Help object.
             * Auto positions elements when instantiated.
             * 
             * @param s list heading string
             * @param f callback function when help button is activated
             */
            ListHeadingHelp(std::string s, std::function<void()> f);

            /**
             * @brief Get the help colour
             * 
             * @return help colour
             */
            Colour getHelpColour();

            /**
             * @brief Set the help colour
             * 
             * @param c new help colour
             */
            void setHelpColour(Colour c);

            /**
             * @brief Get the rectangle colour
             * 
             * @return rectangle colour 
             */
            Colour getRectColour();

            /**
             * @brief Set the rectangle colour
             * 
             * @param c new rectangle coloue
             */
            void setRectColour(Colour c);

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
             * @brief Set the callback function for help button activation
             * 
             * @param f new callback function
             */
            void setHelpCallback(std::function<void()> f);

            /**
             * @brief Adjusts the list heading width.
             * Repositions elements as necessary.
             * 
             * @param w new width
             */
            void setW(int w);

            /**
             * @brief Adjusts the list heading height.
             * Repositions elements as necessary.
             * 
             * @param h new height
             */
            void setH(int h);
    };
};

#endif