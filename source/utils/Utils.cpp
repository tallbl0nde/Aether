#include <filesystem>
#include "Aether/utils/Utils.hpp"

namespace Aether::Utils {
    std::string buttonToCharacter(const Button b) {
        switch (b) {
            case Button::A:
                return "\uE0E0";
                break;

            case Button::B:
                return "\uE0E1";
                break;

            case Button::X:
                return "\uE0E2";
                break;

            case Button::Y:
                return "\uE0E3";
                break;

            case Button::LSTICK:
                return "\uE104";
                break;

            case Button::RSTICK:
                return "\uE105";
                break;

            case Button::L:
                return "\uE0E4";
                break;

            case Button::R:
                return "\uE0E5";
                break;

            case Button::ZL:
                return "\uE0E6";
                break;

            case Button::ZR:
                return "\uE0E7";
                break;

            case Button::PLUS:
                return "\uE0EF";
                break;

            case Button::MINUS:
                return "\uE0F0";
                break;

            case Button::DPAD_LEFT:
                return "\uE0ED";
                break;

            case Button::DPAD_UP:
                return "\uE0EB";
                break;

            case Button::DPAD_RIGHT:
                return "\uE0EE";
                break;

            case Button::DPAD_DOWN:
                return "\uE0EC";
                break;

            case Button::SL_LEFT:
            case Button::SR_LEFT:
                return "\uE0E8";
                break;

            case Button::SL_RIGHT:
            case Button::SR_RIGHT:
                return "\uE0E9";
                break;

            default:
                break;
        }
        return "";
    };

    bool fileExists(std::string path) {
        return std::filesystem::exists(path);
    }

    uint16_t getUTF8Char(const std::string & str, unsigned int & pos) {
        uint16_t c = 0;

        // One byte
        if ((str.length() - pos) >= 1) {
            if ((str[0 + pos] & 0b10000000) == 0) {
                c = str[0 + pos];
                pos += 1;
            }
        }

        // Two bytes
        if (c == 0 && (str.length() - pos) >= 2) {
            if ((str[0 + pos] & 0b11100000) == 0b11000000) {
                c = (str[0 + pos] & 0b00011111) << 6;
                c = (c | (str[1 + pos] & 0b00111111));
                pos += 2;
            }
        }

        // Three bytes
        if (c == 0 & (str.length() - pos) >= 3) {
            if ((str[0 + pos] & 0b11110000) == 0b11100000) {
                c = (str[0 + pos] & 0b00001111) << 12;
                c = (c | ((str[1 + pos] & 0b00111111) << 6));
                c = (c | (str[2 + pos] & 0b00111111));
                pos += 3;
            }
        }

        return c;
    }

    Button SDLtoButton(uint8_t k) {
        return (Button)k;
    }
};