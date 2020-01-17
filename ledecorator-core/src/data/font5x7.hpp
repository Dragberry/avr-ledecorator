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

    static const BitMap<35>* const PROGMEM DATA[18];
};

#endif
