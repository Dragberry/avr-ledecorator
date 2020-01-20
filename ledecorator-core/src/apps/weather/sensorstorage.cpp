#include "sensorstorage.hpp"

uint32_t* SensorStorage::get_last_updated_time()
{
    return &last_updated_time;
}

RingBuffer<int16_t, 6>* SensorStorage::get_data()
{
    return &data;
}

bool SensorStorage::add(int16_t value, uint32_t timestamp)
{
    if (timestamp > last_updated_time)
    {
        if (timestamp - last_updated_time < period)
        {
            return false;
        }
    }
    else
    {
        if (timestamp < period)
        {
            return false;
        }
    }
    last_updated_time = timestamp;
    data.add(value);
    return true;
}

void SensorStorage::reset()
{
    if (data.reset())
    {
        last_updated_time = 0;
    }
}

uint8_t SensorStorage::get_size()
{
    return data.get_size();
}
