#ifndef _touchscreen_h
#define _touchscreen_h

#include "avr/io.h"
#include "util/delay.h"
#include "lcd.h"



uint8_t get_adc(unsigned char ch)
{
	ADMUX=0x60|ch;
	ADCSRA=0xC7;
	while(!ADIF);
	_delay_ms(10);
	return(ADCH);
}

unsigned char get_touch()
{
        unsigned char x_touch,y_touch;
		DDRA|= 0B01010000;
		DDRA&=~0b10100000;
		PORTA|=0B00010000;
		PORTA&=~0b01000000;
		_delay_ms(10);

		x_touch=get_adc(0x05);
		_delay_ms(100);

		DDRA|=0B10100000;
		DDRA&=~0b01010000;
		PORTA|= 0B10000000;
		PORTA&=~0b00100000;
		_delay_ms(10);
		y_touch=get_adc(0x04);
		_delay_ms(100);

		/*stransmit0("\r\nX:");
		cmd(0X80);
		string("X:");
		transmit0((x/100)+48);
		data((x/100)+48);
		x=x%100;
		transmit0((x/10)+48);
		data((x/10)+48);
		x=x%10;
		transmit0(x+48);
		data(x+48);
		stransmit0(", Y:");
		string(", Y:");
		transmit0((y/100)+48);
		data((y/100)+48);
		y=y%100;
		transmit0((y/10)+48);
		data((y/10)+48);
		y=y%10;
		transmit0(y+48);
		data(y+48);
		_delay_ms(10);*/
		if ((x_touch>=20) && (x_touch<70))
		{
			if ((y_touch>173)&&(y_touch<=225))
			{
				//lcd_char('*');
				 string( "1");
				return('1');
			}
			if ((y_touch>109)&&(y_touch<170))
			{
				//lcd_char('#');
				 string( "2");
				 return('2');
			}
			else if ((y_touch>=48)&&(y_touch<100))
			{
				//lcd_char(',');
				 string( "3");
				 return('3');
			}
			
		}
		
		else if ((x_touch>=70) && (x_touch<120))
		{
			if ((y_touch>=170)&&(y_touch<225))
			{
				//lcd_char('*');
				string( "4");
				return('4');
			}
			if ((y_touch>=110)&&(y_touch<170))
			{
				//lcd_char('#');
				string( "5");
				return('5');
			}
			else if ((y_touch>=40)&&(y_touch<=100))
			{
				//lcd_char(',');
				string( "6");
				return('6');
			}
			
		}
		
		else if ((x_touch>=130) && (x_touch<180))
		{
			if ((y_touch>170)&&(y_touch<=225))
			{
				//lcd_char('*');
				string( "7");
				return('7');
			}
			if ((y_touch>=110)&&(y_touch<170))
			{
				//lcd_char('#');
				string( "8");
				return('8');
			}
			if ((y_touch>=40)&&(y_touch<100))
			{
				//lcd_char('#');
				string( "9");
				return('9');
			}
			
		}
		
		else if ((x_touch>=180) && (x_touch<255))
		{
			if ((y_touch>170)&&(y_touch<=225))
			{
				//lcd_char('*');
				cmd(0x01);
				cmd(0x80);
				string("Enter Password :");
				cmd(0xC0);
			}
			if ((y_touch>=110)&&(y_touch<170))
			{
				//lcd_char('#');
				string( "0");
				return('0');
			}
			if ((y_touch>=40)&&(y_touch<100))
			{
				//lcd_char('#');
			}
		}
		return 0;
}

#endif