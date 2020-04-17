#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "lib/avr/hardware/i2c.hpp"
#include "weatherapp.hpp"
#include "../../dragberry/os.hpp"

WeatherApp::WeatherApp() :
    clock(DS1307::Clock(DS1307_ADDRESS,
            I2C::device_read<DS1307::Status, DS1307::Status::OK, DS1307::Status::DEV_NOT_FOUND>,
            I2C::device_write<DS1307::Status, DS1307::Status::OK, DS1307::Status::DEV_NOT_FOUND>
            )
    ),
    interface
    {
        BME280::InterfaceType::I2C,
        I2C::device_read<BME280::Status, BME280::Status::OK, BME280::Status::DEV_NOT_FOUND>,
        I2C::device_write<BME280::Status, BME280::Status::OK, BME280::Status::DEV_NOT_FOUND>,
        WeatherApp::delay_ms
    },
    device(BME280::Device(interface, BMP280_ADDR))
{
    I2C::init();
    I2C::set_bitrate(400);
    time_to_live = TIME_TO_LIVE;
}

WeatherApp::~WeatherApp()
{
}

void WeatherApp::run()
{
    init();
    while (is_going_on())
    {
        device.read_sensor_data(BME280_ALL, [&](BME280::Data& data) -> void
        {
            temperature_sensor.set_value(data.temperature);
            pressure_sensor.set_value(data.pressure);
            humidity_sensor.set_value(data.humidity);
        });

        active_sensor->draw();
        dragberry::os::display::update_assured();

        delay_ms(100);

        System::io::exchange(
            [&](char* frame) -> void
            {
                frame[1] = System::APP_WEATHER;
                System::io::decompose(time, 2);
                System::io::decompose(temperature_sensor.get_value(), 4);
                System::io::decompose(pressure_sensor.get_value(), 8);
                System::io::decompose(humidity_sensor.get_value(), 12);
            },
            [&](char* frame) -> void
            {
                active_sensor_code = (Sensor::Code) frame[3];
                mode = (WeatherApp::Mode) frame[4];
            }
        );
    }

    uint32_t time = get_time();
    temperature_sensor.update(time);
    pressure_sensor.update(time);
    humidity_sensor.update(time);
    System::deregister_timer(this);
}

void WeatherApp::init()
{
    temperature_sensor.load();
    pressure_sensor.load();
    humidity_sensor.load();
    device.init([](BME280::Settings &settings) -> void
    {
        /* Recommended mode of operation: Indoor navigation */
        settings.osr_h = BME280_OVERSAMPLING_1X;
        settings.osr_p = BME280_OVERSAMPLING_16X;
        settings.osr_t = BME280_OVERSAMPLING_2X;
        settings.filter = BME280_FILTER_COEFF_16;
        settings.standby_time = BME280_STANDBY_TIME_62_5_MS;
    });
    device.set_sensor_settings(
            BME280_OSR_PRESS_SEL |
            BME280_OSR_TEMP_SEL |
            BME280_OSR_HUM_SEL |
            BME280_FILTER_SEL |
            BME280_STANDBY_SEL
    );
    device.set_sensor_mode(BME280_NORMAL_MODE);
    System::register_timer(this, 100);
}

static const uint8_t SENSOR_TTL = 16;
static const uint8_t VALUE_TTL = 8;
static const uint8_t CHART_TTL = 2;

void WeatherApp::play()
{
    if (time % SENSOR_TTL == 0)
    {
        if (++active_sensor_index == SENSORS)
        {
            active_sensor_index = 0;
        }
        active_sensor = sensors[active_sensor_index];
    }
    play_sensor();
}

void WeatherApp::play_sensor()
{
    uint16_t sensor_up_time = time % SENSOR_TTL;
    switch (mode)
    {
    case Mode::VALUE:
        play_sensor_value();
        break;
    case Mode::CHARTS:
        play_sensor_charts();
        break;
    default:
        if (sensor_up_time < VALUE_TTL)
        {
            play_sensor_value();
        }
        else
        {
            play_sensor_charts();
        }
        break;
    }
}

void WeatherApp::play_sensor_value()
{
    active_sensor->display_mode = Sensor::DisplayMode::VALUE;
}

void WeatherApp::play_sensor_charts()
{
    uint16_t chart_idx = ((time % SENSOR_TTL) - VALUE_TTL) / CHART_TTL;
    if (chart_idx % 2)
    {
        active_sensor->display_mode = Sensor::DisplayMode::CHART_Y_AXIS;
    }
    else
    {
        active_sensor->display_mode = Sensor::DisplayMode::CHART_X_AXIS;
    }
}

void WeatherApp::on_timer_event()
{
    increment_time();
    uint8_t i = 0;
    switch (active_sensor_code)
    {
    case Sensor::Code::TEMPERATURE:
    case Sensor::Code::PRESSURE:
    case Sensor::Code::HUMIDITY:
        active_sensor_index = 0;
        while (i < SENSORS)
        {
            if (sensors[i]->code == active_sensor_code)
            {
                active_sensor = sensors[i];
                active_sensor_index = i;
                break;
            }
            i++;
        }
        play_sensor();
        break;
    default:
        play();
        break;
    }
}

void WeatherApp::delay_ms(uint8_t ms)
{
    while (0 < ms--)
    {
      _delay_ms(1);
    }
}

uint32_t WeatherApp::get_time()
{
    clock.refresh();
    uint32_t time = 0;
    time += clock.seconds();
    time += (clock.minutes() * 60);
    time += (clock.hours() * 360);
    time += (clock.days() * 86400);
    return time;
}
