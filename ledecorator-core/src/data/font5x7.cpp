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

const BitMap<35> PROGMEM Font5x7::CHAR5x7_ANGLE_BRACKET_OPEN
{
        5, 7,
        0b00010001,
        0b00010001,
        0b00000100,
        0b00010000,
        0b01000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_EQ
{
        5, 7,
        0b00000000,
        0b00111110,
        0b00001111,
        0b10000000,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_ANGLE_BRACKET_CLOSE
{
        5, 7,
        0b01000001,
        0b00000100,
        0b00010001,
        0b00010001,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_QUESTION
{
        5, 7,
        0b01110100,
        0b01000100,
        0b01000010,
        0b00000000,
        0b10000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_MAIL
{
        5, 7,
        0b01110100,
        0b01101011,
        0b01111011,
        0b01000001,
        0b11100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_A
{
        5, 7,
        0b00100010,
        0b10100011,
        0b00011111,
        0b11000110,
        0b00100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_B
{
        5, 7,
        0b11110100,
        0b01100011,
        0b11101000,
        0b11000111,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_C
{
        5, 7,
        0b01110100,
        0b01100001,
        0b00001000,
        0b01000101,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_D
{
        5, 7,
        0b11110100,
        0b01100011,
        0b00011000,
        0b11000111,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_E
{
        5, 7,
        0b11111100,
        0b00100001,
        0b11101000,
        0b01000011,
        0b11100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_F
{
        5, 7,
        0b11111100,
        0b00100001,
        0b11101000,
        0b01000010,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_G
{
        5, 7,
        0b01111100,
        0b00100001,
        0b00001001,
        0b11000101,
        0b11100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_H
{
        5, 7,
        0b10001100,
        0b01100011,
        0b11111000,
        0b11000110,
        0b00100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_I
{
        5, 7,
        0b01110001,
        0b00001000,
        0b01000010,
        0b00010001,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_J
{
        5, 7,
        0b00001000,
        0b01000010,
        0b00010000,
        0b11000101,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_K
{
        5, 7,
        0b10001100,
        0b10101001,
        0b10001010,
        0b01001010,
        0b00100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_L
{
        5, 7,
        0b10000100,
        0b00100001,
        0b00001000,
        0b01000011,
        0b11100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_M
{
        5, 7,
        0b10001110,
        0b11101011,
        0b01011000,
        0b11000110,
        0b00100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_N
{
        5, 7,
        0b10001100,
        0b01110011,
        0b01011001,
        0b11000110,
        0b00100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_O
{
        5, 7,
        0b01110100,
        0b01100011,
        0b00011000,
        0b11000101,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_P
{
        5, 7,
        0b11110100,
        0b01100011,
        0b00011111,
        0b01000010,
        0b00000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_Q
{
        5, 7,
        0b01110100,
        0b01100011,
        0b00011010,
        0b11001001,
        0b10100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_R
{
        5, 7,
        0b11111100,
        0b01100011,
        0b11101010,
        0b01001010,
        0b00100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_S
{
        5, 7,
        0b01110100,
        0b01100000,
        0b11100000,
        0b11000101,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_T
{
        5, 7,
        0b11111001,
        0b00001000,
        0b01000010,
        0b00010000,
        0b10000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_U
{
        5, 7,
        0b10001100,
        0b01100011,
        0b00011000,
        0b11000101,
        0b11000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_V
{
        5, 7,
        0b10001100,
        0b01100011,
        0b00011000,
        0b10101000,
        0b10000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_W
{
        5, 7,
        0b10001100,
        0b01100011,
        0b01011010,
        0b11101110,
        0b00100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_X
{
        5, 7,
        0b10001100,
        0b01010100,
        0b01000101,
        0b01000110,
        0b00100000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_Y
{
        5, 7,
        0b10001100,
        0b01010100,
        0b01000010,
        0b00010000,
        0b10000000,
};

const BitMap<35> PROGMEM Font5x7::CHAR5x7_Z
{
        5, 7,
        0b11111000,
        0b01000100,
        0b01000100,
        0b01000011,
        0b11100000,
};

const BitMap<35>* const PROGMEM Font5x7::DATA[23] =
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
        &CHAR5x7_ANGLE_BRACKET_OPEN,
        &CHAR5x7_CHAR_EQ,
        &CHAR5x7_ANGLE_BRACKET_CLOSE,
        &CHAR5x7_QUESTION,
        &CHAR5x7_MAIL,
        
        &CHAR5X7_A,
        &CHAR5X7_B,
        &CHAR5X7_C,
        &CHAR5X7_D,
        &CHAR5X7_E,
        &CHAR5X7_F,
        &CHAR5X7_G,
        &CHAR5X7_H,
        &CHAR5X7_I,
        &CHAR5X7_J,
        &CHAR5X7_K,
        &CHAR5X7_L,
        &CHAR5X7_M,
        &CHAR5X7_N,
        &CHAR5X7_O,
        &CHAR5X7_P,
        &CHAR5X7_Q,
        &CHAR5X7_R,
        &CHAR5X7_S,
        &CHAR5X7_T,
        &CHAR5X7_U,
        &CHAR5X7_V,
        &CHAR5X7_W,
        &CHAR5X7_X,
        &CHAR5X7_Y,
        &CHAR5X7_Z
};


