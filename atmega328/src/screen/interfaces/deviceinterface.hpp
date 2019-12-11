#ifndef DEVICEINTERFACE_HPP_
#define DEVICEINTERFACE_HPP_

#include <stdint.h>
#include <stdlib.h>

class DeviceInterface
{
public:
	class RowDrawer
	{
	public:
		virtual ~RowDrawer() { }

		virtual void draw_row() = 0;
	};

	RowDrawer* row_drawer = NULL;

	virtual ~DeviceInterface() {}

	virtual void launch(RowDrawer* row_drawer) = 0;

	virtual void stop() = 0;

	virtual void start_row() const = 0;

	virtual void send_byte(const uint8_t byte) const = 0;

	virtual void complete_row() const = 0;

	virtual uint8_t get_data_byte() const = 0;

	virtual void send_data_byte(const uint8_t byte) const = 0;

};

#endif
