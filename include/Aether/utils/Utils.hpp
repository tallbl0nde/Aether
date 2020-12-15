#ifndef AETHER_UTILS_HPP
#define AETHER_UTILS_HPP

#include "Aether/utils/Types.hpp"
#include <cstddef>
#include <string>

/** @brief Fake controller ID for held events */
const short int FAKE_ID = 99;

namespace Aether::Utils {
    /**
     * @brief Returns a string containing a suitable character to represent the passed \ref Aether::Button
     *
     * @param button \ref Aether::Button to get character for
     * @return string containing character matching \ref Aether::Button
     */
    std::string buttonToCharacter(const Button button);

    /**
     * @brief Checks if file exists at location
     *
     * @param path path to file to check
     * @return true if file exists
     * @return false if doesn't file exists
     */
    bool fileExists(std::string path);

    /**
     * @brief Extracts one UTF8 character from the passed string
     *
     * @param str String to examine
     * @param pos Position of first character/byte to examine. Updated to the index of
     * the first character/byte after the returned character.
     * @return UTF8 character at required position, or zero otherwise.
     */
    uint16_t getUTF8Char(const std::string & str, unsigned int & pos);

    /**
     * @brief Returns a button equivalent for SDL button value
     *
     * @param k SDL button value
     * @return Button equivalent from SDL button value
     */
    Button SDLtoButton(uint8_t k);
};

#endif