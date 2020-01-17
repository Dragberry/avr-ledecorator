#ifndef FONT3X5_HPP_
#define FONT3X5_HPP_

#include <avr/pgmspace.h>
#include <stdint.h>
#include "bitmap.hpp"

class Font3x5
{
private:
    static const uint8_t FIRST_INDEX = 43;
    static const uint8_t LAST_INDEX = 59;

public:
    static const BitMap<15>* get_char(const char char_index)
    {
        uint8_t real_index = char_index >= FIRST_INDEX ? char_index - FIRST_INDEX + 1 : 0;
        return (BitMap<15>*) pgm_read_ptr(
                &(DATA[char_index > LAST_INDEX ? 0 : real_index]));
    }

private:
    static const BitMap<15>PROGMEM CHAR3x5_SPACE;

    static const BitMap<15>PROGMEM CHAR3x5_PLUS;

    static const BitMap<15>PROGMEM CHAR3x5_COMMA;

    static const BitMap<15>PROGMEM CHAR3x5_MINUS;

    static const BitMap<15>PROGMEM CHAR3x5_DOT;

    static const BitMap<15>PROGMEM CHAR3x5_SLASH;

    static const BitMap<15> PROGMEM CHAR3x5_0;

    static const BitMap<15>PROGMEM CHAR3x5_1;

    static const BitMap<15>PROGMEM CHAR3x5_2;

    static const BitMap<15>PROGMEM CHAR3x5_3;

    static const BitMap<15>PROGMEM CHAR3x5_4;

    static const BitMap<15>PROGMEM CHAR3x5_5;

    static const BitMap<15>PROGMEM CHAR3x5_6;

    static const BitMap<15>PROGMEM CHAR3x5_7;

    static const BitMap<15>PROGMEM CHAR3x5_8;

    static const BitMap<15>PROGMEM CHAR3x5_9;

    static const BitMap<15>PROGMEM CHAR3x5_COLON;

    static const BitMap<15>PROGMEM CHAR3x5_SEMICOLON;

    static const BitMap<15>* const PROGMEM DATA[18];
};

#endif
