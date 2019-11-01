#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdint.h>
#include "../colors.h"
#include "../definitions.h"
#include "../row.h"
#include "../section.h"

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_SCK PB5
#define SPI_MOSI PB3
#define SPI_SS PB2

class Screen
{
private:
	uint8_t** buffer_1;
	uint8_t** buffer_2;

	volatile uint8_t is_being_read;

	Row current_row;

	uint8_t rows_state;

	void init_device();

	void launch();

	void start_row();

	void confirm_row();

	void send_byte(const uint8_t data);

	void apply_colors(Colors& colors, const uint8_t color, const uint8_t current_bit);

	void apply_colors(Section& section, const uint8_t color, const uint8_t offset);

public:
	uint8_t** buffer;
	uint8_t** active_buffer;

	Screen();

	void switch_buffer();

	inline void start_reading();

	inline void stop_reading();

	void draw_row();
};

#endif
