#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdint.h>
#include "lib/screen/colors.h"
#include "lib/screen/commands.h"
#include "lib/screen/definitions.h"
#include "interfaces/coreinterface.h"
#include "interfaces/datainterface.h"
#include "interfaces/displayinterface.h"
#include "pixel.h"
#include "row.h"
#include "section.h"

class Screen
{
private:
	const CoreInterface& core_interface;
	const DisplayInterface& display_interface;

	uint8_t buffer_1[SCREEN_HEIGHT][SCREEN_WIDTH];
	uint8_t buffer_2[SCREEN_HEIGHT][SCREEN_WIDTH];
	uint8_t(*active_buffer)[SCREEN_WIDTH];
	uint8_t(*buffer)[SCREEN_WIDTH];

	Row current_row;
	uint8_t rows_state;

	volatile uint8_t is_being_read;


	class Worker
	{
	protected:
		Screen& screen;
		const DataInterface& data_interface;

		Worker(Screen& screen, const DataInterface& data_interface) : screen(screen), data_interface(data_interface) {}

	public:
		virtual ~Worker() {}

		virtual uint8_t do_work() = 0;
	};


	class DefaultWorker : public Worker
	{
	public:
		DefaultWorker(Screen& screen, const DataInterface& data_interface) : Worker(screen, data_interface) {}

		uint8_t do_work();
	};


	class ByteTerminalWorker : public Worker
	{
	public:
		ByteTerminalWorker(Screen& screen, const DataInterface& data_interface) : Worker(screen, data_interface) {}

		uint8_t do_work();
	};


	Worker* const workers[TOTAL_WORKERS];
	Worker* worker;

	void switch_buffer();

	void apply_colors(Pixel& pixel, const uint8_t color, const uint8_t current_bit);

	void apply_colors(Section& section, const uint8_t color, const uint8_t offset);

public:
	Screen(
			const CoreInterface& core_interface,
			const DataInterface& data_interface,
			const DisplayInterface& display_interface
			);

	void launch();

	void stop();

	inline void start_reading();

	inline void stop_reading();

	void draw_row();
};

#endif
