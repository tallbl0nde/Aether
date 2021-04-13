#ifndef AETHER_COLOUR_HPP
#define AETHER_COLOUR_HPP

namespace Aether {
    /**
     * @brief Stores RGBA values representing a colour. Each component can be within
     * the range 0 to 255 (inclusive).
     */
    class Colour {
        private:
            unsigned char red;              /** @brief Red component */
            unsigned char green;            /** @brief Green component */
            unsigned char blue;             /** @brief Blue component */
            unsigned char alpha;            /** @brief Alpha component */

        public:
            /**
             * @brief Default constructor, initializes to opaque white.
             */
            Colour();

            /**
             * @brief Constructor initializes colour with passed components.
             *
             * @param r red component
             * @param g green component
             * @param b blue component
             * @param a alpha component
             */
            Colour(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);

            /**
             * @brief Return the red component
             *
             * @return Red component of colour
             */
            unsigned char r() const;

            /**
             * @brief Return the green component
             *
             * @return Green component of colour
             */
            unsigned char g() const;

            /**
             * @brief Return the blue component
             *
             * @return Blue component of colour
             */
            unsigned char b() const;

            /**
             * @brief Return the alpha component
             *
             * @return Alpha component of colour
             */
            unsigned char a() const;

            /**
             * @brief Set the red component of the colour
             *
             * @param r Red component of colour
             */
            void setR(const unsigned char r);


            /**
             * @brief Set the green component of the colour
             *
             * @param g Green component of colour
             */
            void setG(const unsigned char g);


            /**
             * @brief Set the blue component of the colour
             *
             * @param b Blue component of colour
             */
            void setB(const unsigned char b);


            /**
             * @brief Set the alpha component of the colour
             *
             * @param a Alpha component of colour
             */
            void setA(const unsigned char a);

            /**
             * @brief Set all components
             *
             * @param r Red component
             * @param g Green component
             * @param b Blue component
             * @param a Alpha component
             */
            void setColour(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    };
};

#endif