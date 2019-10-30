#include "screen.h"

Screen::Screen()
{
	init_device();
	buffer_1 = new uint8_t*[SCREEN_HEIGHT];
	buffer_2 = new uint8_t*[SCREEN_HEIGHT];
	for (uint8_t row = 0; row < SCREEN_HEIGHT; row++)
	{
		buffer_1[row] = new uint8_t[SCREEN_WIDTH];
		buffer_2[row] = new uint8_t[SCREEN_WIDTH];
	}
	active_buffer = buffer_1;
	buffer = buffer_2;

	is_being_read = 0;

	row = Row();
	launch();
}

void Screen::init_device()
{
	DDRC = ROW_L_MASK;
	DDRB = ROW_H_MASK;

	SPI_DDR |= (1<<SPI_SS) | (1<<SPI_MOSI) | (1<<SPI_SCK);
	SPI_PORT |= (1<<SPI_SS);
	SPCR |= (1<<SPE) | (1<<MSTR);
	SPSR |= (1<<SPI2X);
}

void Screen::launch()
{
	// CTC
	TCCR0A |= (0<<WGM00);
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (0<<WGM02);
	// 001 - f
	// 010 - f/8
	// 011 - f/64
	// 100 - f/256
	// 101 - f/1024
	TCCR0B |= (1<<CS02);
	TCCR0B |= (0<<CS01);
	TCCR0B |= (1<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	// 0.2s
	OCR0A = 4;
}

inline void Screen::start_row()
{
	SPI_PORT &= ~(1<<SPI_SS);
}

inline void Screen::confirm_row()
{
	SPI_PORT |= (1<<SPI_SS);
}

inline void Screen::send_byte(const uint8_t data)
{
	SPDR = data;
	while (!(SPSR & (1<<SPIF)));
}

void Screen::switch_buffer()
{
	while (is_being_read);
	uint8_t** temp = active_buffer;
	active_buffer = buffer;
	buffer = temp;
}

inline void Screen::start_reading()
{
	is_being_read = 1;
}

inline void Screen::stop_reading()
{
	is_being_read = 0;
}

void Screen::apply_colors(Colors& colors, const uint8_t color, const uint8_t currentBit)
{
	if (color > 0b00)
	{
		colors.level0 |= currentBit;
		colors.level1 |= currentBit;
		if (color > 0b01)
		{
			colors.level2 |= currentBit;
			if (color > 0b10)
			{
				colors.level3 |= currentBit;
			}
		}
	}
}

void Screen::apply_colors(Section& section, const uint8_t color, const uint8_t offset)
{
	uint8_t currentBit = (1<<offset);
	apply_colors(section.red, (color & RED), currentBit);
	apply_colors(section.green, (color & GREEN) >> 2, currentBit);
	apply_colors(section.blue, (color & BLUE) >> 4, currentBit);
}

void Screen::draw_row()
{
	if (row.step == 0)
	{
		if (row.index == 0)
		{
			start_reading();
		}
		row.reset();
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++)
		{
			// 0,1,2,3,4,5,6,7...
			uint8_t sectionRowIdx = sectionIdx / SECTIONS_PER_ROW;
			// 0,0,0,0,1,1,1,1...
			uint8_t pictureY = (H_SECTION * sectionRowIdx) + row.index;
			if (sectionRowIdx % 2 == 0) {
				// not-inverted
				// sectionRowIdx: 0
				uint8_t sectionColumnIdx = sectionIdx % SECTIONS_PER_ROW;
				// sectionColumnIdx: 0,1,2,3
				// pictureY: 0,1,2,3,4,5,6,7
				uint8_t pictureX = (W_SECTION * sectionColumnIdx);
				for (uint8_t xOffset = 0; xOffset < W_SECTION; xOffset++)
				{
					uint8_t value = active_buffer[pictureY][pictureX + xOffset];
					apply_colors(row.sections[sectionIdx], value, xOffset);
				}
			}
			else
			{
				// inverted
				// sectionRowIdx: 1
				uint8_t sectionColumnIdx = MAX_SECTION_IDX_IN_ROW - (sectionIdx % SECTIONS_PER_ROW);
				// sectionColumnIdx: 3,2,1,0
				// pictureY: 8,9,10,11,12,13,14,15
				uint8_t pictureX = (W_SECTION * sectionColumnIdx);
				for (uint8_t xOffset = 0; xOffset < W_SECTION; xOffset++)
				{
					uint8_t value = active_buffer[pictureY][pictureX + MAX_PIXEL_IDX_IN_SEGMENT - xOffset];
					apply_colors(row.sections[sectionIdx], value, xOffset);
				}
			}

		}
		uint8_t rows = (1<<row.index);
		row.low = ~(rows & ROW_L_MASK);
		row.high = ~((rows >> 6) & ROW_H_MASK);

		if (row.index == ROWS - 1)
		{
			stop_reading();
		}

		start_row();
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++)
		{
			Section section = row.sections[sectionIdx];
			send_byte(section.red.level0);
			send_byte(section.green.level0);
			send_byte(section.blue.level0);
		}

		PORTC = ROW_L_MASK;
		PORTB = ROW_H_MASK;
		confirm_row();
		PORTC = row.low;
		PORTB = row.high;
	}
	else
	{
		start_row();
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++)
		{
			Section section = row.sections[sectionIdx];

			if (row.step > 2)
			{
				send_byte(section.red.level3);
				send_byte(section.green.level3);
				send_byte(section.blue.level3);
			}
			else if (row.step > 0)
			{
				send_byte(section.red.level2);
				send_byte(section.green.level2);
				send_byte(section.blue.level2);
			}
			else if (row.step > 0)
			{
				send_byte(section.red.level1);
				send_byte(section.green.level1);
				send_byte(section.blue.level1);
			}
			else
			{
				send_byte(section.red.level0);
				send_byte(section.green.level0);
				send_byte(section.blue.level0);
			}

		}
		PORTC = ROW_L_MASK;
		PORTB = ROW_H_MASK;
		confirm_row();
		PORTC = row.low;
		PORTB = row.high;
	}

	if (++row.step == 4)
	{
		row.reset();
		if (++row.index == ROWS)
		{
			row.index = 0;
		}
	}
}
