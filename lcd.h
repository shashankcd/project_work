#ifndef _lcd_h
#define _lcd_h


#include "avr/io.h"
#include "util/delay.h"


lcdinit()
{
	cmd(0x38);
	cmd(0x0C);
	cmd(0x01);
	cmd(0x80);
	
	_delay_ms(1000);
}



void cmd(unsigned char c)
{
	PORTC=c;
	PORTD=0x40;
	_delay_ms(1);
	PORTD=0x00;
}

void data(unsigned	char c)
{
	PORTC=c;
	_delay_ms(1);
	PORTD=0xC0;
	_delay_ms(1);
	PORTD=0x80;
	_delay_ms(1);
	cmd(0x06);
}

void string(char *p)
{
	while(*p) data(*p++);
	
}

#endif