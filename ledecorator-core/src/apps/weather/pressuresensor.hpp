#ifndef PRESSURESENSOR_HPP_
#define PRESSURESENSOR_HPP_

#include "sensor.hpp"

class PressureSensor : public Sensor
{
private:
    static const uint32_t EEMEM LAST_UPDATED_TIME;

    static const RingBuffer<int16_t, 6> EEMEM PRESSURE_DB;

    static const uint8_t PROGMEM IMG_PRESSURE_DATA[128];

    static const Image PROGMEM IMG_PRESSURE;

public:
    PressureSensor();

    void load();

    void update(uint32_t time);

protected:
    void process_value();
};

#endif
