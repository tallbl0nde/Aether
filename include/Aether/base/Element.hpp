#ifndef AETHER_ELEMENT_HPP
#define AETHER_ELEMENT_HPP

#include "Aether/InputEvent.hpp"
#include <vector>

namespace Aether {
    /**
     * @brief Element is the base class to be inherited
     * to form all other types of elements
     */
    class Element {
        private:
            /** @brief Horizontal position of element */
            int x_;
            /** @brief Vertical position of element */
            int y_;
            /** @brief Width of element */
            int w_;
            /** @brief Height of element */
            int h_;
            /** @brief Indicator on whether the element should be rendered or not */
            bool hidden_;
            /** @brief Function to call when the element is activated */
            std::function<void()> callback_;
            /** @brief Indicator on whether element contains highlighted children element */
            bool hasHighlighted_;
            /** @brief Indicator on whether element contains selectable children element */
            bool hasSelectable_;
            /** @brief Indicator on whether element contains selected children element */
            bool hasSelected_;
            /** @brief Indicator on whether element is highlighted */
            bool highlighted_;
            /** @brief Indicator on whether element is selectable */
            bool selectable_;
            /** @brief Indicator on whether element is selected */
            bool selected_;
            /** @brief Indicator on whether element is touch responsive */
            bool touchable_;

        protected:
            /** @brief Background color if element is highlighted */
            static Colour hiBG;
            /** @brief Border color if element is highlighted */
            static Colour hiBorder;
            /** @brief Selected color if element is highlighted */
            static Colour hiSel;
            /** @brief Size of highlight border */
            static unsigned int hiSize;
            /** @brief Indicator on whether the touch is "active" (i.e. hide highlighting) or not */
            static bool isTouch;
            /** @brief Pointer to parent element, if there is one */
            Element * parent_;
            /** @brief Vector of child elements (used to call their methods) */
            std::vector<Element *> children;
            /** @brief Children element which is highlighted/focused (to regain focus on activation) */
            Element * focused_;

            /**
             * @brief Add element at given index in vector
             * @note Will insert at end if index is beyond the end!
             *
             * @param e element to add
             * @param i index to insert at
             */
             void addElementAt(Element * e, size_t i);

        public:
            /**
             * @brief Construct a new Element object.
             *
             * Co-ordinates and size are optional.
             * Default value of co-ordinates are (0, 0)
             * and the default value of size are (100, 100).
             * Parent is set as nullptr
             *
             * @param x x-coordinate of element
             * @param y y-coordinate of element
             * @param w width of element
             * @param h height of element
             */
            Element(int x = 0, int y = 0, int w = 100, int h = 100);

            /**
             * @brief Returns x-coordinate of element
             *
             * @return int x-coordinate of element
             */
            int x();

            /**
             * @brief Returns y-coordinate of element
             *
             * @return int y-coordinate of element
             */
            int y();

            /**
             * @brief Returns width of element
             *
             * @return int width of element
             */
            int w();

            /**
             * @brief Returns height of element
             *
             * @return int height of element
             */
            int h();

            /**
             * @brief Set x-coordinate of element
             *
             * @param x new x-coordinate of element
             */
            virtual void setX(int x);

            /**
             * @brief Set y-coordinate of element
             *
             * @param y new y-coordinate of element
             */
            virtual void setY(int y);

            /**
             * @brief Set width of element
             *
             * @param w new width of element
             */
            virtual void setW(int w);

            /**
             * @brief Set height of element
             *
             * @param h new height of element
             */
            virtual void setH(int h);

            /**
             * @brief Set co-ordinates of element
             *
             * @param x new x-coordinate of element
             * @param y new y-coordinate of element
             */
            virtual void setXY(int x, int y);

            /**
             * @brief Set size of element
             *
             * @param w new width of element
             * @param h new height of element
             */
            virtual void setWH(int w, int h);

            /**
             * @brief Setter size and co-ordinates of element
             *
             * @param x new x-coordinate of element
             * @param y new y-coordinate of element
             * @param w new width of element
             * @param h new height of element
             */
            virtual void setXYWH(int x, int y, int w, int h);

            /**
             * @brief Returns the parent element of this element
             *
             * @return parent element
             */
            Element * parent();

            /**
             * @brief Set the parent element of this element
             *
             * @param p parent element to set as
             */
            void setParent(Element * p);

            /**
             * @brief Adds new element a child
             *
             * @param e element to add
             */
            virtual void addElement(Element * e);

            /**
             * @brief Attempt to delete given child element
             * @note This calls the element destructor if it is found
             *
             * @param e child to delete
             * @return true if deleted successfully (i.e. was a child)
             * @return false if not a child of element
             */
            virtual bool removeElement(Element * e);

            /**
             * @brief Deletes all children elements
             */
            virtual void removeAllElements();

            /**
             * @brief Attempt to remove the given child element
             * @note This does not call the element's destructor, hence it can be reused
             *
             * @param e child to remove
             * @return true if removed successfully (i.e. was a child)
             * @return false if not a child of element
             */
            virtual bool returnElement(Element * e);

            /**
             * @brief Removes (but does not delete) all children elements
             */
            virtual void returnAllElements();

            /**
             * @brief Check if current element is visible on-screen
             *
             * @return true if element is visible
             * @return false if element is not visible
             */
            bool isVisible();

            /**
             * @brief Check if current element is hidden
             *
             * @return true if element is hidden
             * @return false if element is not hidden
             */
            bool hidden();

            /**
             * @brief Set whether element is hidden or not
             *
             * @param b state to change hidden status to
             */
            void setHidden(bool b);

            /**
             * @brief Check if current element is selected
             *
             * @return true if current element is selected
             * @return false if current element is not selected
             */
            bool selected();

            /**
             * @brief Set whether element is selected or not
             *
             * @param b state to change selected status to
             */
            void setSelected(bool b);

            /**
             * @brief Check if current element is selectable
             *
             * @return true if current element is selectable
             * @return false if current element is not selectable
             */
            bool selectable();

            /**
             * @brief Set whether element is selectable or not
             *
             * @param b state to change selectable status to
             */
            void setSelectable(bool b);

            /**
             * @brief Check if current element is touch responsive
             *
             * @return true if current element is touch responsive
             * @return false if current element is not touch responsive
             */
            bool touchable();

            /**
             * @brief Set whether element is touch responsive or not
             *
             * @param b state to change touch responsive status to
             */
            void setTouchable(bool b);

            /**
             * @brief Check if current element is highlighted
             *
             * @return true if current element is highlighted
             * @return false if current element is not highlighted
             */
            bool highlighted();

            /**
             * @brief Set whether element is highlighted or not
             *
             * @param b state to change highlighted status to
             */
            void setHighlighted(bool b);

            /**
             * @brief Check if current element has highlighted children element
             *
             * @return true if current element has highlighted children element
             * @return false if current element does not have highlighted children element
             */
            bool hasHighlighted();

            /**
             * @brief Set whether element has highlighted children element or not
             *
             * @param b state to change highlighted children element status to
             */
            void setHasHighlighted(bool b);

            /**
             * @brief Check if current element has selectable children element
             *
             * @return true if current element has selectable children element
             * @return false if current element doesn't have selectable children element
             */
            bool hasSelectable();
            /**
             * @brief Set whether element has selectable children element or not
             *
             * @param b state to change selectable children element status to
             */
            void setHasSelectable(bool b);

            /**
             * @brief Check if current element has selected children element
             *
             * @return true if current element has selected children element
             * @return false if current element doesn't have selected children element
             */
            bool hasSelected();
            /**
             * @brief Set whether element has selected children element or not
             *
             * @param b state to change selected children element status to
             */
            void setHasSelected(bool b);

            /**
             * @brief Set element as activated
             */
            virtual void setActive();

            /**
             * @brief Set element as in-activated
             */
            virtual void setInactive();

            /**
             * @brief Sets current element as unfocused
             * and instead focuses a sub-element
             * @note This is the same as setFocussed
             *
             * @param e element to focus
             */
            void setFocused(Element * e);

            /**
             * @brief Sets current element as unfocused
             * and instead focuses a sub-element
             * @note This is the same as setFocused
             *
             * @param e element to focus
             */
            void setFocussed(Element * e);

            /**
             * @brief Returns the currently focused element
             * @note This is the same as focussed
             *
             * @return focused element
             */
            Element * focused();

            /**
             * @brief Returns the currently focused element
             * @note This is the same as focused
             *
             * @return focused element
             */
            Element * focussed();

            /**
             * @brief Returns callback function (nullptr if no callback assigned)
             *
             * @return callback function
             */
            std::function<void()> callback();

            /**
             * @brief Set callback function (also marks element as selectable)
             *
             * @param f new callback function to set
             */
            void setCallback(std::function<void()> f);

            /**
             * @brief Attempt to handle a given event
             *
             * @param e event to handle
             * @return true if event was handled
             * @return false if event was not handled
             */
            virtual bool handleEvent(InputEvent * e);

            /**
             * @brief Update is passed time since last frame (for animations)
             *
             * @param dt change in time
             */
            virtual void update(uint32_t dt);

            /**
             * @brief Render child elements
             */
            virtual void render();

            /**
             * @brief Renders the highlight border + background
             *
             * Default highlighted shapes are rectangles.
             */
            virtual void renderHighlighted();

            /**
             * @brief Renders the selected overlay
             */
            virtual void renderSelected();

            /**
             * @brief Destroy the Element object
             */
            virtual ~Element();

            /**
             * @brief Returns the element currently highlighted within given element or nullptr if none found
             *
             * @param e currently highlighted element within given element or nullptr if none found
             */
            friend void moveHighlight(Element * e);
    };
};

#endif