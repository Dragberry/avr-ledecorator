#include "screeninterface.hpp"

ScreenInterface::ScreenInterface(DeviceInterface& device) :
	device_interface(device),
	buffer_1{},
	buffer_2{},
	buffer_3{},
	default_worker(DefaultWorker(*this)),
	byte_terminal_worker(ByteTerminalWorker(*this)),
	workers
	{
		&default_worker,
		&byte_terminal_worker,
	},
	worker(workers[0])
{
	accumulator = buffer_1;
	buffer = buffer_2;
	active_buffer = buffer_3;

	is_buffer_ready = false;

	rows_state = 0;

	device_interface.register_timer_interface(this);
	device_interface.register_data_interface(this);
}

void ScreenInterface::handle_byte(const uint8_t byte, void (*callback)(const uint8_t byte))
{
	callback(byte);
	if (is_command(byte))
	{
		uint8_t command = extract_command(byte);
		if (is_worker_command(command))
		{
			worker = workers[command];
		}
		else
		{
			worker->work_with_command(command);
		}
	}
	else
	{
		worker->work_with_byte(byte);
	}
}

void ScreenInterface::apply_colors(Pixel& pixel, const uint8_t color, const uint8_t currentBit)
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

void ScreenInterface::apply_colors(Section& section, const uint8_t color, const uint8_t offset)
{
	uint8_t currentBit = (1<<offset);
	apply_colors(section.red, (color & RED), currentBit);
	apply_colors(section.green, (color & GREEN) >> 2, currentBit);
	apply_colors(section.blue, (color & BLUE) >> 4, currentBit);
}

void ScreenInterface::on_timer_event()
{
	if (current_row.brightness_step == 0)
	{
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
			if (is_buffer_ready)
			{
				activate_buffer();
				is_buffer_ready = false;
			}
		}
	}
}

void ScreenInterface::switch_accumulator()
{
	uint8_t(*temp)[SCREEN_WIDTH] = accumulator;
	accumulator = buffer;
	buffer = temp;
	is_buffer_ready = true;
}

void ScreenInterface::activate_buffer()
{
	uint8_t(*temp)[SCREEN_WIDTH] = active_buffer;
	active_buffer = buffer;
	buffer = temp;
}
