#ifndef SCREENINTERFACE_HPP_
#define SCREENINTERFACE_HPP_

#include <stdint.h>
#include "lib/screen/colors.h"
#include "lib/screen/commands.h"
#include "lib/screen/definitions.h"

#include "../hardware/datainterface.hpp"
#include "../hardware/deviceinterface.hpp"
#include "../hardware/timerinterface.hpp"
#include "pixel.h"
#include "row.h"
#include "section.h"

class ScreenInterface :
		public DataInterface,
		public TimerInterface
{
private:
	DeviceInterface& device_interface;

	uint8_t buffer_1[SCREEN_HEIGHT][SCREEN_WIDTH];
	uint8_t buffer_2[SCREEN_HEIGHT][SCREEN_WIDTH];
	uint8_t buffer_3[SCREEN_HEIGHT][SCREEN_WIDTH];


	uint8_t(*accumulator)[SCREEN_WIDTH];
	uint8_t(*buffer)[SCREEN_WIDTH];
	uint8_t(*active_buffer)[SCREEN_WIDTH];

	volatile bool is_buffer_ready;

	Row current_row;
	uint8_t rows_state;

	void apply_colors(Pixel& pixel, const uint8_t color, const uint8_t current_bit);

	void apply_colors(Section& section, const uint8_t color, const uint8_t offset);

public:
	ScreenInterface(DeviceInterface& device_interface);

	~ScreenInterface() { }

	void handle_byte(const uint8_t byte);

	void on_timer_event();

	void switch_accumulator();

	void activate_buffer();

private:
	class Worker
	{
	protected:
		ScreenInterface& screen_interface;

		Worker(ScreenInterface& screen_interface) :
					screen_interface(screen_interface) { }

	public:
		virtual ~Worker() { }

		virtual uint8_t work_with_command(const uint8_t command) = 0;

		virtual uint8_t work_with_byte(const uint8_t byte) = 0;
	};

	class DefaultWorker : public Worker
	{
	private:
		uint8_t x = 0;
		uint8_t y = 0;

	public:
		DefaultWorker(ScreenInterface& screen_interface) :
			Worker(screen_interface) { }

		~DefaultWorker() { }

		uint8_t work_with_command(const uint8_t command);

		uint8_t work_with_byte(const uint8_t byte);
	} default_worker;

	class ByteTerminalWorker : public Worker
	{
	private:
		uint8_t x = 0;
		uint8_t cr = 0;
		uint8_t lf = 0;

	public:
		ByteTerminalWorker(ScreenInterface& screen_interface) :
			Worker(screen_interface) { }

		~ByteTerminalWorker() { }

		uint8_t work_with_command(const uint8_t command);

		uint8_t work_with_byte(const uint8_t byte);
	} byte_terminal_worker;

	Worker* workers[2];
	Worker* worker;
};

#endif
