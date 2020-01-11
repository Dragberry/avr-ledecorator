#ifndef WEATHERAPP_HPP_
#define WEATHERAPP_HPP_

#include "lib/avr/hardware/timers.hpp"
#include "pressuresensor.hpp"
#include "temperaturesensor.hpp"
#include "../../hardware/bme280/bme280.hpp"
#include "../../dragberry/os/drawablestring.hpp"

#define BMP280_ADDR 0xEC

class WeatherApp :
        public BME280::Interface,
        public Timers::T1::Handler
{
private:
    BME280::Device device =
            BME280::Device(this, BMP280_ADDR);

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

    BME280::Status read(
                uint8_t dev_id,
                uint8_t reg_addr,
                uint8_t *reg_data,
                uint8_t len
                );

    BME280::Status write(
                uint8_t dev_id,
                uint8_t reg_addr,
                uint8_t *reg_data,
                uint8_t len
                );

    void delay_ms(uint8_t ms);

private:
    void init();

    void work();

};

#endif
