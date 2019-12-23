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
	UART::init(UART::BaudRate::B_1_250_000);
	UART::set_rx_handler(this);
	Timers::T0::start(0, Timers::Prescaller::F_256, this);
}

inline void display::Transmitter::reset()
{
	x = 0;
	y = 0;
}

void display::Transmitter::new_picture()
{
	is_byte_being_transmitted = true;
	UART::send_byte(mask_command(CMD_DEFAULT));
	is_image_being_transmitted = true;
}

void display::Transmitter::on_timer0_event()
{
	if (is_connected && is_image_being_transmitted)
	{
		send_next_byte();
	}
}

inline void display::Transmitter::send_next_byte()
{
	if (is_byte_being_transmitted)
	{
		return;
	}
	is_byte_being_transmitted = true;
	UART::send_byte(buffers.active_buffer[y][x]);
	if (++x == SCREEN_WIDTH)
	{
		x = 0;
		if (++y == SCREEN_HEIGHT)
		{
			y = 0;
			is_image_being_transmitted = false;
		}
	}
}

void display::Transmitter::on_uart_rx_event(const uint8_t byte)
{
	is_byte_being_transmitted = false;
}

void display::connect()
{
	transmitter.reset();
	transmitter.is_connected = true;
//	sbi(TIMSK0, OCIE0A);
}

void display::disconnect()
{
//	cbi(TIMSK0, OCIE0A);
	transmitter.is_connected = false;
	transmitter.is_image_being_transmitted = false;
}

void display::update_requsted()
{
	if (!transmitter.is_image_being_transmitted)
	{
		buffers.swap();
		transmitter.new_picture();
	}
}

void display::update_assured()
{
	while (transmitter.is_image_being_transmitted);
	buffers.swap();
	transmitter.new_picture();
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
