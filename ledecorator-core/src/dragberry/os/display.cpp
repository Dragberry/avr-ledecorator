#include <avr/interrupt.h>
#include "display.hpp"
#include "../os.hpp"

#ifdef FPS_DEBUG
#include "fps.hpp"
#endif

using namespace dragberry::os;

static display::Buffers buffers;

static display::Transmitter transmitter;

inline void display::Buffers::swap()
{
	uint8_t(*temp)[SCREEN_WIDTH] = active_buffer;
	active_buffer = buffer;
	buffer = temp;
}

void display::Transmitter::send_byte(const uint8_t byte)
{
    UartBus::send_byte(byte);
}

void display::Transmitter::start_new_frame()
{
	#ifdef FPS_DEBUG
		dragberry::os::fps::start();
	#endif
    #ifndef DISPLAY_DEBUG
		state = FRAME_START;
	    send_byte(mask_command(CMD_DEFAULT_FRAME_START));
    #endif
}

void display::Transmitter::on_uart_rx_event(const uint8_t byte)
{
	switch (state)
	{
	case TRANSMIT:
		if (++x == SCREEN_WIDTH)
		{
			x = 0;
			if (++y == SCREEN_HEIGHT)
			{
				state = FRAME_END;
				send_byte(mask_command(CMD_DEFAULT_FRAME_END));
				return;
			}
		}
		send_byte(buffers.active_buffer[y][x]);
		return;
	case FRAME_START:
		y = 0;
		x = 0;
		send_byte(buffers.active_buffer[0][0]);
		state = TRANSMIT;
		return;
	case FRAME_END:
		UartBus::free(UartBus::Device::DISPLAY);
		state = IDLE;
		#ifdef FPS_DEBUG
			fps::count();
		#endif
		return;
	default:
		return;
	}
}

bool display::is_busy()
{
    return transmitter.state != Transmitter::IDLE;
}

void display::connect()
{
	while (transmitter.state != Transmitter::IDLE);
	#ifdef FPS_DEBUG
		fps::init();
		fps::start();
	#endif
}

void display::disconnect()
{
	while (transmitter.state != Transmitter::IDLE);
	#ifdef FPS_DEBUG
		fps::show();
		fps::stop();
	#endif
}

void display::update_requsted()
{
    #ifndef DISPLAY_DEBUG
	    if (transmitter.state == Transmitter::IDLE)
	    {
		    buffers.swap();
		    transmitter.start_new_frame();
	    }
    #endif
}

void display::update_assured()
{
    #ifndef DISPLAY_DEBUG
        while (is_busy());
        while (!UartBus::acquire(UartBus::Device::DISPLAY, UartBus::BaudRate::B_2_500_000, []() -> void
            {
                UartBus::set_rx_handler(&transmitter);
                buffers.swap();
                transmitter.start_new_frame();
            })
        );
    #endif
}


void display::set_pixel(uint8_t y, uint8_t x, Color color)
{
	buffers.buffer[y][x] = color;
}

void display::clear_screen(const Color color)
{
	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
		{
			set_pixel(y, x, color);
		}
	}
}

void display::draw_line_vertical(
		const uint8_t start_x,
		uint8_t start_y,
		const uint8_t length,
		const Color color)
{
	if (start_x > SCREEN_WIDTH)
	{
		return;
	}
	uint8_t max_y = start_y + length;
	if (max_y > SCREEN_HEIGHT)
	{
		max_y = SCREEN_HEIGHT;
	}
	while (start_y < max_y)
	{
		set_pixel(start_y++, start_x, color);
	}
}

void display::draw_line_horizontal(
		uint8_t start_x,
		const uint8_t start_y,
		const uint8_t length,
		const Color color)
{
	if (start_y > SCREEN_HEIGHT)
	{
		return;
	}
	uint8_t max_x = start_x + length;
	if (max_x > SCREEN_WIDTH)
	{
		max_x = SCREEN_WIDTH;
	}
	while (start_x < max_x)
	{
		set_pixel(start_y, start_x++, color);
	}
}

void display::draw_area(
		uint8_t start_x,
		uint8_t start_y,
		const uint8_t width,
		const uint8_t height,
		Color color)
{
	uint8_t max_y = start_y + height;
	if (max_y > SCREEN_HEIGHT)
	{
		max_y = SCREEN_HEIGHT;
	}
	uint8_t max_x = start_x + width;
	if (max_x > SCREEN_WIDTH)
	{
		max_x = SCREEN_WIDTH;
	}
	while (start_y < max_y)
	{
		for (uint8_t x = start_x; x < max_x; x++)
		{
			set_pixel(start_y, x, color);
		}
		start_y++;
	}
}

void display::draw_image(
		uint8_t start_x,
		uint8_t start_y,
		const Image* img,
		const Color bg_color)
{
	draw(
			start_x, start_y,
			0, 0,
			img->get_width(), img->get_height(),
			[&](const uint8_t x, const uint8_t y) -> Color
			{
				const Color color = img->get_pixel(x, y);
				if (bg_color != BLACK && color == BLACK)
				{
					return bg_color;
				}
				return color;
			}
	);
}

void display::draw_image(
		uint8_t start_x,
		uint8_t start_y,
		const int8_t offset_x,
		const int8_t offset_y,
		const ImageMono8x8* img,
		const Color color,
		const Color bg_color)
{
	draw(
			start_x, start_y,
			offset_x, offset_y,
			img->get_width(), img->get_height(),
			[&](const uint8_t x, const uint8_t y) -> Color
			{
				return img->get_bit(x, y) ? color : bg_color;
			});
}
