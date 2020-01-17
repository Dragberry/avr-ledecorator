#include "font5x7.hpp"

const BitMap<35> PROGMEM Font5x7::CHAR5x7_SPACE =
{
        3, 7,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_PLUS =
{
        3, 7,
        0b00000001,
        0b01110100,
        0b00000000,
        0b00000000,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_COMMA =
{
        3, 7,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_MINUS =
{
        3, 7,
        0b00000000,
        0b01110000,
        0b00000000,
        0b00000000,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_DOT =
{
        1, 7,
        0b00000010,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_SLASH =
{
        5, 7,
        0b00000000,
        0b01000000,
        0b10000001,
        0b00000010,
        0b00000100,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_0 =
{
        5, 7,
        0b01110100,
        0b01100111,
        0b01011100,
        0b11000101,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_1 =
{
        5, 7,
        0b00100011,
        0b00001000,
        0b01000010,
        0b00010001,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_2 =
{
        5, 7,
        0b01110100,
        0b01000010,
        0b01100100,
        0b01000011,
        0b11100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_3 =
{
        5, 7,
        0b11111000,
        0b01000100,
        0b01100000,
        0b11000101,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_4 =
{
        5, 7,
        0b00010001,
        0b10010101,
        0b00101111,
        0b10001000,
        0b01000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_5 =
{
        5, 7,
        0b11111100,
        0b00111100,
        0b00010000,
        0b11000101,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_6 =
{
        5, 7,
        0b00111010,
        0b00100001,
        0b11101000,
        0b11000101,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_7 =
{
        5, 7,
        0b11111000,
        0b01000100,
        0b01000100,
        0b00100001,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_8 =
{
        5, 7,
        0b01110100,
        0b01100010,
        0b11101000,
        0b11000101,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_9 =
{
        5, 7,
        0b01110100,
        0b01100010,
        0b11110000,
        0b10001011,
        0b10000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_COLON =
{
        1, 7,
        0b00101000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_SEMICOLON =
{
        2, 7,
        0b00001000,
        0b10110000,
        0b00000000,
        0b00000000,
        0b00000000,
};

const BitMap<35>* const PROGMEM Font5x7::DATA[18] =
{
        &CHAR5x7_SPACE,
        &CHAR5x7_PLUS,
        &CHAR5x7_COMMA,
        &CHAR5x7_MINUS,
        &CHAR5x7_DOT,
        &CHAR5x7_SLASH,

        &CHAR5x7_0,
        &CHAR5x7_1,
        &CHAR5x7_2,
        &CHAR5x7_3,
        &CHAR5x7_4,
        &CHAR5x7_5,
        &CHAR5x7_6,
        &CHAR5x7_7,
        &CHAR5x7_8,
        &CHAR5x7_9,

        &CHAR5x7_COLON,
        &CHAR5x7_SEMICOLON,
};


