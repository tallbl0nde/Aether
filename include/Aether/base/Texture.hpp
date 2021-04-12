#ifndef AETHER_TEXTURE_HPP
#define AETHER_TEXTURE_HPP

#include "Aether/base/Element.hpp"
#include <atomic>

namespace Aether {
    /**
     * @brief Supported "on create" texture rendering options.
     */
    enum class Render {
        Sync,           /**< Render texture synchronously (i.e. as soon as created) */
        Async,          /**< Render texture asynchronously on another thread */
        Wait,           /**< Don't automatically render the texture */
    };

    /**
     * @brief A Texture is an element that stores a texture to render
     * in the window. It can't be instantiated alone as it's essentially
     * a 'container' element, but is inherited by classes such as \ref Text
     * and \ref Image which generate the texture. This class provides common
     * getter methods and handles rendering texture asynchronously.
     */
    class Texture : public Element {
        private:
            // Forward declare nested class
            class RenderJob;

            /**
             * @brief Possible statuses for the texture
             * to be in when rendering asynchronously.
             */
            enum class AsyncStatus {
                Waiting,                            /**< Waiting for start signal/rendered synchronously */
                Rendering,                          /**< Task has been queued/is being executed */
                NeedsConvert,                       /**< Task is finished, need to convert to texture */
                Done                                /**< Everything is done; the texture can be shown */
            };

            int asyncID;                            /** @brief ID of async task */
            std::function<void()> callback;         /** @brief Callback to invoke when rendering is complete */
            std::atomic<AsyncStatus> status;        /** @brief Current status of texture */

            Colour colour_;                         /** @brief Colour to tint texture with */
            Drawable * drawable;                    /** @brief The texture to draw on screen */
            Drawable * tmpDrawable;                 /** @brief Pointer temporarily storing rendered drawable */

            /**
             * @brief Small helper to set up a new drawable.
             */
            void setupDrawable();

        protected:
            /**
             * @brief Method provided by children defining how to render the texture.
             */
            virtual Drawable * renderDrawable() = 0;

        public:
            /**
             * @brief Constructs a new texture element. Position defaults to (0, 0).
             *
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             */
            Texture(const int x = 0, const int y = 0);

            /**
             * @brief Set a callback to be invoked when rendering is complete. An example
             * use for this is to resize/position based on the texture's size. Pass nullptr
             * to remove.
             *
             * @param func Callback function
             */
            void setRenderCallback(const std::function<void()> func);

            /**
             * @brief Returns the texture's tint colour.
             *
             * @return Colour texture is tinted with.
             */
            Colour colour();

            /**
             * @brief Set the colour to tint the texture with.
             *
             * @param col Colour to tint with
             */
            void setColour(const Colour & col);

            /**
             * @brief Returns the width of the stored texture. Returns 0 if the texture
             * is being rendered asynchronously and isn't finished.
             *
             * @return Stored texture's width in pixels.
             */
            int textureWidth();

            /**
             * @brief Returns the height of the stored texture. Returns 0 if the texture
             * is being rendered asynchronously and isn't finished.
             *
             * @return Stored texture's height in pixels.
             */
            int textureHeight();

            /**
             * @brief Set the mask area for the texture. Pixels outside of this area
             * are not drawn.
             * @note This must be called after the texture is ready (see \ref ready()), and is lost
             * whenever the texture is changed.
             *
             * @param x Top-left x coordinate of mask
             * @param y Top-left y coordinate of mask
             * @param w Mask width
             * @param h Mask height
             */
            void setMask(const int x, const int y, const unsigned int w, const unsigned int h);

            /**
             * @brief Destroy the stored texture, freeing memory. Safe to call even if no texture is stored.
             * @note This will block if the texture isn't finished rendering.
             */
            void destroy();

            /**
             * @brief Returns whether the texture is finished rendering and ready to be shown.
             *
             * @return Whether the texture can be rendered on the next frame.
             */
            bool ready();

            /**
             * @brief Immediately render the texture synchronously. This has no
             * effect if a texture is currently stored, or a task is already queued. To
             * recreate the texture, call \ref destroy() first.
             */
            void renderSync();

            /**
             * @brief Request to start rendering the texture asynchronously. This has no
             * effect if a texture is currently stored, or a task is already queued. To
             * recreate the texture, call \ref destroy() first.
             */
            void renderAsync();

            /**
             * @brief Called internally. Overrides Element's update method to handle
             * the asynchronous rendering operations.
             *
             * @param dt Delta time since last frame in ms
             */
            void update(unsigned int dt);

            /**
             * @brief Called internally. Overrides Element's render method to actually
             * show the stored texture.
             */
            void render();

            /**
             * @brief Destroys the texture, freeing all allocated memory.
             */
            ~Texture();
    };
};

#endif