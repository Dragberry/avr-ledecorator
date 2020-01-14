#include "clockapp.hpp"

ClockApp::ClockApp() :
        clock(DS1307::Clock(DS1307_ADDRESS, ClockApp::read, ClockApp::write)),
        hh_mm_string(DrawableString(3, 0, 27, 8)),
        ss_string(DrawableString(10, 8, 12, 8))
{
    hh_mm_string_value[0] = '0';
    hh_mm_string_value[1] = '0';
    hh_mm_string_value[2] = ':';
    hh_mm_string_value[3] = '0';
    hh_mm_string_value[4] = '0';
    hh_mm_string_value[5] = '\0';

    ss_string.color = WHITE;

    hh_mm_string_value[0] = '0';
    ss_string_value[1] = '0';

    ss_string.color = WHITE;
}

void ClockApp::runner()
{
    ClockApp app;
    app.run();
}

void ClockApp::run()
{
    I2C::init();
    I2C::set_bitrate(400);

    clock.refresh();
    if (clock.status != DS1307::Status::OK)
    {
        return;
    }

//    clock.seconds(0);
//    clock.minutes(35);
//    clock.hours(2);
//    clock.update();

    Timers::T1::start(0x7A1, Timers::Prescaller::F_1024, this);
    while (time < CLOCK_APP_TIME)
    {
        if (update_required)
        {
            hh_mm_string.draw();
            ss_string.draw();
            dragberry::os::display::update_assured();
            update_required = false;
        }
    }
}

void ClockApp::on_timer1_event()
{
    time++;
    clock.refresh();
    if (clock.status == DS1307::Status::OK)
    {
        *(hh_mm_string_value + 0) = clock.hours_d();
        *(hh_mm_string_value + 1) = clock.hours_u();

        *(hh_mm_string_value + 3) = clock.minutes_d();
        *(hh_mm_string_value + 4) = clock.minutes_u();

        hh_mm_string.set_string(hh_mm_string_value);

        *(ss_string_value + 0) = clock.seconds_d();
        *(ss_string_value + 1) = clock.seconds_u();

        ss_string.set_string(ss_string_value);
    }

    update_required = true;
}

DS1307::Status ClockApp::read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length)
{
    if (I2C::device_read(dev_addr, reg_addr, data, length) == I2C::Status::OK)
    {
        return DS1307::Status::OK;
    }
    return DS1307::Status::DEV_NOT_FOUND;
}

DS1307::Status ClockApp::write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length)
{
    if (I2C::device_write(dev_addr, reg_addr, data, length) == I2C::Status::OK)
    {
        return DS1307::Status::OK;
    }
    return DS1307::Status::DEV_NOT_FOUND;
}
