#ifndef FONT5X7_HPP_
#define FONT5X7_HPP_

#include <avr/pgmspace.h>
#include <stdint.h>
#include "bitmap.hpp"

class Font5x7
{
private:
    static const uint8_t FIRST_INDEX = 43;
    static const uint8_t LAST_INDEX = 59;

public:
    static const BitMap<35>* get_char(const char char_index)
    {
        uint8_t real_index = char_index >= FIRST_INDEX ? char_index - FIRST_INDEX + 1 : 0;
        return (BitMap<35>*) pgm_read_ptr(
                &(DATA[char_index > LAST_INDEX ? 0 : real_index]));
    }
private:
    static const BitMap<35>PROGMEM CHAR5x7_SPACE;

    static const BitMap<35>PROGMEM CHAR5x7_PLUS;

    static const BitMap<35>PROGMEM CHAR5x7_COMMA;

    static const BitMap<35>PROGMEM CHAR5x7_MINUS;

    static const BitMap<35>PROGMEM CHAR5x7_DOT;

    static const BitMap<35>PROGMEM CHAR5x7_SLASH;

    static const BitMap<35> PROGMEM CHAR5x7_0;

    static const BitMap<35>PROGMEM CHAR5x7_1;

    static const BitMap<35>PROGMEM CHAR5x7_2;

    static const BitMap<35>PROGMEM CHAR5x7_3;

    static const BitMap<35>PROGMEM CHAR5x7_4;

    static const BitMap<35>PROGMEM CHAR5x7_5;

    static const BitMap<35>PROGMEM CHAR5x7_6;

    static const BitMap<35>PROGMEM CHAR5x7_7;

    static const BitMap<35>PROGMEM CHAR5x7_8;

    static const BitMap<35>PROGMEM CHAR5x7_9;

    static const BitMap<35>PROGMEM CHAR5x7_COLON;

    static const BitMap<35>PROGMEM CHAR5x7_SEMICOLON;
    
    static const BitMap<35> PROGMEM CHAR5x7_ANGLE_BRACKET_OPEN;

    static const BitMap<35> PROGMEM CHAR5x7_EQ;

    static const BitMap<35> PROGMEM CHAR5x7_ANGLE_BRACKET_CLOSE;

    static const BitMap<35> PROGMEM CHAR5x7_QUESTION;

    static const BitMap<35> PROGMEM CHAR5x7_MAIL;

    static const BitMap<35> PROGMEM CHAR5x7_A;

    static const BitMap<35> PROGMEM CHAR5x7_B;

    static const BitMap<35> PROGMEM CHAR5x7_C;

    static const BitMap<35> PROGMEM CHAR5x7_D;

    static const BitMap<35> PROGMEM CHAR5x7_E;

    static const BitMap<35> PROGMEM CHAR5x7_F;

    static const BitMap<35> PROGMEM CHAR5x7_G;

    static const BitMap<35> PROGMEM CHAR5x7_H;

    static const BitMap<35> PROGMEM CHAR5x7_I;

    static const BitMap<35> PROGMEM CHAR5x7_J;

    static const BitMap<35> PROGMEM CHAR5x7_K;

    static const BitMap<35> PROGMEM CHAR5x7_L;

    static const BitMap<35> PROGMEM CHAR5x7_M;

    static const BitMap<35> PROGMEM CHAR5x7_N;

    static const BitMap<35> PROGMEM CHAR5x7_O;

    static const BitMap<35> PROGMEM CHAR5x7_P;

    static const BitMap<35> PROGMEM CHAR5x7_Q;

    static const BitMap<35> PROGMEM CHAR5x7_R;

    static const BitMap<35> PROGMEM CHAR5x7_S;

    static const BitMap<35> PROGMEM CHAR5x7_T;

    static const BitMap<35> PROGMEM CHAR5x7_U;

    static const BitMap<35> PROGMEM CHAR5x7_V;

    static const BitMap<35> PROGMEM CHAR5x7_W;

    static const BitMap<35> PROGMEM CHAR5x7_X;

    static const BitMap<35> PROGMEM CHAR5x7_Y;

    static const BitMap<35> PROGMEM CHAR5x7_Z;

    static const BitMap<35>* const PROGMEM DATA[91];
};

#endif
