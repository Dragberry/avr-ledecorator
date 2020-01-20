#ifndef SENSORSTORAGE_HPP_
#define SENSORSTORAGE_HPP_

#include <stdlib.h>
#include "../../util/ringbuffer.hpp"

class SensorStorage
{
private:
    const uint16_t period = 3600;

    uint32_t last_updated_time;

    RingBuffer<int16_t, 6> data;

public:
    uint32_t* get_last_updated_time();

    RingBuffer<int16_t, 6>* get_data();

    bool add(int16_t value, uint32_t timestamp);

    void reset();

    uint8_t get_size();
};

#endif
