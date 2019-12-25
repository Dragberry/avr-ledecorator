#include "display.hpp"
#include <avr/interrupt.h>

using namespace dragberry::os;

static display::Buffers buffers;

static display::Transmitter transmitter;

inline void display::Buffers::swap()
{
	uint8_t(*temp)[SCREEN_WIDTH] = active_buffer;
	active_buffer = buffer;
	buffer = temp;
}

display::Transmitter::Transmitter()
{
	UART::init(UART::BaudRate::B_2_500_000);
	UART::set_rx_handler(this);
	Timers::T0::start(0, Timers::Prescaller::F_256, this);
}

inline void display::Transmitter::enable()
{
	sbi(TIMSK0, OCIE0A);
}

inline void display::Transmitter::disable()
{
	cbi(TIMSK0, OCIE0A);
}

void display::Transmitter::start_new_frame()
{
	state = FRAME_START;
	enable();
}

void display::Transmitter::on_timer0_event()
{
	if (is_busy)
	{
		return;
	}
	switch (state)
	{
	case TRANSMIT:
		send_byte(buffers.active_buffer[y][x]);
		break;
	 case FRAME_START:
		send_byte(mask_command(CMD_DEFAULT_FRAME_START));
		break;
	case FRAME_END:
		send_byte(mask_command(CMD_DEFAULT_FRAME_END));
		break;
	default:
		break;
	}
}

inline void display::Transmitter::send_byte(const uint8_t byte)
{
	is_busy = true;
	UART::send_byte(byte);
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
			}
		}
		break;
	case FRAME_START:
		x = 0;
		y = 0;
		state = TRANSMIT;
		break;
	case FRAME_END:
		disable();
		state = IDLE;
		break;
	default:
		break;
	}
	is_busy = false;
}

void display::connect()
{
	while (transmitter.state != Transmitter::IDLE);
	transmitter.is_connected = true;
}

void display::disconnect()
{
	while (transmitter.state != Transmitter::IDLE);
	transmitter.is_connected = false;
}

void display::update_requsted()
{
	if (transmitter.is_connected && transmitter.state == Transmitter::IDLE)
	{
		buffers.swap();
		transmitter.start_new_frame();
	}
}

void display::update_assured()
{
	while (!transmitter.is_connected || transmitter.state != Transmitter::IDLE);
	buffers.swap();
	transmitter.start_new_frame();
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
