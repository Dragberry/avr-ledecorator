#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "i2c.h"

// Standard I2C bit rates are:
// 100KHz for slow speed
// 400KHz for high speed

void I2C::init()
{
	//set pull-up resistors on I2C bus pins
	sbi(PORTC, PC5);	// i2c SCL on ATmega163,323,16,32,etc
	sbi(PORTC, PC4);	// i2c SDA on ATmega163,323,16,32,etc
	// clear SlaveReceive and SlaveTransmit handler to null
	slave_handler = NULL;
	// set i2c bit rate to 100KHz
	set_bitrate(100);
	// enable TWI (two-wire interface)
	sbi(TWCR, TWEN);
	state = IDLE;
	// enable TWI interrupt and slave address ACK
	sbi(TWCR, TWIE);
	sbi(TWCR, TWEA);
	//enable interrupts
	sei();
}

void I2C::set_bitrate(uint16_t bitrate_khz)
{
	uint8_t bitrate_div;
	// set i2c bitrate
	// SCL freq = F_CPU/(16+2*TWBR))
	// calculate bitrate division
	bitrate_div = ((F_CPU / 1000l) / bitrate_khz);
	if(bitrate_div >= 16)
		bitrate_div = (bitrate_div - 16) / 2;
	outb(TWBR, bitrate_div);
}

void I2C::set_local_device_addr(uint8_t device_addr, uint8_t gen_call_en)
{
	// set local device address (used in slave mode only)
	outb(TWAR, ((device_addr & 0xFE) | (gen_call_en ? 1 : 0)));
}

void I2C::set_slave_handler(SlaveHandler* handler)
{
	slave_handler = handler;
}

inline void I2C::send_start()
{
	// send start condition
	outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWSTA));
}
//! Send an I2C stop condition in Master mode
inline void I2C::send_stop()
{
	// transmit stop condition
	// leave with TWEA on for slave receiving
	outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA) | BV(TWSTO));
}
//! Wait for current I2C operation to complete
inline void I2C::wait_for_complete()
{
	// wait for i2c interface to complete operation
	while(!(inb(TWCR) & BV(TWINT)));
}
//! Send an (address|R/W) combination or a data byte over I2C
inline void I2C::send_byte(uint8_t data)
{
	// save data to the TWDR
	outb(TWDR, data);
	// begin send
	outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT));
}
//! Receive a data byte over I2C
// ackFlag = TRUE if recevied data should be ACK'ed
// ackFlag = FALSE if recevied data should be NACK'ed
inline void I2C::receive_byte(uint8_t ack_flag)
{
	// begin receive over i2c
	if(ack_flag)
	{
		// ack_flag = TRUE: ACK the recevied data
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
	}
	else
	{
		// ack_flag = FALSE: NACK the recevied data
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT));
	}
}
//! Pick up the data that was received with i2cReceiveByte()
inline uint8_t I2C::get_received_byte()
{
	// retieve received data byte from i2c TWDR
	return (inb(TWDR));
}
//! Get current I2c bus status from TWSR
inline uint8_t I2C::get_status()
{
	// retieve current i2c status from i2c TWSR
	return (inb(TWSR));
}

void I2C::master_send(uint8_t device_addr, uint8_t length, uint8_t* data)
{
	uint8_t i;
	// wait for interface to be ready
	while(state);
	// set state
	state = MASTER_TX;
	// save data
	device_addr_rw = (device_addr & 0xFE);	// RW cleared: write operation
	for(i = 0; i < length; i++)
	{
		send_data[i] = *data++;
	}
	send_data_index = 0;
	send_data_length = length;
	// send start condition
	send_start();
}

void I2C::master_receive(uint8_t device_addr, uint8_t length, uint8_t* data)
{
	uint8_t i;
	// wait for interface to be ready
	while(state);
	// set state
	state = MASTER_RX;
	// save data
	device_addr_rw = (device_addr | 0x01);	// RW set: read operation
	receive_data_index = 0;
	receive_data_length = length;
	// send start condition
	send_start();
	// wait for data
	while(state);
	// return data
	for(i = 0; i < length; i++)
	{
		*data++ = receive_data[i];
	}
}

uint8_t I2C::master_send_ni(uint8_t device_addr, uint8_t length, uint8_t* data)
{
	uint8_t retval = I2C_OK;

	// disable TWI interrupt
	cbi(TWCR, TWIE);

	// send start condition
	send_start();
	wait_for_complete();

	// send device address with write
	send_byte(device_addr & 0xFE);
	wait_for_complete();

	// check if device is present and live
	if(inb(TWSR) == TW_MT_SLA_ACK)
	{
		// send data
		while(length)
		{
			send_byte(*data++);
			wait_for_complete();
			length--;
		}
	}
	else
	{
		// device did not ACK it's address,
		// data will not be transferred
		// return error
		retval = I2C_ERROR_NODEV;
	}

	// transmit stop condition
	// leave with TWEA on for slave receiving
	send_stop();
	while(!(inb(TWCR) & BV(TWSTO)));

	// enable TWI interrupt
	sbi(TWCR, TWIE);

	return retval;
}

uint8_t I2C::master_receive_ni(uint8_t device_addr, uint8_t length, uint8_t *data)
{
	cbi(PORTC, PC0);
	cbi(PORTC, PC1);
	cbi(PORTC, PC2);
	cbi(PORTC, PC3);


	uint8_t retval = I2C_OK;

	// disable TWI interrupt
	cbi(TWCR, TWIE);

	// send start condition

	sbi(PORTC, PC0);

	send_start();
	wait_for_complete();

	sbi(PORTC, PC1);
	// send device address with read
	send_byte(device_addr | 0x01);
	wait_for_complete();

	// check if device is present and live
	if(inb(TWSR) == TW_MR_SLA_ACK)
	{
		sbi(PORTC, PC2);
		// accept receive data and ack it
		while(length > 1)
		{
			receive_byte(-1);
			wait_for_complete();
			*data++ = get_received_byte();
			// decrement length
			length--;
		}

		// accept receive data and nack it (last-byte signal)
		receive_byte(0);
		wait_for_complete();
		*data++ = get_received_byte();
	}
	else
	{
		sbi(PORTC, PC4);
		// device did not ACK it's address,
		// data will not be transferred
		// return error
		retval = I2C_ERROR_NODEV;
	}

	// transmit stop condition
	// leave with TWEA on for slave receiving
	send_stop();

	// enable TWI interrupt
	sbi(TWCR, TWIE);

	return retval;
}

I2C::State I2C::get_state()
{
	return state;
}

void I2C::handle()
{
	// read status bits
	uint8_t status = inb(TWSR) & TWSR_STATUS_MASK;

	switch(status)
	{
	// Master General
	case TW_START:						// 0x08: Sent start condition
	case TW_REP_START:					// 0x10: Sent repeated start condition
		// send device address
		send_byte(device_addr_rw);
		break;

	// Master Transmitter & Receiver status codes
	case TW_MT_SLA_ACK:					// 0x18: Slave address acknowledged
	case TW_MT_DATA_ACK:				// 0x28: Data acknowledged
		if(send_data_index < send_data_length)
		{
			// send data
			send_byte(send_data[send_data_index++]);
		}
		else
		{
			// transmit stop condition, enable SLA ACK
			send_stop();
			// set state
			state = IDLE;
		}
		break;
	case TW_MR_DATA_NACK:				// 0x58: Data received, NACK reply issued
		// store final received data byte
		receive_data[receive_data_index++] = inb(TWDR);
		// continue to transmit STOP condition
	case TW_MR_SLA_NACK:				// 0x48: Slave address not acknowledged
	case TW_MT_SLA_NACK:				// 0x20: Slave address not acknowledged
	case TW_MT_DATA_NACK:				// 0x30: Data not acknowledged
		// transmit stop condition, enable SLA ACK
		send_stop();
		// set state
		state = IDLE;
		break;
	case TW_MT_ARB_LOST:				// 0x38: Bus arbitration lost
		// release bus
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT));
		// set state
		state = IDLE;
		break;
	case TW_MR_DATA_ACK:				// 0x50: Data acknowledged
		// store received data byte
		receive_data[receive_data_index++] = inb(TWDR);
		// fall-through to see if more bytes will be received
	case TW_MR_SLA_ACK:					// 0x40: Slave address acknowledged
		if(receive_data_index < (receive_data_length - 1))
		{	// data byte will be received, reply with ACK (more bytes in transfer)
			receive_byte(TRUE);
		}
		else
		{
			// data byte will be received, reply with NACK (final byte in transfer)
			receive_byte(FALSE);
		}
		break;
	// Slave Receiver status codes
	case TW_SR_SLA_ACK:					// 0x60: own SLA+W has been received, ACK has been returned
	case TW_SR_ARB_LOST_SLA_ACK:		// 0x68: own SLA+W has been received, ACK has been returned
	case TW_SR_GCALL_ACK:				// 0x70:     GCA+W has been received, ACK has been returned
		// we are being addressed as slave for writing (data will be received from master)
		// set state
		state = SLAVE_RX;
		// prepare buffer
		receive_data_index = 0;
		// receive data byte and return ACK
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
		break;
	case TW_SR_DATA_ACK:				// 0x80: data byte has been received, ACK has been returned
	case TW_SR_GCALL_DATA_ACK:			// 0x90: data byte has been received, ACK has been returned
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
		break;
	case TW_SR_DATA_NACK:				// 0x88: data byte has been received, NACK has been returned
	case TW_SR_GCALL_DATA_NACK:			// 0x98: data byte has been received, NACK has been returned
		// receive data byte and return NACK
		receive_byte(FALSE);
		break;
	case TW_SR_STOP:					// 0xA0: STOP or REPEATED START has been received while addressed as slave
		// switch to SR mode with SLA ACK
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
		// i2c receive is complete, call i2cSlaveReceive
		if (slave_handler != NULL)
		{
			slave_handler->handle_recieve(receive_data_index, receive_data);
		}
		// set state
		state = IDLE;
		break;

	// Slave Transmitter
	case TW_ST_SLA_ACK:					// 0xA8: own SLA+R has been received, ACK has been returned
	case TW_ST_ARB_LOST_SLA_ACK:		// 0xB0:     GCA+R has been received, ACK has been returned
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
	case TW_ST_DATA_ACK:				// 0xB8: data byte has been transmitted, ACK has been received
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
		break;
	case TW_ST_DATA_NACK:				// 0xC0: data byte has been transmitted, NACK has been received
	case TW_ST_LAST_DATA:				// 0xC8:
		// all done
		// switch to open slave
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
		// set state
		state = IDLE;
		break;
	// Misc
	case TW_NO_INFO:					// 0xF8: No relevant state information
		// do nothing
		break;
	case TW_BUS_ERROR:					// 0x00: Bus error due to illegal start or stop condition
		// reset internal hardware and release bus
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWSTO) | BV(TWEA));
		// set state
		state = IDLE;
		break;
	}
}

ISR(TWI_vect)
{
	I2C::handle();
}
