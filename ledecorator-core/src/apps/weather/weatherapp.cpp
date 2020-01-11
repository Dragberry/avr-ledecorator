#include <stdlib.h>
#include <util/delay.h>
#include "lib/avr/hardware/i2c.hpp"
#include "weatherapp.hpp"
#include "../../dragberry/os.hpp"

#define WEATHER_APP_TIME 100 // x0.1 second

WeatherApp::WeatherApp() :
        BME280::Interface(BME280::InterfaceType::I2C)
{
    I2C::init();
    I2C::set_bitrate(400);
    time = 0;
}

WeatherApp::~WeatherApp()
{
    Timers::T1::stop();
}

void WeatherApp::runner()
{
//    UART::init(UART::BaudRate::B_500_000);
    WeatherApp app;
    app.run();
}

void WeatherApp::run()
{
    init();
    while (time < WEATHER_APP_TIME)
    {
        device.read_sensor_data(BME280_ALL, [&](BME280::Data& data) -> void
        {
            temperature_sensor.set_value(data.temperature);
            pressure_sensor.set_value(data.pressure);
        });
        active_sensor->draw();
        dragberry::os::display::update_assured();
        delay_ms(70);
    }
    temperature_sensor.save();
    pressure_sensor.save();
}

void WeatherApp::init()
{
    temperature_sensor.load();
    pressure_sensor.load();
    device.init([](BME280::Settings &settings) -> void
    {
        /* Recommended mode of operation: Indoor navigation */
        settings.osr_p = BME280_OVERSAMPLING_16X;
        settings.osr_t = BME280_OVERSAMPLING_2X;
        settings.filter = BME280_FILTER_COEFF_16;
        settings.standby_time = BME280_STANDBY_TIME_62_5_MS;
    });
    device.set_sensor_settings(
            BME280_OSR_PRESS_SEL |
            BME280_OSR_TEMP_SEL |
            BME280_FILTER_SEL |
            BME280_STANDBY_SEL
    );
    device.set_sensor_mode(BME280_NORMAL_MODE);
    Timers::T1::start(0x7A1, Timers::Prescaller::F_1024, this);
}

void WeatherApp::on_timer1_event()
{
    time++;
    if (time % 50 == 0)
    {
        if (++active_sensor_index == 2)
        {
            active_sensor_index = 0;
        }
        active_sensor = sensors[active_sensor_index];
    }
}

BME280::Status WeatherApp::read(
            uint8_t dev_id,
            uint8_t reg_addr,
            uint8_t *reg_data,
            uint8_t len
            )
{
    I2C::master_send_ni(dev_id, 1, &reg_addr);
    I2C::master_receive_ni(dev_id, len, reg_data);
    return BME280::Status::OK;
}

BME280::Status WeatherApp::write(
        uint8_t dev_id,
        uint8_t reg_addr,
        uint8_t *reg_data,
        uint8_t len
        )
{
    uint8_t full_data[len + 1];
    full_data[0] = reg_addr;
    uint8_t idx = 1;
    while (idx <= len)
    {
        full_data[idx] = reg_data[idx - 1];
        idx++;
    }
    I2C::master_send_ni(dev_id, len + 1, full_data);
    return BME280::Status::OK;
}

void WeatherApp::delay_ms(uint8_t ms)
{
    while (0 < ms--)
    {
      _delay_ms(1);
    }
}
