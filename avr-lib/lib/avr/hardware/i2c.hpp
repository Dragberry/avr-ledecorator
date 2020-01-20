#ifndef I2C_H
#define I2C_H

#define 	outb(addr, data)  	addr = (data)
#define 	inb(addr)   		(addr)
#define 	outw(addr, data)  	addr = (data)
#define 	inw(addr)   		(addr)
#define 	BV(bit)   			(1<<(bit))
#define 	cbi(reg, bit)   	reg &= ~(BV(bit))
#define 	sbi(reg, bit)   	reg |= (BV(bit))

#define FALSE	0
#define TRUE	-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include "i2cslavehandler.h"

// TWSR values (not bits)
#define TW_START					0x08
#define TW_REP_START				0x10
// Master Transmitter
#define TW_MT_SLA_ACK				0x18
#define TW_MT_SLA_NACK				0x20
#define TW_MT_DATA_ACK				0x28
#define TW_MT_DATA_NACK				0x30
#define TW_MT_ARB_LOST				0x38
// Master Receiver
#define TW_MR_ARB_LOST				0x38
#define TW_MR_SLA_ACK				0x40
#define TW_MR_SLA_NACK				0x48
#define TW_MR_DATA_ACK				0x50
#define TW_MR_DATA_NACK				0x58
// Slave Transmitter
#define TW_ST_SLA_ACK				0xA8
#define TW_ST_ARB_LOST_SLA_ACK		0xB0
#define TW_ST_DATA_ACK				0xB8
#define TW_ST_DATA_NACK				0xC0
#define TW_ST_LAST_DATA				0xC8
// Slave Receiver
#define TW_SR_SLA_ACK				0x60
#define TW_SR_ARB_LOST_SLA_ACK		0x68
#define TW_SR_GCALL_ACK				0x70
#define TW_SR_ARB_LOST_GCALL_ACK	0x78
#define TW_SR_DATA_ACK				0x80
#define TW_SR_DATA_NACK				0x88
#define TW_SR_GCALL_DATA_ACK		0x90
#define TW_SR_GCALL_DATA_NACK		0x98
#define TW_SR_STOP					0xA0
// Misc
#define TW_NO_INFO					0xF8
#define TW_BUS_ERROR				0x00

// defines and constants
#define TWCR_CMD_MASK		0x0F
#define TWSR_STATUS_MASK	0xF8

#define I2C_SEND_DATA_BUFFER_SIZE 4
#define I2C_RECEIVE_DATA_BUFFER_SIZE 4

//#define I2C_DEBUG
//#define I2C_DEBUG_INT

#if defined(I2C_DEBUG ) || defined(I2C_DEBUG_INT)
#include "uart.hpp"
#endif

#if defined (__AVR_ATmega16__)
#define I2C_PORT PORTC
#define I2C_SCL PC0
#define I2C_SDA PC1
#elif defined (__AVR_ATmega328P__)
#define I2C_PORT PORTC
#define I2C_SCL PC5
#define I2C_SDA PC4
#else
#  if !defined(__COMPILING_AVR_LIBC__)
#    warning "device type not defined"
#  endif
#endif

namespace I2C {
    enum Status
    {
       OK,
       ERROR_NODEV

    };

    enum State
	{
		IDLE = 0,
		BUSY = 1,
		MASTER_TX = 2,
		MASTER_RX = 3,
		SLAVE_TX = 4,
		SLAVE_RX = 5
	};

	static volatile State state;
	// send/transmit buffer (outgoing data)
	static uint8_t device_addr_rw;
	static uint8_t send_data[I2C_SEND_DATA_BUFFER_SIZE];
	static uint8_t send_data_index;
	static uint8_t send_data_length;
	// receive buffer (incoming data)
	static uint8_t receive_data[I2C_RECEIVE_DATA_BUFFER_SIZE];
	static uint8_t receive_data_index;
	static uint8_t receive_data_length;

	// Set this handler to handle slave
	static SlaveHandler* slave_handler;
	//! Initialize I2C (TWI) interface
	void init();

	//! Set the I2C transaction bitrate (in KHz)
	void set_bitrate(uint16_t bitrate_khz);

	// I2C setup and configurations commands
	//! Set the local (AVR processor's) I2C device address
	void set_local_device_addr(uint8_t device_addr, uint8_t gen_call_en);

	//! Set the user handler which handles receiving and transmitting data as a slave
	void set_slave_handler(SlaveHandler* slave_handler);

	// Low-level I2C transaction commands
	//! Send an I2C start condition in Master mode
	void send_start();
	//! Send an I2C stop condition in Master mode
	void send_stop();
	//! Wait for current I2C operation to complete
	void wait_for_complete();
	//! Send an (address|R/W) combination or a data byte over I2C
	void send_byte(uint8_t data);
	//! Receive a data byte over I2C
	// ackFlag = TRUE if recevied data should be ACK'ed
	// ackFlag = FALSE if recevied data should be NACK'ed
	void receive_byte(uint8_t ack_flag);
	//! Pick up the data that was received with i2cReceiveByte()
	uint8_t get_received_byte();
	//! Get current I2c bus status from TWSR
	uint8_t get_status();

	// high-level I2C transaction commands
	//! send I2C data to a device on the bus
	void master_send(uint8_t device_addr, uint8_t length, uint8_t *data);
	//! receive I2C data from a device on the bus
	void master_receive(uint8_t device_addr, uint8_t length, uint8_t* data);

	//! send I2C data to a device on the bus (non-interrupt based)
	Status master_send_ni(uint8_t device_addr, uint8_t length, uint8_t* data);
	//! receive I2C data from a device on the bus (non-interrupt based)
	Status master_receive_ni(uint8_t device_addr, uint8_t length, uint8_t *data);


    //! High-level read operation to use with typical I2C devices
    template <typename TemplateStatus, TemplateStatus OK, TemplateStatus DEV_NOT_FOUND>
    TemplateStatus device_read(
            uint8_t device_addr,
            uint8_t register_addr,
            uint8_t *data,
            uint8_t length
            )
    {
        Status status = master_send_ni(device_addr, 1, &register_addr);
        if (status == Status::OK)
        {
            status = master_receive_ni(device_addr, length, data);
        }
        return status == Status::OK ? OK : DEV_NOT_FOUND;
    }

    //! High-level write operation to use with typical I2C devices
    template <typename TemplateStatus, TemplateStatus OK, TemplateStatus DEV_NOT_FOUND>
    TemplateStatus device_write(
            uint8_t device_addr,
            uint8_t register_addr,
            uint8_t *data,
            uint8_t length
          )
    {
        uint8_t full_data[length + 1];
        full_data[0] = register_addr;
        uint8_t idx = 1;
        while (idx <= length)
        {
            full_data[idx] = data[idx - 1];
            idx++;
        }
        return I2C::master_send_ni(device_addr, length + 1, full_data) == Status::OK ? OK : DEV_NOT_FOUND;
    }

	//! Get the current high-level state of the I2C interface
	State get_state();

	//! I2C (TWI) interrupt service routine
	void handle();

	inline void on_star_common()
	{
		// send device address
		send_byte(device_addr_rw);
	}

	inline void on_start()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: M->START");
			UART::send_string("I2C: M->SEND ADDRESS");
			UART::send_byte_as_binary(device_addr_rw);
		#endif
		on_star_common();
	}

	inline void on_repeated_start()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: M->REP_START");
			UART::send_string("I2C: M->SEND ADDRESS");
			UART::send_byte_as_binary(device_addr_rw);
		#endif
		on_star_common();
	}


	inline void on_ack()
	{
		if(send_data_index < send_data_length)
		{
			// send data
			send_byte(send_data[send_data_index]);
			#ifdef I2C_DEBUG_INT
				UART::send_string("I2C: MT->SEND BYTE");
				UART::send_byte_as_binary(send_data[send_data_index]);
			#endif
			send_data_index++;
		}
		else
		{
			#ifdef I2C_DEBUG_INT
				UART::send_string("I2C: MT->SEND STOP");
			#endif
			// transmit stop condition, enable SLA ACK
			send_stop();
			// set state
			state = IDLE;
		}
	}

	inline void on_slave_address_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: MT->SLA_ACK");
		#endif
		on_ack();
	}

	inline void on_mt_data_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: MT->DATA_ACK");
		#endif
		on_ack();
	}

	inline void on_nack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: SEND STOP");
		#endif
		// transmit stop condition, enable SLA ACK
		send_stop();
		// set state
		state = IDLE;
	}

	inline void on_mr_data_nack()
	{
		// store final received data byte
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: MR->DATA_NACK");
			UART::send_string("I2C: MR->DATA_NACK: LAST RECEIVED BYTE:");
		#endif
		receive_data[receive_data_index++] = inb(TWDR);
		#ifdef I2C_DEBUG_INT
			UART::send_byte_as_binary(receive_data[receive_data_index - 1]);
		#endif
		on_nack();
	}

	inline void on_mr_slave_address_nack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: MR->SLA_NACK");
		#endif
		on_nack();
	}

	inline void on_mt_slave_address_nack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: MT->SLA_NACK");
		#endif
		on_nack();
	}

	inline void on_mt_data_nack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: MT->DATA_NACK");
		#endif
		on_nack();
	}

	inline void on_bus_arbitration_lost()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: MT->ARB_LOST");
		#endif
		// release bus
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT));
		// set state
		state = IDLE;
	}

	inline void on_mr_ack()
	{
		if(receive_data_index < (receive_data_length - 1))
		{	// data byte will be received, reply with ACK (more bytes in transfer)
			receive_byte(TRUE);
		}
		else
		{
			// data byte will be received, reply with NACK (final byte in transfer)
			receive_byte(FALSE);
		}
	}

	inline void on_mr_data_ack()
	{
		// store received data byte
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: MR->DATA_ACK: RECEIVED BYTE: ");
		#endif
			receive_data[receive_data_index++] = inb(TWDR);
		#ifdef I2C_DEBUG_INT
			UART::send_byte_as_binary(receive_data[receive_data_index - 1]);
		#endif
		on_mr_ack();
	}

	inline void on_mr_slave_address_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: MR->SLA_ACK");
		#endif
		on_mr_ack();
	}

	inline void on_sr_ack()
	{
		// we are being addressed as slave for writing (data will be received from master)
		// set state
		state = SLAVE_RX;
		// prepare buffer
		receive_data_index = 0;
		// receive data byte and return ACK
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
	}

	inline void on_sr_slave_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: SR->SLA_ACK");
		#endif
		on_sr_ack();
	}

	inline void on_sr_arbitration_lost_slave_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: SR->ARB_LOST_SLA_ACK");
		#endif
		on_sr_ack();
	}

	inline void on_sr_gcall_slave_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: SR->GCALL_ACK");
		#endif
		on_sr_ack();
	}

	inline void on_sr_data_ack_common()
	{
		// get previously received data byte
		receive_data[receive_data_index++] = inb(TWDR);
		// check receive buffer status
		if(receive_data_index < I2C_RECEIVE_DATA_BUFFER_SIZE)
		{
			// receive data byte and return ACK
			receive_byte(TRUE);
		}
		else
		{
			// receive data byte and return NACK
			receive_byte(FALSE);
		}
	}

	inline void on_sr_data_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: SR->DATA_ACK");
		#endif
		on_sr_data_ack_common();
	}

	inline void on_sr_gcall_data_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: SR->GCALL_DATA_ACK");
		#endif
		on_sr_data_ack_common();
	}

	inline void on_sr_data_nack_common()
	{
		receive_byte(FALSE);
	}

	inline void on_sr_data_nack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: SR->DATA_NACK");
		#endif
		on_sr_data_nack_common();
	}

	inline void on_sr_gcall_data_nack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: SR->GCALL_DATA_NACK");
		#endif
		on_sr_data_nack_common();
	}

	inline void on_sr_stop()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: SR->STOP");
		#endif
		// switch to SR mode with SLA ACK
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
		// i2c receive is complete, call i2cSlaveReceive
		if (slave_handler != NULL)
		{
			slave_handler->handle_recieve(receive_data_index, receive_data);
		}
		// set state
		state = IDLE;
	}

	inline void on_st_slave_data_ack_common()
	{
		// transmit data byte
		outb(TWDR, send_data[send_data_index++]);
		if(send_data_index < send_data_length)
		{
			// expect ACK to data byte
			outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
		}
		else
		{
			// expect NACK to data byte
			outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT));
		}
	}

	inline void on_st_slave_ack_common()
	{
		// we are being addressed as slave for reading (data must be transmitted back to master)
		// set state
		state = SLAVE_TX;
		// request data from application
		if(slave_handler != NULL)
		{
			send_data_length = slave_handler->handle_transmit(I2C_SEND_DATA_BUFFER_SIZE, send_data);
		}
		// reset data index
		send_data_index = 0;
		// fall-through to transmit first data byte
		on_st_slave_data_ack_common();
	}

	inline void on_st_slave_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: ST->SLA_ACK");
		#endif
		on_st_slave_ack_common();
	}

	inline void on_st_arbitration_lost_slave_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: ST->ARB_LOST_SLA_ACK");
		#endif
		on_st_slave_ack_common();
	}

	inline void on_st_data_ack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: ST->DATA_ACK");
		#endif
		on_st_slave_data_ack_common();
	}

	inline void on_st_data_nack_common()
	{
		// all done
		// switch to open slave
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
		// set state
		state = IDLE;
	}

	inline void on_st_data_nack()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: ST->DATA_NACK");
		#endif
		on_st_data_nack_common();
	}

	inline void on_st_last_data()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: ST->LAST_DATA");
		#endif
		on_st_data_nack_common();
	}

	inline void on_no_info()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: NO_INFO");
		#endif
		// do nothing
	}

	inline void on_bus_error()
	{
		#ifdef I2C_DEBUG_INT
			UART::send_string("I2C: BUS_ERROR");
		#endif
		// reset internal hardware and release bus
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWSTO) | BV(TWEA));
		// set state
		state = IDLE;
	}
};

#endif
