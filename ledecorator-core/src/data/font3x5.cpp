#include "font3x5.hpp"

const BitMap<15> PROGMEM Font3x5::CHAR3X5_SPACE =
{
        1, 5,
        0b00000000,
        0b00000000,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_PLUS =
{
        3, 5,
        0b00001011,
        0b10100000
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_COMMA =
{
        2, 5,
        0b00000001,
        0b10000000
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_MINUS =
{
        3, 5,
        0b00000011,
        0b10000000
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_DOT =
{
        3, 5,
        0b00000000,
        0b00000100
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_SLASH =
{
        3, 5,
        0b00000101,
        0b01000000
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_0 =
{
        3, 5,
        0b11110110,
        0b11011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_1 =
{
        3, 5,
        0b01011001,
        0b00101110
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_2 =
{
        3, 5,
        0b11100111,
        0b11001110
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_3 =
{
        3, 5,
        0b11100111,
        0b10011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_4 =
{
        3, 5,
        0b10110111,
        0b10010010
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_5 =
{
        3, 5,
        0b11110011,
        0b10011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_6 =
{
        3, 5,
        0b11110011,
        0b11011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_7 =
{
        3, 5,
        0b11100100,
        0b10010010
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_8 =
{
        3, 5,
        0b11110111,
        0b11011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_9 =
{
        3, 5,
        0b11110111,
        0b10011110
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_COLON =
{
        1, 5,
        0b01010000,
        0b00000000,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_SEMICOLON =
{
        2, 5,
        0b00010001,
        0b10000000,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_ANGLE_BRACKET_OPEN
{
        3, 5,
        0b00101010,
        0b00100010,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_EQ
{
        3, 5,
        0b00011100,
        0b01110000,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_ANGLE_BRACKET_CLOSE
{
        3, 5,
        0b10001000,
        0b10101000,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_QUESTION
{
        3, 5,
        0b11100101,
        0b10000100,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_MAIL
{
        3, 5,
        0b11110111,
        0b11001110,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_A
{
        3, 5,
        0b01010111,
        0b11011010,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_B
{
        3, 5,
        0b11010111,
        0b01011100,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_C
{
        3, 5,
        0b01110010,
        0b01000110,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_D
{
        3, 5,
        0b11010110,
        0b11011100,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_E
{
        3, 5,
        0b11110011,
        0b11001110,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_F
{
        3, 5,
        0b11110011,
        0b01001000,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_G
{
        3, 5,
        0b01110010,
        0b01010110,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_H
{
        3, 5,
        0b10110111,
        0b11011010,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_I
{
        3, 5,
        0b11101001,
        0b00101110,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_J
{
        3, 5,
        0b11101001,
        0b00101100,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_K
{
        3, 5,
        0b10110111,
        0b01011010,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_L
{
        3, 5,
        0b10010010,
        0b01001110,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_M
{
        3, 5,
        0b10111111,
        0b11011010,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_N
{
        3, 5,
        0b10111111,
        0b11111010,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_O
{
        3, 5,
        0b01010110,
        0b11010100,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_P
{
        3, 5,
        0b11010111,
        0b01001000,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_Q
{
        3, 5,
        0b11110110,
        0b11100010,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_R
{
        3, 5,
        0b11010111,
        0b01011010,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_S
{
        3, 5,
        0b01110001,
        0b00011100,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_T
{
        3, 5,
        0b11101001,
        0b00100100,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_U
{
        3, 5,
        0b10110110,
        0b11011110,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_V
{
        3, 5,
        0b10110110,
        0b11010100,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_W
{
        3, 5,
        0b10110111,
        0b11111010,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_X
{
        3, 5,
        0b10110101,
        0b01011010,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_Y
{
        3, 5,
        0b10110101,
        0b00100100,
};

const BitMap<15> PROGMEM Font3x5::CHAR3X5_Z
{
        3, 5,
        0b11100101,
        0b01001110,
};

const BitMap<15>* const PROGMEM Font3x5::DATA[49] =
{
        &CHAR3X5_SPACE,
        &CHAR3X5_PLUS,
        &CHAR3X5_COMMA,
        &CHAR3X5_MINUS,
        &CHAR3X5_DOT,
        &CHAR3X5_SLASH,
        &CHAR3X5_0,
        &CHAR3X5_1,
        &CHAR3X5_2,
        &CHAR3X5_3,
        &CHAR3X5_4,
        &CHAR3X5_5,
        &CHAR3X5_6,
        &CHAR3X5_7,
        &CHAR3X5_8,
        &CHAR3X5_9,
        &CHAR3X5_COLON,
        &CHAR3X5_SEMICOLON,
        &CHAR3X5_ANGLE_BRACKET_OPEN,
        &CHAR3X5_EQ,
        &CHAR3X5_ANGLE_BRACKET_CLOSE,
        &CHAR3X5_QUESTION,
        &CHAR3X5_MAIL,
        &CHAR3X5_A,
        &CHAR3X5_B,
        &CHAR3X5_C,
        &CHAR3X5_D,
        &CHAR3X5_E,
        &CHAR3X5_F,
        &CHAR3X5_G,
        &CHAR3X5_H,
        &CHAR3X5_I,
        &CHAR3X5_J,
        &CHAR3X5_K,
        &CHAR3X5_L,
        &CHAR3X5_M,
        &CHAR3X5_N,
        &CHAR3X5_O,
        &CHAR3X5_P,
        &CHAR3X5_Q,
        &CHAR3X5_R,
        &CHAR3X5_S,
        &CHAR3X5_T,
        &CHAR3X5_U,
        &CHAR3X5_V,
        &CHAR3X5_W,
        &CHAR3X5_X,
        &CHAR3X5_Y,
        &CHAR3X5_Z
};
