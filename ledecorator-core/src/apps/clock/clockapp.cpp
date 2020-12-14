#include <string.h>
#include "lib/avr/hardware/i2c.hpp"
#include "clockapp.hpp"
#include "../../data/font3x5.hpp"
#include "../../data/font5x7.hpp"


const ClockApp::StoredState EEMEM ClockApp::STORED_STATE = ClockApp::StoredState();


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
    eeprom_read_block((void*) &app_state, (const void*) &STORED_STATE, sizeof(STORED_STATE));
    time_to_live = app_state.time_to_live;
    hh_mm_string.set_string(hh_mm_string_value);
    ss_string.set_string(ss_string_value);
    date_string.set_string(date_string_value);
    set_colors();
}

ClockApp::~ClockApp()
{
}

void ClockApp::set_colors()
{
    hh_mm_string.color = app_state.time_color;
    hh_mm_string.bg_color = app_state.background_color;
    ss_string.color = app_state.seconds_color;
    ss_string.bg_color = app_state.background_color;
    date_string.color = app_state.date_color;
    date_string.bg_color = app_state.background_color;
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
            if (time % 8 == 0)
            {
                System::io::exchange(
                    [&](char* frame) -> void
                    {
                        frame[1] = System::APP_CLOCK;
                        System::io::decompose(time, 2);
                        if (load_requested)
                        {
                            eeprom_read_block((void*) &app_state, (const void*) &STORED_STATE, sizeof(STORED_STATE));
                            frame[4] = System::COMMAND_LOAD;
                            System::io::decompose(app_state.time_to_live / TICKS_PER_SECOND, 5);
                            frame[7] = app_state.time_color;
                            frame[8] = app_state.seconds_color;
                            frame[9] = app_state.date_color;
                            frame[10] = app_state.background_color;
                            load_requested = false;
                        }
                        else
                        {
                            frame[4] = System::COMMAND_EMPTY;
                            frame[5] = clock.hours();
                            frame[6] = clock.minutes();
                            frame[7] = clock.seconds();
                            frame[8] = clock.days();
                            frame[9] = clock.months();
                            frame[10] = clock.years();
                        }
                    },
                    [&](char* frame) -> void
                    {
                        switch (frame[3])
                        {
                        case System::COMMAND_LOAD:
                            load_requested = true;
                            break;
                        case System::COMMAND_SAVE:
                            System::io::compose(app_state.time_to_live, 4);
                            app_state.time_to_live *= TICKS_PER_SECOND;
                            app_state.time_color = frame[6];
                            app_state.seconds_color = frame[7];
                            app_state.date_color = frame[8];
                            app_state.background_color = frame[9];
                            set_colors();
                            eeprom_update_block((const void*) &app_state, (void*) &STORED_STATE, sizeof(STORED_STATE));
                            load_requested = true;
                            break;
                        case SET_TIME_COMMAND:
                            clock.hours(frame[4]);
                            clock.minutes(frame[5]);
                            clock.seconds(frame[6]);
                            clock.days(frame[7]);
                            clock.months(frame[8]);
                            clock.years(frame[9]);
                            clock.blink(true);
                            clock.update();
                            break;
                        default:
                            break;
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

            dragberry::os::display::clear_screen(app_state.background_color);
            hh_mm_string.draw();
            ss_string.draw();
            date_string.draw();
            dragberry::os::display::update_assured();
            update_required = false;
        }
    }
    System::deregister_timer();
}

void ClockApp::on_timer_event()
{
    increment_time();
    update_required = true;
}
