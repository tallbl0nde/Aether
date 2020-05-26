#include <filesystem>
#include "Aether/utils/Utils.hpp"

namespace Aether::Utils {
    bool fileExists(std::string path) {
        return std::filesystem::exists(path);
    }

    Button SDLtoButton(uint8_t k) {
        return (Button)k;
    }
};