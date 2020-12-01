#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "os.hpp"
#include "../apps/clock/clockapp.hpp"
#include "../apps/games/life/lifegame.hpp"
#include "../apps/games/snake/snakegame.h"
#include "../apps/sandbox/sandboxapp.hpp"
#include "../apps/weather/weatherapp.hpp"

volatile uint16_t System::time = 0;

Timer *System::timer = nullptr;

uint16_t System::period = 0;

volatile uint8_t System::curr_app_code = 0;

volatile uint8_t System::next_app_code = 0;

Application* System::current_app = nullptr;

const uint8_t System::NUMBER_OF_PROGRAMMS = 5;

const uint8_t EEMEM System::NUMBER_OF_STORED_PROGRAMMS = NUMBER_OF_PROGRAMMS;

const uint8_t EEMEM System::STORED_PROGRAMMS[NUMBER_OF_PROGRAMMS] = {
        APP_SNAKE,
        APP_CLOCK,
        APP_WEATHER,
        APP_LIFE,
        APP_SANDBOX
};

uint8_t System::number_of_loaded_programms = 0;

Execution System::loaded_programms[NUMBER_OF_PROGRAMMS] = { 0 };

Timer::~Timer()
{
}

void System::register_timer(Timer *timer, uint16_t period)
{
    System::period = period * 4;
    System::timer = timer;
}

void System::deregister_timer()
{
    System::timer = nullptr;
    System::period = 0;
}

inline
void System::on_system_timer_event()
{
    if (time % 32 == 0)
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

    cbi(DDRC, PC3);
    sbi(PORTC, PC3);
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

void System::load()
{
    number_of_loaded_programms = 0;
    uint8_t number_of_stored_programms = eeprom_read_byte(&NUMBER_OF_STORED_PROGRAMMS);
    uint8_t index = 0;
    while (index < number_of_stored_programms)
    {
        switch (eeprom_read_byte(&STORED_PROGRAMMS[index]))
        {
        case APP_SNAKE:
            loaded_programms[number_of_loaded_programms] =
            { APP_SNAKE, []() -> void
                {
                    SnakeGame app;
                    current_app = &app;
                    app.run();
                    current_app = nullptr;
                }
            };
            number_of_loaded_programms++;
            break;
        case APP_CLOCK:
            loaded_programms[number_of_loaded_programms] =
            { APP_CLOCK, []() -> void
                {
                    ClockApp app;
                    current_app = &app;
                    app.run();
                    current_app = nullptr;
                }
            };
            number_of_loaded_programms++;
            break;
        case APP_WEATHER:
            loaded_programms[number_of_loaded_programms] =
            { APP_WEATHER, []() -> void
                {
                    WeatherApp app;
                    current_app = &app;
                    app.run();
                    current_app = nullptr;
                }
            };
            number_of_loaded_programms++;
            break;
        case APP_LIFE:
            loaded_programms[number_of_loaded_programms] =
            { APP_LIFE, []() -> void
                {
                    LifeGame app;
                    current_app = &app;
                    app.run();
                    current_app = nullptr;
                }
            };
            number_of_loaded_programms++;
            break;
        case APP_SANDBOX:
            loaded_programms[number_of_loaded_programms] =
            { APP_SANDBOX, []() -> void
                {
                    SandboxApp app;
                    current_app = &app;
                    app.run();
                    current_app = nullptr;
                }
            };
            number_of_loaded_programms++;
            break;
        default:
            break;
        }
        index++;
    }
}

void System::run()
{
    uint8_t execution_index = 0;
    while (true)
    {
        if (next_app_code != 0)
        {
            uint8_t i = 0;
            while (i < number_of_loaded_programms)
            {
                if (loaded_programms[i].code == next_app_code)
                {
                    execution_index = i;
                    next_app_code = 0;
                    break;
                }
                i++;
            }
        }
        curr_app_code = loaded_programms[execution_index].code;
        loaded_programms[execution_index].exec();
        if (++execution_index >= number_of_loaded_programms)
        {
            execution_index = 0;
        }
    }
}

void System::process_event()
{
    uint8_t app_code = BLE::rx_buffer[1];
    uint8_t command = BLE::rx_buffer[2];


    bool app_code_valid = false;
    if (app_code == APP_IDLE)
    {
        app_code_valid = true;
    }
    else
    {
        uint8_t i = 0;
        while (i < number_of_loaded_programms)
        {
           if (loaded_programms[i++].code == app_code)
           {
               app_code_valid = true;
               break;
           }
        }
    }


    if (!app_code_valid)
    {
        return;
    }
    if (app_code != APP_IDLE && app_code != curr_app_code)
    {
       current_app->terminate();
       next_app_code = app_code;
    }
    else
    {
       switch (command)
       {
       case COMMAND_INFINITE:
           current_app->ignore_ttl(true);
           break;
       case COMMAND_FINITE:
           current_app->ignore_ttl(false);
           break;
       case COMMAND_RESTART:
           current_app->terminate();
           break;
       default:
           break;
       }
    }
}

void System::io::decompose(const uint16_t& value, const uint8_t idx)
{
    BLE::tx_buffer[idx] = (char)value;
    BLE::tx_buffer[idx + 1] = (char)(value >> 8);
}

void System::io::decompose(const int32_t& value, const uint8_t idx)
{
    BLE::tx_buffer[idx] = (char)value;
    BLE::tx_buffer[idx + 1] = (char)(value >> 8);
    BLE::tx_buffer[idx + 2] = (char)(value >> 16);
    BLE::tx_buffer[idx + 3] = (char)(value >> 24);
}

ISR(TIMER0_COMPA_vect)
{
    System::on_system_timer_event();
}
