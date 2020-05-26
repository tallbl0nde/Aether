#ifndef AETHER_LISTCOMMENT_HPP
#define AETHER_LISTCOMMENT_HPP

#include "Aether/base/Element.hpp"
#include "Aether/primary/TextBlock.hpp"

namespace Aether {
    /**
     * @brief A ListComment is a sentence/paragraph usually found under an
     * option describing what it does.
     */
    class ListComment : public Element {
        private:
            /** @brief Text block for comment text */
            TextBlock * text;

            /**
             * @brief Repositions and re-renders comment text block
             */
            void updateElement();

        public:
            /**
             * @brief Construct a new List Comment object
             * 
             * @param s comment string
             */
            ListComment(std::string s);

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
             * @brief Set new width for comment
             * @note Text must be rerendered when width changes
             * @param w new width
             */
            void setW(int w);
    };
};

#endif