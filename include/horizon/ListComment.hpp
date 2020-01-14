#ifndef AETHER_LISTCOMMENT_HPP
#define AETHER_LISTCOMMENT_HPP

#include "ListItem.hpp"
#include "primary/TextBlock.hpp"

namespace Aether {
    // A ListComment is a sentence/paragraph usually found under an
    // option describing what it does.
    class ListComment : public ListItem {
        private:
            // TextBLock element
            TextBlock * text;

            // Reposition + render text block
            void updateElement();

        public:
            // Constructor takes string
            ListComment(std::string);

            // Getter + setter for colour
            Colour getTextColour();
            void setTextColour(Colour);

            // Text must be rerendered when width changes
            void setW(int);
    };
};

#endif