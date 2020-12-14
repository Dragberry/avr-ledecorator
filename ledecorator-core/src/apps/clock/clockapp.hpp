#ifndef CLOCKAPP_HPP_
#define CLOCKAPP_HPP_

#include "../../dragberry/os.hpp"
#include "../../dragberry/os/display.hpp"
#include "../../dragberry/os/drawablestring5x7.hpp"
#include "../../dragberry/os/drawablestring3x5.hpp"
#include "../../hardware/ds1307/ds1307.hpp"

using namespace dragberry::os;

class ClockApp :
        public Application,
        public Timer
{
private:
    static const uint8_t TICKS_PER_SECOND = 10;

    static const uint8_t SET_TIME_COMMAND = 'U';

    struct StoredState
    {
        uint16_t time_to_live = 30 * TICKS_PER_SECOND;
        Color time_color = WHITE;
        Color seconds_color = RED;
        Color date_color = YELLOW;
        Color background_color = BLACK;
    };

    static const StoredState EEMEM STORED_STATE;

    StoredState app_state;

    volatile bool load_requested = false;

    volatile bool update_required = false;

    DS1307::Clock clock;

    char hh_mm_string_value[6];

    DrawableString5x7 hh_mm_string;

    char ss_string_value[3];

    DrawableString3x5 ss_string;

    char date_string_value[9];

    DrawableString3x5 date_string;

    void set_colors();

public:
    ClockApp();

    ~ClockApp();

    void run();

    void on_timer_event();

};

#endif
