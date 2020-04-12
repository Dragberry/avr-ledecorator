#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lib/avr/hardware/uart.hpp"
#include "os.hpp"
#include "../apps/clock/clockapp.hpp"
#include "../apps/games/life/lifegame.hpp"
#include "../apps/games/snake/snakegame.h"
#include "../apps/weather/weatherapp.hpp"

volatile uint16_t System::time = 0;

Timer *System::timer = nullptr;

uint16_t System::period = 0;

volatile uint8_t System::curr_app_code = 0;

volatile uint8_t System::next_app_code = 0;

Application* System::current_app = nullptr;

Execution System::programms[] = {
    { APP_SNAKE, []() -> void
        {
            SnakeGame app;
            current_app = &app;
            app.run();
            current_app = nullptr;
        }
    },
    { APP_CLOCK, []() -> void
        {
            ClockApp app;
            current_app = &app;
            app.run();
            current_app = nullptr;
        }
    }
};

Timer::~Timer()
{
}

void System::register_timer(Timer *timer, uint8_t period)
{
    System::period = period * 4;
    System::timer = timer;
}

void System::deregister_timer(Timer *timer)
{
    System::timer = nullptr;
    System::period = 0;
}

inline
void System::on_system_timer_event()
{
    if (time % (period == 0 ? 32 : period) == period / 2)
    {
        if (BLE::is_connected())
        {
            BLE::run();
        }
        else
        {
            BLE::stop();
        }

    }
    if (timer != nullptr)
    {
        if (time % period == 0)
        {
            timer->on_timer_event();
        }
    }
    if (++time == 64000)
    {
        time = 0;
    }
}

void System::init()
{
    UartBus::init();

    SPI::init();

    // BLE Listener input
    cbi(DDRD, PD3);
    sbi(PORTD, PD3);
    // BLE Listener interruption
    sbi(EICRA, ISC10);
    cbi(EICRA, ISC11);
    sbi(EIMSK, INT1);
    // Main timer
    cbi(TCCR0B, WGM02);
    sbi(TCCR0A, WGM01);
    cbi(TCCR0A, WGM00);

    sbi(TCCR0B, CS02);
    cbi(TCCR0B, CS01);
    sbi(TCCR0B, CS00);

    sbi(TIMSK0, OCIE0A);

    outb(OCR0A, 49);

    BLE::check_connection();

    sei();
}

void System::run()
{
    static const uint8_t APPS = 2;

    uint8_t execution_index = 0;
    while (true)
    {
        if (next_app_code != 0)
        {
            uint8_t i = 0;
            while (i < APPS)
            {
                if (programms[i].code == next_app_code)
                {
                    execution_index = i;
                    next_app_code = 0;
                    break;
                }
                i++;
            }
        }
        curr_app_code = programms[execution_index].code;
        programms[execution_index].exec();
        if (++execution_index >= APPS)
        {
            execution_index = 0;
        }
    }
}

void System::process_event()
{
    uint8_t app_code = BLE::tx_buffer[1];
    uint8_t command = BLE::tx_buffer[2];
    if (app_code != APP_IDLE)
    {
       if (app_code != curr_app_code)
       {
           current_app->terminate();
           next_app_code = app_code;
       }
       else if (command == COMMAND_RESTART)
       {
           current_app->terminate();
       }
    }
}

ISR(TIMER0_COMPA_vect)
{
    System::on_system_timer_event();
}
