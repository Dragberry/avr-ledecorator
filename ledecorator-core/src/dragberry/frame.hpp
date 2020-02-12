#ifndef FRAME_HPP_
#define FRAME_HPP_

#include "../util/ringbuffer.hpp"

class Frame : public RingBuffer<uint8_t, 20>
{
public:
    void add(const uint8_t item);

    uint8_t poll();
};

#endif
