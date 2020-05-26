#ifndef AETHER_LISTSEPARATOR_HPP
#define AETHER_LISTSEPARATOR_HPP

#include "Aether/base/Element.hpp"

/** @brief Default list separator height */
#define DEFAULT_HEIGHT 45

namespace Aether {
    /**
     * @brief ListSeparator is simply a blank element. It has a default height
     * which matches Horizon's but can be changed if necessary using setH()
     */
    class ListSeparator : public Element {
        public:
            /**
             * @brief Construct a new List Separator object
             * 
             * @param h list separator height
             */
            ListSeparator(unsigned int h = DEFAULT_HEIGHT);
    };
};

#endif