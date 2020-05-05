#ifndef AETHER_ELEMENT_HPP
#define AETHER_ELEMENT_HPP

#include "InputEvent.hpp"
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
            Element * parent;
            /** @brief Vector of child elements (used to call their methods) */
            std::vector<Element *> children;
            /** @brief Element which is highlighted/focused (to regain focus on activation) */
            Element * focussed_;

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
             * @brief Getter function for x-coordinate of element
             * 
             * @return int x-coordinate of element
             */
            int x();

            /**
             * @brief Getter function for y-coordinate of element
             * 
             * @return int y-coordinate of element
             */
            int y();

            /**
             * @brief Getter function for width of element
             * 
             * @return int width of element
             */
            int w();

            /**
             * @brief Getter function for height of element
             * 
             * @return int height of element
             */
            int h();

            /**
             * @brief Setter function for x-coordinate of element
             * 
             * @param x new x-coordinate of element
             */
            virtual void setX(int x);

            /**
             * @brief Setter function for y-coordinate of element
             * 
             * @param y new y-coordinate of element
             */
            virtual void setY(int y);

            /**
             * @brief Setter function for width of element
             * 
             * @param w new width of element
             */
            virtual void setW(int w);

            /**
             * @brief Setter function for height of element
             * 
             * @param h new height of element
             */
            virtual void setH(int h);

            /**
             * @brief Setter function for co-ordinates of element
             * 
             * @param x new x-coordinate of element
             * @param y new y-coordinate of element
             */
            virtual void setXY(int x, int y);

            /**
             * @brief Setter function for size of element
             * 
             * @param w new width of element
             * @param h new height of element
             */
            virtual void setWH(int w, int h);

            /**
             * @brief Setter function to for size and co-ordinates of element
             * 
             * @param x new x-coordinate of element
             * @param y new y-coordinate of element
             * @param w new width of element
             * @param h new height of element
             */
            virtual void setXYWH(int x, int y, int w, int h);

            /**
             * @brief Set the parent element for current element
             * 
             * @param p parent element to set as
             */
            void setParent(Element * p);

            /**
             * @brief Adds new element as current element's childrent element
             * 
             * @param e Children element to add
             */
            virtual void addElement(Element * e);

            /**
             * @brief Attempt to remove children element from current element
             * 
             * @param e children to delete
             * @return true if deleted successfully
             * @return false if not a child of element
             */
            virtual bool removeElement(Element * e);

            /**
             * @brief Remove all children elements associated with current element
             */
            virtual void removeAllElements();

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
             * @brief Set the Has Selected object
             * 
             * @param b 
             */
            void setHasSelected(bool b);

            /**
             * @brief Set the Active object
             * 
             */
            virtual void setActive();

            /**
             * @brief Set the Inactive object
             * 
             */
            virtual void setInactive();

            /**
             * @brief Set the Focused object
             * 
             * @param e 
             */
            void setFocussed(Element * e);

            /**
             * @brief 
             * 
             * @return Element* 
             */
            Element * focussed();

            // Returns callback function (nullptr if no callback assigned)
            /**
             * @brief 
             * 
             * @return std::function<void()> 
             */
            std::function<void()> callback();

            // Set callback function (also marks element as selectable)
            /**
             * @brief Set the Callback object
             * 
             * @param f 
             */
            void setCallback(std::function<void()> f);

            // Handle the given event
            /**
             * @brief 
             * 
             * @param e 
             * @return true 
             * @return false 
             */
            virtual bool handleEvent(InputEvent * e);

            // Update is passed time since last frame (for animations)
            /**
             * @brief 
             * 
             * @param dt 
             */
            virtual void update(uint32_t dt);

            // Render child elements
            /**
             * @brief 
             * 
             */
            virtual void render();

            // Default are rectangles
            // Renders the highlight border + background
            /**
             * @brief 
             * 
             */
            virtual void renderHighlighted();

            // Renders the selected overlay
            /**
             * @brief 
             * 
             */
            virtual void renderSelected();

            // Destructor calls destructor of children
            /**
             * @brief Destroy the Element object
             * 
             */
            virtual ~Element();

            // 
            // or nullptr if none found
            /**
             * @brief Returns the element currently highlighted within given element
             * 
             * @param e 
             */
            friend void moveHighlight(Element * e);
    };
};

#endif