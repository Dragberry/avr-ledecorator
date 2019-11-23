#ifndef I2C_H
#define I2C_H

#define 	outb(addr, data)  	addr = (data)
#define 	inb(addr)   		(addr)
#define 	outw(addr, data)  	addr = (data)
#define 	inw(addr)   		(addr)
#define 	BV(bit)   			(1<<(bit))
#define 	cbi(reg, bit)   	reg &= ~(BV(bit))
#define 	sbi(reg, bit)   	reg |= (BV(bit))

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
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

// return values
#define I2C_OK				0x00
#define I2C_ERROR_NODEV		0x01

#define I2C_SEND_DATA_BUFFER_SIZE 16
#define I2C_RECEIVE_DATA_BUFFER_SIZE 16

namespace I2C {
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
	uint8_t master_send_ni(uint8_t device_addr, uint8_t length, uint8_t* data);
	//! receive I2C data from a device on the bus (non-interrupt based)
	uint8_t master_receive_ni(uint8_t device_addr, uint8_t length, uint8_t *data);

	//! Get the current high-level state of the I2C interface
	State get_state();

	//! I2C (TWI) interrupt service routine
	void handle();
};

#endif
