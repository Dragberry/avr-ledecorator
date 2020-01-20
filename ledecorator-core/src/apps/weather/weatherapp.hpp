#ifndef WEATHERAPP_HPP_
#define WEATHERAPP_HPP_

#include "lib/avr/hardware/timers.hpp"
#include "pressuresensor.hpp"
#include "temperaturesensor.hpp"
#include "../../hardware/bme280/bme280.hpp"
#include "../../hardware/ds1307/ds1307.hpp"
#include "../../dragberry/os/drawablestring.hpp"

#define BMP280_ADDR 0xEC

class WeatherApp : public Timers::T1::Handler
{
private:
    DS1307::Clock clock;
    BME280::Interface interface;
    BME280::Device device;

    volatile uint16_t time = 0;

    TemperatureSensor temperature_sensor;
    PressureSensor pressure_sensor;

    Sensor* sensors[2] =
    {
        &temperature_sensor,
        &pressure_sensor
    };

    uint8_t active_sensor_index = 0;
    Sensor* active_sensor = sensors[active_sensor_index];

public:
    WeatherApp();

    ~WeatherApp();

    static void runner();

    void run();

    void on_timer1_event();

    static void delay_ms(uint8_t ms);

private:
    void init();

    void work();

    uint32_t get_time();
};

#endif
