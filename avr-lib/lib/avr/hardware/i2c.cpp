#include "i2c.hpp"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

// Standard I2C bit rates are:
// 100KHz for slow speed
// 400KHz for high speed

void I2C::init()
{
    //set pull-up resistors on I2C bus pins
    sbi(I2C_PORT, I2C_SCL); // i2c SCL on ATmega163,323,16,32,etc
    sbi(I2C_PORT, I2C_SDA); // i2c SDA on ATmega163,323,16,32,etc
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
    #ifdef I2C_DEBUG
    UART::send_string("I2C: SEND START");
    #endif
    // send start condition
    outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWSTA));
}
//! Send an I2C stop condition in Master mode
inline void I2C::send_stop()
{
    #ifdef I2C_DEBUG
    UART::send_string("I2C: SEND STOP");
    #endif
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
    #ifdef I2C_DEBUG
    UART::send_string("I2C: SEND BYTE");
    UART::send_byte_as_hex(data);
    #endif
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
    #ifdef I2C_DEBUG
    UART::send_string("I2C: RECEIVING BYTE");
    #endif
    // begin receive over i2c
    if(ack_flag)
    {
        #ifdef I2C_DEBUG
        UART::send_string("I2C: RECEING BYTE: ACK");
        #endif
        // ack_flag = TRUE: ACK the recevied data
        outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
    }
    else
    {
        #ifdef I2C_DEBUG
        UART::send_string("I2C: RECEIVING BYTE: NACK");
        #endif
        // ack_flag = FALSE: NACK the recevied data
        outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT));
    }
}
//! Pick up the data that was received with i2cReceiveByte()
inline uint8_t I2C::get_received_byte()
{
    uint8_t byte = inb(TWDR);
    #ifdef I2C_DEBUG
    UART::send_byte_as_hex(byte);
    #endif
    // retieve received data byte from i2c TWDR
    return byte;
}
//! Get current I2c bus status from TWSR
inline uint8_t I2C::get_status()
{
    // retieve current i2c status from i2c TWSR
    return (inb(TWSR));
}

void I2C::master_send(uint8_t device_addr, uint8_t length, uint8_t* data)
{
    #ifdef I2C_DEBUG
    UART::send_string("I2C: MSTR SEND");
    #endif
    uint8_t i;
    // wait for interface to be ready
    while(state);
    // set state
    state = MASTER_TX;
    // save data
    device_addr_rw = (device_addr & 0xFE);  // RW cleared: write operation
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
    #ifdef I2C_DEBUG
    UART::send_string("I2C: MSTR RECEIVE");
    #endif
    uint8_t i;
    // wait for interface to be ready
    while(state);
    // set state
    state = MASTER_RX;
    // save data
    device_addr_rw = (device_addr | 0x01);  // RW set: read operation
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

I2C::Status I2C::master_send_ni(uint8_t device_addr, uint8_t length, uint8_t* data)
{
    #ifdef I2C_DEBUG
    UART::send_string("I2C: MSTR SEND NI");
    #endif
    Status status = OK;

    // disable TWI interrupt
    cbi(TWCR, TWIE);

    // send start condition
    send_start();
    wait_for_complete();

    // send device address with write
    #ifdef I2C_DEBUG
    UART::send_string("I2C: SEND ADDR");
    UART::send_byte_as_hex(device_addr);
    #endif
    send_byte(device_addr & 0xFE);
    wait_for_complete();

    // check if device is present and live
    if(inb(TWSR) == TW_MT_SLA_ACK)
    {
        #ifdef I2C_DEBUG
        UART::send_string("I2C: DEV ALIVE");
        #endif
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
        #ifdef I2C_DEBUG
        UART::send_string("I2C: ERR NODEV");
        #endif
        // device did not ACK it's address,
        // data will not be transferred
        // return error
        status = ERROR_NODEV;
    }

    // transmit stop condition
    // leave with TWEA on for slave receiving
    send_stop();
    while(!(inb(TWCR) & BV(TWSTO)));

    // enable TWI interrupt
    sbi(TWCR, TWIE);

    return status;
}

I2C::Status I2C::master_receive_ni(uint8_t device_addr, uint8_t length, uint8_t *data)
{
    #ifdef I2C_DEBUG
    UART::send_string("I2C: MSTR RECEIVE NI");
    #endif
    Status status = OK;

    // disable TWI interrupt
    cbi(TWCR, TWIE);

    // send start condition
    send_start();
    wait_for_complete();

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
            #ifdef I2C_DEBUG
            UART::send_string("I2C: BYTE RECEIVED");
            #endif
            // decrement length
            length--;
        }

        // accept receive data and nack it (last-byte signal)
        receive_byte(0);
        wait_for_complete();
        *data++ = get_received_byte();
        #ifdef I2C_DEBUG
        UART::send_string("I2C: BYTE RECEIVED");
        #endif
    }
    else
    {
        #ifdef I2C_DEBUG
        UART::send_string("I2C: ERR NODEV");
        #endif
        // device did not ACK it's address,
        // data will not be transferred
        // return error
        status = ERROR_NODEV;
    }

    // transmit stop condition
    // leave with TWEA on for slave receiving
    send_stop();

    // enable TWI interrupt
    sbi(TWCR, TWIE);

    return status;
}

//! High-level read operation to use with typical I2C devices
I2C::Status I2C::device_read(
        uint8_t device_addr,
        uint8_t register_addr,
        uint8_t *data,
        uint8_t length
        )
{
    Status status;
    status = master_send_ni(device_addr, 1, &register_addr);
    if (status == OK)
    {
        status = master_receive_ni(device_addr, length, data);
    }
    return status;
}

//! High-level write operation to use with typical I2C devices
I2C::Status I2C::device_write(
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
    return I2C::master_send_ni(device_addr, length + 1, full_data);
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
    case TW_START:                      // 0x08: Sent start condition
        on_start();
        break;
    case TW_REP_START:                  // 0x10: Sent repeated start condition
        on_repeated_start();
        break;
    // Master Transmitter & Receiver status codes
    case TW_MT_SLA_ACK:                 // 0x18: Slave address acknowledged
        on_slave_address_ack();
        break;
    case TW_MT_DATA_ACK:                // 0x28: Data acknowledged
        on_mt_data_ack();
        break;
    case TW_MR_DATA_NACK:               // 0x58: Data received, NACK reply issued
        on_mr_data_nack();
        break;
    case TW_MR_SLA_NACK:                // 0x48: Slave address not acknowledged
        on_mr_slave_address_nack();
        break;
    case TW_MT_SLA_NACK:                // 0x20: Slave address not acknowledged
        on_mt_slave_address_nack();
        break;
    case TW_MT_DATA_NACK:               // 0x30: Data not acknowledged
        on_mr_data_nack();
        break;
    case TW_MT_ARB_LOST:                // 0x38: Bus arbitration lost
        on_bus_arbitration_lost();
        break;
    case TW_MR_DATA_ACK:                // 0x50: Data acknowledged
        on_mr_data_ack();
        break;
    case TW_MR_SLA_ACK:                 // 0x40: Slave address acknowledged
        on_mr_slave_address_ack();
        break;
    // Slave Receiver status codes
    case TW_SR_SLA_ACK:                 // 0x60: own SLA+W has been received, ACK has been returned
        on_sr_slave_ack();
        break;
    case TW_SR_ARB_LOST_SLA_ACK:        // 0x68: own SLA+W has been received, ACK has been returned
        on_sr_arbitration_lost_slave_ack();
        break;
    case TW_SR_GCALL_ACK:               // 0x70:     GCA+W has been received, ACK has been returned
        on_sr_gcall_slave_ack();
        break;
    case TW_SR_DATA_ACK:                // 0x80: data byte has been received, ACK has been returned
        on_sr_data_ack();
        break;
    case TW_SR_GCALL_DATA_ACK:          // 0x90: data byte has been received, ACK has been returned
        on_sr_gcall_slave_ack();
        break;
    case TW_SR_DATA_NACK:               // 0x88: data byte has been received, NACK has been returned
        on_sr_data_nack();
        break;
    case TW_SR_GCALL_DATA_NACK:         // 0x98: data byte has been received, NACK has been returned
        on_sr_gcall_data_nack();
        break;
    case TW_SR_STOP:                    // 0xA0: STOP or REPEATED START has been received while addressed as slave
        on_sr_stop();
        break;
    // Slave Transmitter
    case TW_ST_SLA_ACK:                 // 0xA8: own SLA+R has been received, ACK has been returned
        on_st_slave_ack();
        break;
    case TW_ST_ARB_LOST_SLA_ACK:        // 0xB0:     GCA+R has been received, ACK has been returned
        on_st_arbitration_lost_slave_ack();
        break;
    case TW_ST_DATA_ACK:                // 0xB8: data byte has been transmitted, ACK has been received
        on_st_data_ack();
        break;
    case TW_ST_DATA_NACK:               // 0xC0: data byte has been transmitted, NACK has been received
        on_st_data_nack();
        break;
    case TW_ST_LAST_DATA:               // 0xC8:
        on_st_last_data();
        break;
    // Misc
    case TW_NO_INFO:                    // 0xF8: No relevant state information
        on_no_info();
        break;
    case TW_BUS_ERROR:                  // 0x00: Bus error due to illegal start or stop condition
        on_bus_error();
        break;
    }
}

ISR(TWI_vect)
{
    I2C::handle();
}
