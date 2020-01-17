#include "entities.hpp"

const BitMap<9> PROGMEM Entities::GLIDER =
{
        3, 3,
       /*
        * 111
        * 001
        * 010
        */
       0b11100101,
       0b00000000,
};

const BitMap<20> PROGMEM Entities::SHIP_LIGHT =
{
        5, 4,
        /*
         * 10010
         * 00001
         * 10001
         * 01111
         */
        0b10010000,
        0b01100010,
        0b11110000
};

const BitMap<30> PROGMEM Entities::SHIP_MEDIUM =
{
        6, 5,
        /*
         * 001000
         * 100010
         * 000001
         * 100001
         * 011111
         */
        0b00100010,
        0b00100000,
        0b01100001,
        0b01111100,
};

const BitMap<35> PROGMEM Entities::SHIP_LARGE =
{
       7, 5,
       /*
        * 0011000
        * 1000010
        * 0000001
        * 1000001
        * 1111111
        */
       0b00110001,
       0b00001000,
       0b00001100,
       0b00010111,
       0b11100000,
};

const BitMap<96> PROGMEM Entities::COPPERHEAD =
{
        12, 8,
        /*
         * 000001011000
         * 000010000001
         * 000110001001
         * 110100000110
         * 110100000110
         * 000110001001
         * 000010000001
         * 000001011000
         */
        0b00000101,
        0b10000000,
        0b10000001,
        0b00011000,
        0b10011101,
        0b00000110,
        0b11010000,
        0b01100001,
        0b10001001,
        0b00001000,
        0b00010000,
        0b01011000,
};
