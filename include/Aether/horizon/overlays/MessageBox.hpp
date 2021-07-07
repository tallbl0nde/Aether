#ifndef AETHER_MESSAGEBOX_HPP
#define AETHER_MESSAGEBOX_HPP

#include "Aether/horizon/button/BorderButton.hpp"
#include "Aether/primary/Rectangle.hpp"
#include "Aether/Overlay.hpp"

namespace Aether {
    /**
     * @brief A "MessageBox" is a box presented in the middle of the screen,
     * usually containing a few yes/no/cancel buttons and some text.
     * Buttons are set up using the provided functions whereas the
     * "body" of the box is set by giving an element containing the
     * desired elements.
     */
    class MessageBox : public Overlay {
        private:
            /** @brief Bottom left button */
            BorderButton * left;
            /** @brief Bottom right button */
            BorderButton * right;
            /** @brief Top button */
            BorderButton * top;
            /** @brief Top line/separator */
            Rectangle * topH;
            /** @brief Bottom line/separator */
            Rectangle * bottomH;
            /** @brief Vertical line/separator */
            Rectangle * vertH;
            /** @brief Element used as body (set to nullptr if nothing). Anchored at top left of rectangle. */
            Element * body;
            /** @brief Rectangle "background" */
            Rectangle * rect;
            /** @brief Button text colour */
            Colour buttonColour;

            /**
             * @brief Repositions the buttons
             */
            void repositionButtons();

            /**
             * @brief Resizes the elements
             */
            void resizeElements();

        public:
            /**
             * @brief Construct a new Message Box object.
             */
            MessageBox();

            /**
             * @brief Set the line colour
             * @note Must be called after adding buttons!
             * @param c new line colour
             */
            void setLineColour(Colour c);

            /**
             * @brief Set the rectangle colour
             *
             * @param c new rectangle colour
             */
            void setRectangleColour(Colour c);

            /**
             * @brief Set the text colour object
             *
             * @param c new text colour
             */
            void setTextColour(Colour c);

            /**
             * @brief Add button to bottom left rectangle.
             * @note Don't call these multiple times as the previous ones aren't deleted!
             * @param s button label string
             * @param f function to invoke when button pressed
             */
            void addLeftButton(std::string s, std::function<void()> f);

            /**
             * @brief Add button to bottom right rectangle.
             * @note Don't call these multiple times as the previous ones aren't deleted!
             * @param s button label string
             * @param f function to invoke when button pressed
             */
            void addRightButton(std::string s, std::function<void()> f);

            /**
             * @brief Add button to top rectangle.
             * @note Don't call these multiple times as the previous ones aren't deleted!
             * @param s button label string
             * @param f function to invoke when button pressed
             */
            void addTopButton(std::string s, std::function<void()> f);

            /**
             * @brief Get the body size
             *
             * @param w pointer to write body width to
             * @param h pointer to write body height to
             */
            void getBodySize(int * w, int * h);

            /**
             * @brief Set the body size
             *
             * @param w new body width
             * @param h new body height
             */
            void setBodySize(int w, int h);

            /**
             * @brief Deletes the element (and it's children!) used as body
             */
            void emptyBody();

            /**
             * @brief Set the body element
             * Does nothing if a body has been set alreadu!
             * @param e body element to set
             */
            void setBody(Element * e);
    };
};

#endif
