#ifndef AETHER_WINDOW_HPP
#define AETHER_WINDOW_HPP

#include <queue>
#include <stack>

// Forward declare the typedef
namespace {
    typedef std::function<void(const std::string, const bool)> LogHandler;
};

// Forward declare pointers
namespace Aether {
    class Overlay;
    class Screen;
    class Timer;
}

namespace Aether {
    /**
     * @brief The Window is the root element, which initializes Aether and contains
     * all of the screens/elements to render. It also handles received events and
     * passes them down the tree.
     * @note Any added screens are not deleted, it is up to you to do so before
     * deleting the Window object.
     */
    class Window {
        public:
            /**
             * @brief Types of operations to perform on the screen stack
             */
            enum class ScreenOperation {
                Push,       /**< Push screen on the stack */
                Pop,        /**< Pop a screen from the stack */
                Set         /**< Set the screen as active */
            };

        private:
            /**
             * @brief Enumeration of valid fade actions
             */
            enum class FadeAction {
                None,       /**< Not fading */
                In,         /**< Fading in */
                Out         /**< Fading out */
            };

            /**
             * @brief Collection of variables used to fade the screen
             */
            struct FadeVars {
                FadeAction action;                                              /** @brief Current fade action (see \ref FadeAction) */
                short alpha;                                                    /** @brief Current fade alpha */
                bool in;                                                        /** @brief Should we fade in? */
                bool out;                                                       /** @brief Should we fade out? */
            } fade;

            double lastMillis;                                                  /** @brief Timestamp of last frame */
            bool shouldLoop;                                                    /** @brief Whether the window loop should continue */
            bool showDebug;                                                     /** @brief Whether to show the debug overlay */
            Timer * timer;                                                      /** @brief Timer measuring runtime of program */

            Colour bgColour;                                                    /** @brief Colour to clear screen with */
            Drawable * bgDrawable;                                              /** @brief Image to clear screen with */

            Button heldButton;                                                  /** @brief Button currently held */
            int heldTime;                                                       /** @brief Time (in ms) current button has been held for */
            int holdDelay_;                                                     /** @brief Time to wait before considering a press a hold */
            std::function<Colour(uint32_t)> highlight;                          /** @brief Function to return new highlight colour */

            std::vector<Overlay *> overlays;                                    /** @brief Vector of overlays to render, drawn from start to end */
            std::queue< std::pair<ScreenOperation, Screen *> > screenOps;       /** @brief Queue of operations to perform on screens at start of next loop */
            std::stack<Screen *> screenStack;                                   /** @brief Stack of 'pushed' screens */
            Screen * screen;                                                    /** @brief Current screen to render */

            /**
             * @brief Handle all requested operations from \ref pushScreen(), \ref popScreen() and
             * \ref showScreen().
             */
            void performScreenOps();

            /**
             * @brief Process SDL events
             */
            void processEvents();

            /**
             * @brief Render debug information
             *
             * @param delta Milliseconds since last frame
             */
            void renderDebug(const double delta);

            /**
             * @brief Render fade rectangle (if needed)
             *
             * @param delta Milliseconds since last frame
             */
            void renderFade(const double delta);

            /**
             * @brief Update state regarding the held button
             *
             * @param delta Milliseconds since last frame
             */
            void updateHeldButton(const double delta);

            /**
             * @brief Update each overlay's state
             *
             * @param delta Milliseconds since last frame
             */
            void updateOverlays(const double delta);

        public:
            /**
             * @brief Initializes an Aether instance. There should only ever be one Window
             * instantiated at once (ideally once for the entire program).
             * @note The construction parameters are ignored on some platforms.
             *
             * @param name Name to give window (optional)
             * @param width Width of window (optional)
             * @param height Height of window (optional)
             * @param log Handler to log messages (optional)
             */
            Window(const std::string & name, const unsigned int width, const unsigned int height, const LogHandler & log);

            /**
             * @brief Set whether debugging information is shown.
             *
             * @param show Whether to show info on screen.
             */
            void showDebugInfo(const bool show);

            /**
             * @brief Enable a fade in animation.
             *
             * @param fade Whether to show the fade animation on window load
             */
            void setFadeIn(const bool fade);

            /**
             * @brief Enable a fade out animation.
             *
             * @param fade Whether to show the fade animation on window exit
             */
            void setFadeOut(const bool fade);

            /**
             * @brief Set the colour used to clear the screen between frames.
             * The alpha channel is ignored.
             *
             * @param col Colour to fill screen with
             */
            void setBackgroundColour(const Colour & col);

            /**
             * @brief Set an image to use as the background. Pass an empty
             * string to remove.
             * @note An image always overrides the background colour.
             *
             * @param path Path to an image file.
             * @return Whether successful or not.
             */
            bool setBackgroundImage(const std::string & path);

            /**
             * @brief Set a custom font to use for text rendering. Pass an empty
             * string to revert back to internal fonts.
             *
             * @param path Path to a .ttf file
             */
            void setFont(const std::string & path);

            /**
             * @brief Set the spacing between wrapped lines of text.
             *
             * @param spacing Height of one line in terms of font size (1.1 by default).
             */
            void setFontSpacing(const double spacing);

            /**
             * @brief Set the function used to animate the highlight border.
             *
             * @param func Function which returns a colour given the runtime (in ms).
             */
            void setHighlightAnimation(const std::function<Colour(const uint32_t)> & func);

            /**
             * @brief Set the background colour used for highlighting an element.
             *
             * @param col Background highlight colour
             */
            void setHighlightBackground(const Colour & col);

            /**
             * @brief Set the colour used to highlight an element when selected.
             *
             * @param col Highlight colour
             */
            void setHighlightOverlay(const Colour & col);

            /**
             * @brief Returns the current value for 'hold delay' (see \ref setHoldDelay()).
             *
             * @return Milliseconds waited between firing duplicate button events
             */
            int holdDelay();

            /**
             * @brief Set the time to wait before firing duplicate button events due to
             * a button being held down.
             *
             * @param ms Event interval (in ms)
             */
            void setHoldDelay(const int ms);

            /**
             * @brief Add the given overlay to the window, placing it on top of existing ones.
             * @note Overlays aren't deleted when closed, that is your responsibility.
             *
             * @param ovl Overlay to show
             */
            void addOverlay(Overlay * ovl);

            /**
             * @brief Push the current screen onto the stack at the next loop
             * @note The pushed screen is not 'unloaded' (i.e. onUnload() is not called)
             */
            void pushScreen();

            /**
             * @brief Pop a screen from the stack at the next loop
             * @note The popped screen is not 'loaded' (i.e. onLoad() is not called)
             */
            void popScreen();

            /**
             * @brief Remove the screen set with \ref showScreen() *immediately* from the window.
             * This does not wait until the next loop, unlike other screen manipulation methods.
             */
            void removeScreen();

            /**
             * @brief Set the screen to show (takes effect at next loop). Ths does not
             * alter the screen stack in anyway; it simply replaces the current screen.
             *
             * @param screen Screen to show
             */
            void showScreen(Screen * screen);

            /**
             * @brief Handles rendering the current screen and events. This should be
             * called until it returns false.
             *
             * @return Whether Aether is 'running' (i.e. there is a screen to show and
             * \ref exit() hasn't been called).
             */
            bool loop();

            /**
             * @brief Stop the main \ref loop().
             */
            void exit();

            /**
             * @brief Cleans up Aether
             */
            ~Window();
    };
};

#endif