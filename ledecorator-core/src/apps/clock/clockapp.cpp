#include "lib/avr/hardware/i2c.hpp"
#include "clockapp.hpp"
#include "../../data/font3x5.hpp"
#include "../../data/font5x7.hpp"

ClockApp::ClockApp() :
        clock(DS1307::Clock(DS1307_ADDRESS,
                I2C::device_read<DS1307::Status, DS1307::Status::OK, DS1307::Status::DEV_NOT_FOUND>,
                I2C::device_write<DS1307::Status, DS1307::Status::OK, DS1307::Status::DEV_NOT_FOUND>
                )
        ),
        hh_mm_string_value{"==:=="},
        hh_mm_string(DrawableString5x7(0, 0, 26, 7)),
        ss_string_value{"=="},
        ss_string(DrawableString3x5(25, 2, 8, 5)),
        date_string_value{"--/--/--"},
        date_string(DrawableString3x5(0, 10, 32, 5))
{
    hh_mm_string.color = WHITE;
    hh_mm_string.set_string(hh_mm_string_value);

    ss_string.color = RED;
    ss_string.set_string(ss_string_value);

    date_string.color = YELLOW;
    date_string.set_string(date_string_value);
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

//    clock.days(18);
//    clock.months(1);
//    clock.years(20);
//    clock.update();

    Timers::T1::start(0x7A1, Timers::Prescaller::F_1024, this);
    while (time < CLOCK_APP_TIME)
    {
        if (update_required)
        {
            dragberry::os::display::clear_screen(BLACK);
            hh_mm_string.draw();
            ss_string.draw();
            date_string.draw();
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
        *(hh_mm_string_value + 0) = clock.hours_d() + '0';
        *(hh_mm_string_value + 1) = clock.hours_u() + '0';

        *(hh_mm_string_value + 3) = clock.minutes_d() + '0';
        *(hh_mm_string_value + 4) = clock.minutes_u() + '0';

        hh_mm_string.set_string(hh_mm_string_value);

        *(ss_string_value + 0) = clock.seconds_d() + '0';
        *(ss_string_value + 1) = clock.seconds_u() + '0';

        ss_string.set_string(ss_string_value);

        *(date_string_value + 0) = clock.days_d() + '0';
        *(date_string_value + 1) = clock.days_u() + '0';

        *(date_string_value + 3) = clock.months_d() + '0';
        *(date_string_value + 4) = clock.months_u() + '0';

        *(date_string_value + 6) = clock.years_d() + '0';
        *(date_string_value + 7) = clock.years_u() + '0';

        date_string.set_string(date_string_value);
    }
    update_required = true;
}
