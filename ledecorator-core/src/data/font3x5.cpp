#include "font3x5.hpp"

const BitMap<15> PROGMEM Font3x5::CHAR3x5_SPACE =
{
        1, 5,
        0b00000000,
        0b00000000,
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_PLUS =
{
        3, 5,
        0b00001011,
        0b10100000
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_COMMA =
{
        2, 5,
        0b00000001,
        0b11000000
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_MINUS =
{
        3, 5,
        0b00000011,
        0b10000000
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_DOT =
{
        3, 5,
        0b00000000,
        0b00000100
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_SLASH =
{
        3, 5,
        0b00100101,
        0b01001000
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_0 =
{
        3, 5,
        0b11110110,
        0b11011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_1 =
{
        3, 5,
        0b01011001,
        0b00101110
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_2 =
{
        3, 5,
        0b11100111,
        0b11001110
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_3 =
{
        3, 5,
        0b11100111,
        0b10011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_4 =
{
        3, 5,
        0b10110111,
        0b10010010
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_5 =
{
        3, 5,
        0b11110011,
        0b10011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_6 =
{
        3, 5,
        0b11110011,
        0b11011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_7 =
{
        3, 5,
        0b11100100,
        0b10010010
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_8 =
{
        3, 5,
        0b11110111,
        0b11011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_9 =
{
        3, 5,
        0b11110111,
        0b10011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_COLON =
{
        1, 5,
        0b01010000,
        0b00000000,
};

const BitMap<15> PROGMEM Font3x5::CHAR3x5_SEMICOLON =
{
        2, 5,
        0b01010000,
        0b01000000,
};

const BitMap<15>* const PROGMEM Font3x5::DATA[18] =
{
        &CHAR3x5_SPACE,
        &CHAR3x5_PLUS,
        &CHAR3x5_COMMA,
        &CHAR3x5_MINUS,
        &CHAR3x5_DOT,
        &CHAR3x5_SLASH,

        &CHAR3x5_0,
        &CHAR3x5_1,
        &CHAR3x5_2,
        &CHAR3x5_3,
        &CHAR3x5_4,
        &CHAR3x5_5,
        &CHAR3x5_6,
        &CHAR3x5_7,
        &CHAR3x5_8,
        &CHAR3x5_9,

        &CHAR3x5_COLON,
        &CHAR3x5_SEMICOLON,
};
