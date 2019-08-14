#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "device/screen.h"

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_SCK PB7
#define SPI_MOSI PB5
#define SPI_SS PB4

#define SECTION_WIDTH 8
#define SECTION_HEIGHT 8

#define SECTIONS_WIDTH 1
#define SECTIONS_HEIGHT 1

#define SCREEN_WIDTH (SECTION_WIDTH * SECTIONS_WIDTH)
#define SCREEN_HEIGHT (SECTION_HEIGHT * SECTIONS_HEIGHT)

void initSPI();
void resetSPI();
void confirmSPI();
void sendByte(const uint8_t data);

void initScreen();
void initScreenTimer();
void stopScreenTimer();
void shutDownScreen();

Screen* screen;

int main() {
	initSPI();
	initScreen();
	initScreenTimer();
	sei();

	while(1);

	return 0;
}

void initSPI() {
	SPI_DDR |= (1 << SPI_SS) | (1 << SPI_MOSI) | (1 << SPI_SCK);
	SPI_PORT |= (1 << SPI_SS);
	SPCR |= (1 << SPE) | (1 << MSTR) | (0 << DORD);
	SPSR |= (1 << SPI2X);
}

void resetSPI() {
	SPI_PORT &= ~(1 << SPI_SS);
}

void confirmSPI() {
	SPI_PORT |= (1 << SPI_SS);
}

void sendByte(const uint8_t data) {
	SPDR = data;
	while (!(SPSR & (1 << SPIF )));
}

void initScreen() {
	screen = new Screen(SECTION_HEIGHT, SECTION_WIDTH, SECTIONS_HEIGHT, SECTIONS_WIDTH);
}

void initScreenTimer() {
	TCCR0 |= (1 << WGM01) | (0 << CS02) | (1 << CS01) | (1 << CS00);
	TIMSK |= (1 << OCIE0);
	OCR0 = 0x08;
}

void stopScreenTimer() {
	TCCR0 = 0;
	TIMSK &= ~(1 << OCIE0);
	OCR0 = 0;
}

void shutDownScreen() {
	stopScreenTimer();
	delete []screen;
}

ISR(TIMER0_COMP_vect) {
	resetSPI();
	screen->drawRow(sendByte);
	confirmSPI();
}
