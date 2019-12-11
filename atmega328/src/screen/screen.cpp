#include "screen.h"

Screen::Screen(DeviceInterface& device_interface) :
		device_interface(device_interface),
		buffer_1{},
		buffer_2{},
		active_buffer(buffer_1),
		buffer(buffer_2),
		current_row(),
		rows_state(0xFF),
		is_being_read(0),
		workers
		{
			new DefaultWorker(*this, device_interface),
			new ByteTerminalWorker(*this, device_interface)
		},
		worker(workers[0]) { }

Screen::~Screen() { }

void Screen::launch()
{
	while(1)
	{
		uint8_t result = worker->do_work();
		if (result == CMD_STOP)
		{
			return;
		}
		else
		{
			worker = workers[result < TOTAL_WORKERS ? result : CMD_DEFAULT];
		}
	}
}

void Screen::switch_buffer()
{
	while (is_being_read);
	uint8_t (*temp)[SCREEN_WIDTH] = active_buffer;
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

void Screen::apply_colors(Pixel& pixel, const uint8_t color, const uint8_t currentBit)
{
	if (color > 0b00)
	{
		pixel.level0 |= currentBit;
		pixel.level1 |= currentBit;
		if (color > 0b01)
		{
			pixel.level2 |= currentBit;
			if (color > 0b10)
			{
				pixel.level3 |= currentBit;
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
	if (current_row.brightness_step == 0)
	{
		if (current_row.index == 0)
		{
			start_reading();
		}
		current_row.reset();
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++)
		{
			// 0,1,2,3,4,5,6,7...
			uint8_t sectionRowIdx = sectionIdx / SECTIONS_PER_ROW;
			// 0,0,0,0,1,1,1,1...
			uint8_t pictureY = (H_SECTION * sectionRowIdx) + current_row.index;
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
					apply_colors(current_row.sections[sectionIdx], value, xOffset);
				}
			}
			else
			{
				// inverted
				// sectionRowIdx: 1
				uint8_t sectionColumnIdx = SECTIONS_PER_ROW - 1 - (sectionIdx % SECTIONS_PER_ROW);
				// sectionColumnIdx: 3,2,1,0
				// pictureY: 8,9,10,11,12,13,14,15
				uint8_t pictureX = (W_SECTION * sectionColumnIdx);
				for (uint8_t xOffset = 0; xOffset < W_SECTION; xOffset++)
				{
					uint8_t value = active_buffer[pictureY][pictureX + W_SECTION - 1 - xOffset];
					apply_colors(current_row.sections[sectionIdx], value, xOffset);
				}
			}

		}
		rows_state = ~(1<<current_row.index);
		if (current_row.index == ROWS - 1)
		{
			stop_reading();
		}
		device_interface.start_row();
		device_interface.send_byte(rows_state);
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++)
		{
			Section section = current_row.sections[sectionIdx];
			device_interface.send_byte(section.red.level0);
			device_interface.send_byte(section.green.level0);
			device_interface.send_byte(section.blue.level0);
		}
		device_interface.complete_row();
	}
	else
	{
		device_interface.start_row();
		device_interface.send_byte(rows_state);
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++)
		{
			Section section = current_row.sections[sectionIdx];

			if (current_row.brightness_step > 2)
			{
				device_interface.send_byte(section.red.level3);
				device_interface.send_byte(section.green.level3);
				device_interface.send_byte(section.blue.level3);
			}
			else if (current_row.brightness_step > 0)
			{
				device_interface.send_byte(section.red.level2);
				device_interface.send_byte(section.green.level2);
				device_interface.send_byte(section.blue.level2);
			}
			else if (current_row.brightness_step > 0)
			{
				device_interface.send_byte(section.red.level1);
				device_interface.send_byte(section.green.level1);
				device_interface.send_byte(section.blue.level1);
			}
			else
			{
				device_interface.send_byte(section.red.level0);
				device_interface.send_byte(section.green.level0);
				device_interface.send_byte(section.blue.level0);
			}

		}
		device_interface.complete_row();
	}

	if (++current_row.brightness_step == 4)
	{
		current_row.reset();
		if (++current_row.index == ROWS)
		{
			current_row.index = 0;
		}
	}
}
