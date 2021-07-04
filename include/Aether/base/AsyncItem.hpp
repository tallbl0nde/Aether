#ifndef BASE_ASYNCITEM_HPP
#define BASE_ASYNCITEM_HPP

#include "Aether/base/Texture.hpp"

namespace Aether {
    /**
     * @brief Abstract class which handles rendering child elements
     * asynchronously based on the position of the element itself.
     * Before/after the element comes into view the textures will be
     * created/destroyed respectively. This can be set by overriding
     * \ref renderThreshold().
     */
    class AsyncItem : public Element {
        private:
            /**
             * @brief Status of child textures.
             */
            enum class Status {
                NotRendered,    /**< Textures aren't rendered*/
                Rendering,      /**< Textures are rendering */
                Rendered        /**< Textured are rendered*/
            };

            /**
             * @brief Current fade progress.
             */
            float fadeAmount;

            /**
             * @brief Current rendering status.
             */
            Status renderStatus;

            /**
             * @brief Vector of textures to render.
             */
            std::vector<Texture *> textures;

            /**
             * @brief Optional method which can be overidden in order to return
             * whether the item is allowed to render it's textures (once it is
             * within the threshold). Defaults to always returning true. An example
             * usage is that this method returns false until a string has been set,
             * after which it returns true.
             */
            virtual bool canRender();

            /**
             * @brief Number of milliseconds the item will take to completely
             * fade in once rendered. Defaults to 150ms.
             * @return Number of milliseconds to fade in.
             */
            virtual int fadeSpeed();

            /**
             * @brief Abstract method which should position all child elements
             * as required. Called once all textures have finished rendering or
             * when the dimensions of the element have changed (if textures are
             * rendered).
             */
            virtual void positionElements() = 0;

            /**
             * @brief Returns the number of pixels away from the item's parent
             * which is considered the 'threshold'. When an item is beyond
             * the threshold it's textures will be destroyed; when an item
             * is within the threshold it's textures will be rendered.
             * Defaults to 1000 pixels.
             * @return Size of threshold, in pixels.
             */
            virtual int renderThreshold();

            /**
             * @brief Returns whether the item is within the renderThreshold with
             * regards to it's parent's location.
             * @return true if within threshold, false otherwise
             */
            bool withinThreshold();

        protected:
            /**
             * @brief Adds the given texture to the item's monitoring list,
             * causing it to be rendered/destroyed when needed automatically.
             * @note This does not replace \ref addElement().
             * @param texture The texture to monitor/render
             */
            void addTexture(Aether::Texture * texture);

            /**
             * @brief Removes the given texture from the item's monitoring list,
             * causing it to no longer be handled by the element.
             * @note This will not stop any rendering/destruction operations
             * already being performed on the texture. This does not replace
             * \ref removeElement().
             * @param texture The texture to stop monitoring
             * @return true if texture was being monitored, false otherwise
             */
            bool removeTexture(Aether::Texture * texture);

        public:
            /**
             * @brief Creates a new AsyncItem.
             */
            AsyncItem();

            /**
             * @brief Updates the state of child elements.
             * @param dt time since last call in milliseconds.
             */
            void update(uint32_t dt);

            /**
             * @brief Sets the width of the item, in pixels.
             * Also calls \ref updateElements() if textures are already
             * rendered.
             * @param w Width in pixels
             */
            void setW(int w);

            /**
             * @brief Sets the height of the item, in pixels.
             * Also calls \ref updateElements() if textures are already
             * rendered.
             * @param h Height in pixels
             */
            void setH(int h);

            /**
             * @brief Destroys the AsyncItem.
             */
            ~AsyncItem();
    };
};

#endif
