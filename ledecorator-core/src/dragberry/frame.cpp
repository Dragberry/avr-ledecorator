#include "frame.hpp"

void Frame::add(const uint8_t item)
{
    RingBuffer<uint8_t, 20>::add(item);
}

uint8_t Frame::poll()
{
    return RingBuffer<uint8_t, 20>::poll();
}

