#include "List.hpp"

namespace Aether {
    List::List(int x, int y, int w, int h) : Scrollable(x, y, w, h) {
        this->setShowScrollBar(true);
    }
};