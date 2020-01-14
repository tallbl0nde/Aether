#ifndef AETHER_LISTOPTION_HPP
#define AETHER_LISTOPTION_HPP

#include "ListItem.hpp"
#include "primary/Rectangle.hpp"
#include "primary/Text.hpp"

namespace Aether {
    // A ListOption is a ListItem similar to what appears in Horizon's
    // lists. It has a string and value, as well as a callback function
    // which is called when selected.
    class ListOption : public ListItem {
        private:
            // Elements for lines + strings
            Rectangle * topR;
            Rectangle * bottomR;
            Text * string;
            Text * value;

            // Function to call when selected
            std::function<std::string()> func;

            // Reposition elements
            void positionElements();

        public:
            // Constructor takes strings + function
            ListOption(std::string, std::string, std::function<std::string()>);

            // Getters + setters for colours
            Colour getLineColour();
            void setLineColour(Colour);
            Colour getHintColour();
            void setHintColour(Colour);
            Colour getValueColour();
            void setValueColour(Colour);
            // Set all colours at once: line, hint, value
            void setColours(Colour, Colour, Colour);

            // Adjusting width requires changing width of rectangles + repositioning of text
            void setW(int);
    };
};

#endif