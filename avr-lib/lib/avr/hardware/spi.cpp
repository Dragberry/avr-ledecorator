#include "spi.hpp"

void SPI::init()
{
	sbi(SPI_DDR, SPI_SS);
	sbi(SPI_DDR, SPI_MOSI);
	sbi(SPI_DDR, SPI_SCK);

	sbi(SPI_PORT, SPI_SS);

	sbi(SPCR, SPE);
	sbi(SPCR, MSTR);

	sbi(SPSR, SPI2X);
}

void SPI::send_byte(const uint8_t byte)
{
	outb(SPDR, byte);
	while (!(SPSR & BV(SPIF)));
}

void SPI::reset()
{
	cbi(SPI_PORT, SPI_SS);
}

void SPI::latch()
{
	sbi(SPI_PORT, SPI_SS);
}

void SPI::stop()
{
	cbi(SPI_DDR, SPI_SS);
	cbi(SPI_DDR, SPI_MOSI);
	cbi(SPI_DDR, SPI_SCK);

	cbi(SPI_PORT, SPI_SS);

	cbi(SPCR, SPE);
	cbi(SPCR, MSTR);

	cbi(SPSR, SPI2X);
}
