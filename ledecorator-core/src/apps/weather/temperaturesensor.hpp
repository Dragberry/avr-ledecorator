#ifndef TEMPERATURESENSOR_HPP_
#define TEMPERATURESENSOR_HPP_

#include "sensor.hpp"

class TemperatureSensor : public Sensor
{
private:
    static const char UNIT[];

    static const uint32_t EEMEM LAST_UPDATED_TIME;

    static const RingBuffer<int16_t, 6> EEMEM TEMPERATURE_DB;

    static const uint8_t PROGMEM IMG_TEMPERATURE_DATA[128];

    static const Image PROGMEM IMG_TEMPERATURE;

public:
    TemperatureSensor();

    void load();

    void update(uint32_t time);

protected:
    void process_value();
};

#endif
