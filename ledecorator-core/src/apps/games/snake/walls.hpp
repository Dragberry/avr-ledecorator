#ifndef WALLS_HPP_
#define WALLS_HPP_

#include "../../../data/bitmap.hpp"

class Walls
{
public:
    static const BitMap<8 * 8> PROGMEM CROSS;

    static const BitMap<12 * 8> PROGMEM TUNNEL;
};

#endif
