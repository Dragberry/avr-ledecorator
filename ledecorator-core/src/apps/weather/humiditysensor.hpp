#ifndef HUMIDITYSENSOR_HPP_
#define HUMIDITYSENSOR_HPP_

#include "sensor.hpp"

class HumiditySensor : public Sensor
{
public:
    static const char UNIT[];

private:
    static const uint32_t EEMEM LAST_UPDATED_TIME;

    static const RingBuffer<int16_t, 6> EEMEM HUMIDITY_DB;

    static const uint8_t PROGMEM IMG_HUMIDITY_DATA[128];

    static const Image PROGMEM IMG_HUMIDITY;

public:
    HumiditySensor();

    void load();

    void update(uint32_t time);

protected:
    void process_value();
};

#endif
