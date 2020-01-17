#ifndef ENTITIES_HPP_
#define ENTITIES_HPP_

#include "../../../data/bitmap.hpp"

class Entities
{
public:
    static const BitMap<9> PROGMEM GLIDER;

    static const BitMap<20> PROGMEM SHIP_LIGHT;

    static const BitMap<30> PROGMEM SHIP_MEDIUM;

    static const BitMap<35> PROGMEM SHIP_LARGE;

    static const BitMap<96> PROGMEM COPPERHEAD;
};

#endif
