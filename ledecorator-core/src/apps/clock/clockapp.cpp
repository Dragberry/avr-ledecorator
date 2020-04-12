#include <string.h>
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

    this->time_to_live = CLOCK_APP_TIME;
}

ClockApp::~ClockApp()
{
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

    System::register_timer(this, 10);
    while (is_going_on())
    {
        if (update_required)
        {
            clock.refresh();
            if (time % 10 == 0)
            {
                System::io::exchange(
                    [&](char* frame) -> void
                    {
                        frame[1] = System::APP_CLOCK;
                        frame[2] = clock.hours();
                        frame[3] = clock.minutes();
                        frame[4] = clock.seconds();
                        frame[5] = clock.days();
                        frame[6] = clock.months();
                        frame[7] = clock.years();
                        frame[8] = (char)(time >> 8);
                        frame[9] = (char) time;
                    },
                    [&](char* frame) -> void
                    {
                        if (frame[3] == 'U')
                        {
                            clock.hours(frame[4]);
                            clock.minutes(frame[5]);
                            clock.seconds(frame[6]);
                            clock.days(frame[7]);
                            clock.months(frame[8]);
                            clock.years(frame[9]);
                            clock.update();
                        }
                    }
                );
            }

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

            dragberry::os::display::clear_screen(BLACK);
            hh_mm_string.draw();
            ss_string.draw();
            date_string.draw();
            dragberry::os::display::update_assured();
            update_required = false;
        }
    }
    System::deregister_timer(this);
}

void ClockApp::on_timer_event()
{
    increment_time();
    update_required = true;
}
