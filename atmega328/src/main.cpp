#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "screen/defenitions.h"
#include "screen/color.h"
#include "screen/section.h"
#include "screen/row.h"

#define FCPU 20000000UL
#define USART_BAUDRATE  9600UL
#define UBRR ((FCPU / (USART_BAUDRATE * 16UL)) - 1)

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_SCK PB5
#define SPI_MOSI PB3
#define SPI_SS PB2

const uint8_t D_0 = 0b00111111;
const uint8_t D_1 = 0b00000110;
const uint8_t D_2 = 0b01011011;
const uint8_t D_3 = 0b01001111;
const uint8_t D_4 = 0b01100110;
const uint8_t D_5 = 0b01101101;
const uint8_t D_6 = 0b01111101;
const uint8_t D_7 = 0b00000111;
const uint8_t D_8 = 0b01111111;
const uint8_t D_9 = 0b01101111;

const uint8_t DIGITS[10] = { D_0, D_1, D_2, D_3, D_4, D_5, D_6, D_7, D_8, D_9 };



uint8_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {};
uint8_t buffer2[SCREEN_HEIGHT][SCREEN_WIDTH] = {};

void setup();

void loop();

void initRowTimer() {
	// CTC
	TCCR0A |= (0<<WGM00);
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (0<<WGM02);
	// 001 - f
	// 010 - f/8
	// 011 - f/64
	// 100 - f/256
	// 101 - f/1024
	TCCR0B |= (1<<CS02);
	TCCR0B |= (0<<CS01);
	TCCR0B |= (1<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	// 0.2s
	OCR0A = 0x4;
}

void initEventTimer() {
	// CTC
	TCCR1A |= (0<<WGM10);
	TCCR1A |= (0<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B |= (0<<WGM13);
	// 000 - f
	// 100 - f/256
	// 101 - f/1024
	TCCR1B |= (1<<CS12);
	TCCR1B |= (0<<CS11);
	TCCR1B |= (1<<CS10);
	TIMSK1 |= (1<<OCIE1A);
	// 0.2s
	OCR1A = 0x3D09;
}

void timers_init()
{
	initRowTimer();
	initEventTimer();
}

void USART_init(uint16_t ubrr) {
	UBRR0H = (uint8_t) (ubrr >> 8);
	UBRR0L = (uint8_t) ubrr;

//	UCSR0B |= (1<<TXEN0);
//	UCSR0B |= (1<<TXCIE0);

	UCSR0B |= (1<<RXEN0);
	UCSR0B |= (1<<RXCIE0);

	// 1 stop bit
	UCSR0C |= (0<<USBS0);
	// 8 bit
	UCSR0C |= (1<<UCSZ00);
	UCSR0C |= (1<<UCSZ01);
}

void USART_transmit(const uint8_t data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

uint8_t USART_receive()
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

void SPI_init()
{
	SPI_DDR |= (1<<SPI_SS) | (1<<SPI_MOSI) | (1<<SPI_SCK);
	SPI_PORT |= (1<<SPI_SS);
	SPCR |= (1<<SPE) | (1<<MSTR);
	SPSR |= (1<<SPI2X);
}

void ports_init()
{
	DDRC = ROW_L_MASK;
	DDRB = ROW_H_MASK;
}


void setup() {
	ports_init();
	SPI_init();
	timers_init();
//	USART_init(UBRR);
	sei();

	buffer[0][0] = 0;
	buffer[0][1] = RED;
	buffer[0][2] = YELLOW;
	buffer[0][3] = GREEN;
	buffer[0][4] = CYAN;
	buffer[0][5] = BLUE;
	buffer[0][6] = MAGENTA;
	buffer[0][7] = WHITE;


	buffer[1][0] = RED;
	buffer[2][0] = YELLOW;
	buffer[3][0] = GREEN;
	buffer[4][0] = CYAN;
	buffer[5][0] = BLUE;
	buffer[6][0] = MAGENTA;
	buffer[7][0] = WHITE;


	buffer[8][0] = 0;
	buffer[8][1] = RED;
	buffer[8][2] = YELLOW;
	buffer[8][3] = GREEN;
	buffer[8][4] = CYAN;
	buffer[8][5] = BLUE;
	buffer[8][6] = MAGENTA;
	buffer[8][7] = WHITE;


	buffer[9][0] = RED;
	buffer[10][0] = YELLOW;
	buffer[11][0] = GREEN;
	buffer[12][0] = CYAN;
	buffer[13][0] = BLUE;
	buffer[14][0] = MAGENTA;
	buffer[15][0] = WHITE;


	buffer[9][8] = RED;
	buffer[10][8] = YELLOW;
	buffer[11][8] = GREEN;
	buffer[12][8] = CYAN;
	buffer[13][8] = BLUE;
	buffer[14][8] = MAGENTA;
	buffer[15][8] = WHITE;

	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
		{
			buffer[y][x] = 0b00010101;
		}
	}
}

void SPI_reset() {
	SPI_PORT &= ~(1<<SPI_SS);
}

void confirmSPI() {
	SPI_PORT |= (1<<SPI_SS);
}

void sendByteBySPI(const uint8_t data) {
	SPDR = data;
	while (!(SPSR & (1<<SPIF)));
}


void loop()
{
//	uint8_t data = USART_receive();
//	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
//	{
//		for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
//		{
//			uint8_t data = 0;
//			do
//			{
//				data = USART_receive();
//			} while(data == '\r' || data == '\n');
//
//			buffer[y][x] = data;
//		}
//	}
}

int main()
{
	setup();
	SPI_reset();
	while(1)
	{
		loop();
	}
	return 0;
}

void applyColors(Color& color, const uint8_t colorValue, const uint8_t currentBit) {
	if (colorValue > 0b00) {
		color.level0 |= currentBit;
		color.level1 |= currentBit;
		if (colorValue > 0b01) {
			color.level2 |= currentBit;
			if (colorValue > 0b10) {
				color.level3 |= currentBit;
			}
		}
	}
}

void applyColors(Section& section, const uint8_t value, const uint8_t offset) {
	uint8_t currentBit = (1<<offset);
	applyColors(section.red, (value & RED), currentBit);
	applyColors(section.green, (value & GREEN) >> 2, currentBit);
	applyColors(section.blue, (value & BLUE) >> 4, currentBit);
}

Row row = Row();

// row timer, 20ms
ISR(TIMER0_COMPA_vect) {
	if (row.step == 0) {
		row.reset();
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++) {
			// 0,1,2,3,4,5,6,7...
			uint8_t sectionRowIdx = sectionIdx / SECTIONS_PER_ROW;
			// 0,0,0,0,1,1,1,1...
			uint8_t pictureY = (H_SECTION * sectionRowIdx) + row.index;
			if (sectionRowIdx % 2 == 0) {
				// not-inverted
				// sectionRowIdx: 0
				uint8_t sectionColumnIdx = sectionIdx % SECTIONS_PER_ROW;
				// sectionColumnIdx: 0,1,2,3
				// pictureY: 0,1,2,3,4,5,6,7
				uint8_t pictureX = (W_SECTION * sectionColumnIdx);
				for (uint8_t xOffset = 0; xOffset < W_SECTION; xOffset++) {
					uint8_t value = buffer[pictureY][pictureX + xOffset];
					applyColors(row.sections[sectionIdx], value, xOffset);
				}
			} else {
				// inverted
				// sectionRowIdx: 1
				uint8_t sectionColumnIdx = MAX_SECTION_IDX_IN_ROW - (sectionIdx % SECTIONS_PER_ROW);
				// sectionColumnIdx: 3,2,1,0
				// pictureY: 8,9,10,11,12,13,14,15
				uint8_t pictureX = (W_SECTION * sectionColumnIdx);
				for (uint8_t xOffset = 0; xOffset < W_SECTION; xOffset++) {
					uint8_t value = buffer[pictureY][pictureX + MAX_PIXEL_IDX_IN_SEGMENT - xOffset];
					applyColors(row.sections[sectionIdx], value, xOffset);
				}
			}

		}
		uint8_t rows = (1<<row.index);
		row.low = ~(rows & ROW_L_MASK);
		row.high = ~((rows >> 6) & ROW_H_MASK);

		SPI_reset();
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++)
		{
			Section section = row.sections[sectionIdx];
			sendByteBySPI(section.red.level0);
			sendByteBySPI(section.green.level0);
			sendByteBySPI(section.blue.level0);
		}

		PORTC = ROW_L_MASK;
		PORTB = ROW_H_MASK;
		confirmSPI();
		PORTC = row.low;
		PORTB = row.high;


	} else {
		SPI_reset();
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++) {
			Section section = row.sections[sectionIdx];

			if (row.step > 2) {
				sendByteBySPI(section.red.level3);
				sendByteBySPI(section.green.level3);
				sendByteBySPI(section.blue.level3);
			} else
				if (row.step > 0) {
					sendByteBySPI(section.red.level2);
					sendByteBySPI(section.green.level2);
					sendByteBySPI(section.blue.level2);
				} else
					if (row.step > 0) {
						sendByteBySPI(section.red.level1);
						sendByteBySPI(section.green.level1);
						sendByteBySPI(section.blue.level1);
					} else {
						sendByteBySPI(section.red.level0);
						sendByteBySPI(section.green.level0);
						sendByteBySPI(section.blue.level0);
					}

		}
		PORTC = ROW_L_MASK;
		PORTB = ROW_H_MASK;
		confirmSPI();
		PORTC = row.low;
		PORTB = row.high;
	}

	if (++row.step == 4) {
		row.reset();
		if (++row.index == ROWS) {
			row.index = 0;
		}
	}
}

//void applyColors(Colors& colors, const uint8_t value, const uint8_t offset) {
//	uint8_t red = (value & RED);
//	if (red && LEVELS[red] >= brightnessStep) {
//		colors.red |= (1<<offset);
//	}
//	uint8_t green = (value & GREEN) >> 2;
//	if (green && LEVELS[green] >= brightnessStep) {
//		colors.green |= (1<<offset);
//	}
//	uint8_t blue = (value & BLUE) >> 4;
//	if (blue && LEVELS[blue] >= brightnessStep) {
//		colors.blue |= (1<<offset);
//	}
//}
//
//
//
//// row timer, 20ms
//ISR(TIMER0_COMPA_vect) {
//	RowColors rowColors = RowColors();
//
//
//
//
//	resetSPI();
//	for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++) {
//		// 0,1,2,3,4,5,6,7...
//		uint8_t sectionRowIdx = sectionIdx / SECTIONS_PER_ROW;
//		// 0,0,0,0,1,1,1,1...
//		Colors colors = Colors();
//		uint8_t pictureY = (H_SECTION * sectionRowIdx) + currentRow;
//		if (sectionRowIdx % 2 == 0) {
//			// not-inverted
//			// sectionRowIdx: 0
//			uint8_t sectionColumnIdx = sectionIdx % SECTIONS_PER_ROW;
//			// sectionColumnIdx: 0,1,2,3
//			// pictureY: 0,1,2,3,4,5,6,7
//			uint8_t pictureX = (W_SECTION * sectionColumnIdx);
//			for (uint8_t xOffset = 0; xOffset < W_SECTION; xOffset++) {
//				uint8_t value = buffer[pictureY][pictureX + xOffset];
//				applyColors(colors, value, xOffset);
//			}
//		} else {
//			// inverted
//			// sectionRowIdx: 1
//			uint8_t sectionColumnIdx = MAX_SECTION_IDX_IN_ROW - (sectionIdx % SECTIONS_PER_ROW);
//			// sectionColumnIdx: 3,2,1,0
//			// pictureY: 8,9,10,11,12,13,14,15
//			uint8_t pictureX = (W_SECTION * sectionColumnIdx);
//			for (uint8_t xOffset = 0; xOffset < W_SECTION; xOffset++) {
//				uint8_t value = buffer[pictureY][pictureX + MAX_PIXEL_IDX_IN_SEGMENT - xOffset];
//				applyColors(colors, value, xOffset);
//			}
//		}
//		sendByteBySPI(colors.red);
//
//		sendByteBySPI(colors.green);
//
//		sendByteBySPI(colors.blue);
//
//	}
//	PORTC = ROW_L_MASK;
//	PORTB = ROW_H_MASK;
//	confirmSPI();
//	uint8_t rows = (1<<currentRow);
//	PORTC = ~(rows & ROW_L_MASK);
//	PORTB = ~((rows >> 6) & ROW_H_MASK);
//
//	if(++brightnessStep == 16) {
//		brightnessStep = 0;
//		if (++currentRow == ROWS) {
//			currentRow = 0;
//		}
//	}
//}

// event timer, 200ms
ISR(TIMER1_COMPA_vect) {
//	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
//		for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
//			uint8_t v = buffer[y][x];
//			v++;
//			buffer[y][x] = v == 0b01000000 ? 0 : v;
//		}
//	}
}

ISR(USART_TX_vect) {

}

ISR(USART_RX_vect) {
//	while (!(UCSR0A & (1<<RXC0)));
//	uint8_t temp = UDR0;
//	PORTC = temp;
}
