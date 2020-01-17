#ifndef FONT3X5_HPP_
#define FONT3X5_HPP_

#include <avr/pgmspace.h>
#include <stdint.h>
#include "bitmap.hpp"

class Font3x5
{
private:
    static const uint8_t FIRST_INDEX = 43;
    static const uint8_t LAST_INDEX = 92;

public:
    static const BitMap<15>* get_char(const char char_index)
    {
        uint8_t real_index = char_index >= FIRST_INDEX ? char_index - FIRST_INDEX + 1 : 0;
        return (BitMap<15>*) pgm_read_ptr(
                &(DATA[char_index > LAST_INDEX ? 0 : real_index]));
    }

private:
    static const BitMap<15> PROGMEM CHAR3X5_SPACE;

    static const BitMap<15> PROGMEM CHAR3X5_PLUS;

    static const BitMap<15> PROGMEM CHAR3X5_COMMA;

    static const BitMap<15> PROGMEM CHAR3X5_MINUS;

    static const BitMap<15> PROGMEM CHAR3X5_DOT;

    static const BitMap<15> PROGMEM CHAR3X5_SLASH;

    static const BitMap<15> PROGMEM CHAR3X5_0;

    static const BitMap<15> PROGMEM CHAR3X5_1;

    static const BitMap<15> PROGMEM CHAR3X5_2;

    static const BitMap<15> PROGMEM CHAR3X5_3;

    static const BitMap<15> PROGMEM CHAR3X5_4;

    static const BitMap<15> PROGMEM CHAR3X5_5;

    static const BitMap<15> PROGMEM CHAR3X5_6;

    static const BitMap<15> PROGMEM CHAR3X5_7;

    static const BitMap<15> PROGMEM CHAR3X5_8;

    static const BitMap<15> PROGMEM CHAR3X5_9;

    static const BitMap<15> PROGMEM CHAR3X5_COLON;

    static const BitMap<15> PROGMEM CHAR3X5_SEMICOLON;
    
    static const BitMap<15> PROGMEM CHAR3X5_ANGLE_BRACKET_OPEN;

    static const BitMap<15> PROGMEM CHAR3X5_EQ;

    static const BitMap<15> PROGMEM CHAR3X5_ANGLE_BRACKET_CLOSE;

    static const BitMap<15> PROGMEM CHAR3X5_QUESTION;

    static const BitMap<15> PROGMEM CHAR3X5_MAIL;

    static const BitMap<15> PROGMEM CHAR3X5_A;

    static const BitMap<15> PROGMEM CHAR3X5_B;

    static const BitMap<15> PROGMEM CHAR3X5_C;

    static const BitMap<15> PROGMEM CHAR3X5_D;

    static const BitMap<15> PROGMEM CHAR3X5_E;

    static const BitMap<15> PROGMEM CHAR3X5_F;

    static const BitMap<15> PROGMEM CHAR3X5_G;

    static const BitMap<15> PROGMEM CHAR3X5_H;

    static const BitMap<15> PROGMEM CHAR3X5_I;

    static const BitMap<15> PROGMEM CHAR3X5_J;

    static const BitMap<15> PROGMEM CHAR3X5_K;

    static const BitMap<15> PROGMEM CHAR3X5_L;

    static const BitMap<15> PROGMEM CHAR3X5_M;

    static const BitMap<15> PROGMEM CHAR3X5_N;

    static const BitMap<15> PROGMEM CHAR3X5_O;

    static const BitMap<15> PROGMEM CHAR3X5_P;

    static const BitMap<15> PROGMEM CHAR3X5_Q;

    static const BitMap<15> PROGMEM CHAR3X5_R;

    static const BitMap<15> PROGMEM CHAR3X5_S;

    static const BitMap<15> PROGMEM CHAR3X5_T;

    static const BitMap<15> PROGMEM CHAR3X5_U;

    static const BitMap<15> PROGMEM CHAR3X5_V;

    static const BitMap<15> PROGMEM CHAR3X5_W;

    static const BitMap<15> PROGMEM CHAR3X5_X;

    static const BitMap<15> PROGMEM CHAR3X5_Y;

    static const BitMap<15> PROGMEM CHAR3X5_Z;

    static const BitMap<15>* const PROGMEM DATA[49];
};

#endif
