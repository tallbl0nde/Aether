#include "Aether/base/Texture.hpp"
#include "Aether/base/Texture.RenderJob.hpp"

namespace Aether {
    Texture::RenderJob::RenderJob(Texture * texture) : Job() {
        this->texture = texture;
    }

    void Texture::RenderJob::work() {
        this->texture->tmpDrawable = this->texture->renderDrawable();
        this->texture->status = AsyncStatus::NeedsConvert;
    }
}