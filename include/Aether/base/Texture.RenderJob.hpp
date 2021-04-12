#ifndef AETHER_TEXTURE_RENDERJOB_HPP
#define AETHER_TEXTURE_RENDERJOB_HPP

#include "Aether/base/Texture.hpp"
#include "Aether/ThreadPool.Job.hpp"

namespace Aether {
    /**
     * @brief Extends a thread pool job to render the given texture element
     * on a separate thread.
     */
    class Texture::RenderJob : public ThreadPool::Job {
        private:
            /**
             * @brief Texture object to operate on.
             */
            Texture * texture;

            /**
             * @brief Implements \ref ThreadPool::Job::work() to render the texture.
             */
            void work();

        public:
            /**
             * @brief Constructs a new Render job.
             *
             * @param texture Texture to operate on
             */
            RenderJob(Texture * texture);
    };
};

#endif