#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#include "apps/application.h"
#include "apps/games/snake/snakegame.h"
#include "apps/games/life/lifegame.h"
#include "screen/colors.h"
#include "screen/definitions.h"
#include "screen/row.h"
#include "screen/section.h"
#include "screen/processors/screen.h"
#include "screen/processors/processor.h"
#include "screen/processors/fillscreenprocessor.h"
#include "screen/processors/defaultscreenprocessor.h"
#include "screen/processors/uartprocessor.h"

#define FCPU 20000000UL
#define USART_BAUDRATE  9600UL
#define UBRR ((FCPU / (USART_BAUDRATE * 16UL)) - 1)

Screen* screen;

//Application* app;

void setup();

void loop();

void init_USART(uint16_t ubrr)
{
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

void init_app()
{

//	app = new SnakeGame(SCREEN_HEIGHT, SCREEN_WIDTH, BLUE, YELLOW, RED);
//	app = new LifeGame(YELLOW, BLUE);
}

void init_app_timer()
{
//	// CTC
//	TCCR1A |= (0<<WGM10);
//	TCCR1A |= (0<<WGM11);
//	TCCR1B |= (1<<WGM12);
//	TCCR1B |= (0<<WGM13);
//	// 000 - f
//	// 100 - f/256
//	// 101 - f/1024
//	TCCR1B |= (1<<CS12);
//	TCCR1B |= (0<<CS11);
//	TCCR1B |= (0<<CS10);
//	TIMSK1 |= (1<<OCIE1A);
//	// 0.2s
//	OCR1A = 0x3D09;
}

void setup()
{
	screen = new Screen();
	init_USART(UBRR);
	sei();
}

void stop_app_timer() {
	TCCR1B = 0;
	TIMSK1 &= ~(1 << OCIE1A);
	OCR1A = 0;
}

void stop_app() {
//	stop_app_timer();
//	delete app;
//	app = NULL;
}

void loop()
{
//	init_app();
//	init_app_timer();
//	while (app->is_running());
//	stop_app_timer();
//	stop_app();
//	clear_screen();
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

Processor* processors[1] =
{
//	new UartProcessor(),
	new DefaultScreenProcessor()
//    new FillScreenProcessor()
};

Processor* processor = processors[0];

int main()
{
	setup();
	while(1)
	{
		processor->process(screen);
//		loop();
	}
	return 0;
}

void draw()
{
//	app->build_image(buffer);
}


uint8_t temp = 1;

// row timer, 20ms
ISR(TIMER0_COMPA_vect)
{
	screen->draw_row();
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

// app timer, 200ms
ISR(TIMER1_COMPA_vect) {
//	++(*app);
//	draw();
}

ISR(USART_TX_vect) {

}

ISR(USART_RX_vect) {
//	while (!(UCSR0A & (1<<RXC0)));
//	uint8_t temp = UDR0;
//	PORTC = temp;
}
