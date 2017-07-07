//PORTD ---> PD0,PD1 --> RX,TX FOR FINGER PRINT 
//PORTD ---> PD2,PD3 --> RX,TX FOR FACE UART
//PORTD ---> PD7,PD6,PD5 ---> RS,RW,E OF LCD
//PORTC ---> LCD DATA
//PORTA ---> PA7,PA6,PA5,PA4 ---> TOUCH SCREEN DATA
//PORTB ---> MOTOR DRIVER


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include "fingerprint.h"
#include "touchscreen.h"
#include "lcd.h"




unsigned char ack[12]={0};


volatile unsigned int i=0,it=0,one_time=0,pk=0,dk=0;


void uartinit_1(void)
{
	UBRR1L=0x33;
	UCSR1B |= (1 << RXEN1 ) | (1 << TXEN1 )|(1<<RXCIE1);

}


uart_trans1(char tx_data1)
{
	while (( UCSR1A & (1 << UDRE1 )) == 0);
	UDR1 = tx_data1 ;
	
}

void stransmit1(char *p)
{
	while(*p) uart_trans1(*p++);
}

ISR(USART1_RX_vect)
{
	dk=UDR1;
	cmd(0x01);
	cmd(0x80);
	string("Received Data");
	cmd(0xC0);
	data(dk);
	_delay_ms(1000);
}

ISR(USART0_RX_vect)
{
	ack[it]=UDR0;
	it++;
	
	if(it>11)
	{
			pk=1;
			it=0;
	}	
}


void main()
{
	DDRC=0xff;
	DDRD=0xf0;
	DDRA=0x00;
	DDRB=0xff;
	unsigned int flag=0,flag0=0,flag1=0,id,t,t1,t2;
	unsigned char p[4],q,r,s[4]="1324",s1[4]="4537",s2[4]="9063";
	uartinit();
	uartinit_1();
	sei();
	lcdinit();
	cmd(0x80);
	string("Enter Password : ");
	cmd(0xC0);
	PORTB=0x00;
	while(1)
	{
		if(flag==0)
		{
			for(i=0;i<4;i++)
			{
				p[i]=0;
			}
			i=0;
		  while(i<4)
		  {
			r=get_touch();
			if(r!=0)
			{ 
			p[i]=r;
			i++;
			_delay_ms(1000);
			}
			
		  }
		  cmd(0x01);
		  cmd(0x80);
		  string("Enter Password : ");
		  cmd(0xC0);
		  string("Checking....");
		  _delay_ms(2000);
		  cmd(0x01);
		  if(!strncmp(s,p,4))
		  {
				flag=1;
				flag0=1;
				cmd(0x01);
				cmd(0x80);
				string("Welcome ");
				cmd(0xC0);
				string("Omkar");
				_delay_ms(3000);
				id=1;
		  }
		  else if(!strncmp(s1,p,4))
		  {
				flag=1;
				flag0=1;
				cmd(0x01);
				cmd(0x80);
				string("Welcome ");
				cmd(0xC0);
				string("Shravan");
				_delay_ms(3000);
				id=3;
		  }
		  else if(!strncmp(s2,p,4))
		  {
				flag=1;
				flag0=1;
				cmd(0x01);
				cmd(0x80);
				string("Welcome ");
				cmd(0xC0);
				string("Vishal");
				_delay_ms(3000);
				id=2;
		  }
		  else
		  {
			cmd(0x01);
			cmd(0x80);
			string("Enter Password : ");
			cmd(0xC0);
			string("Wrong Password");
		  }
		}
		else if(flag0==1)
		{
			cmd(0x80);
			string("Password Match");
			cmd(0xC0);
			string("Put Finger >");
			_delay_ms(2000);
			while(one_time==0)
			{
				genimg();
				_delay_ms(500);
				if(pk==1)
				{
					int match=1;
					for(int j=0; j<12; j++)
					{
						if(ack[j]!=genimg_ack[j])	match=0;
					}
					if(match==1)
					{		
						cmd(0x01);
						cmd(0x80);
						string("Finger Collected");
						one_time=1;
						
					}
					else
					{
						cmd(0x01);
						cmd(0x80);
						string("No Finger");	
					}
				}
			}
			genchar1();
			_delay_ms(500);
			
			gentemp();
			_delay_ms(500);
			
			cmd(0x01);
			cmd(0x80);
			string("searching");
			searchfing();
			
			cmd(0x01);
			cmd(0x80);
			t = ack[11] % 100;
			t1=(t/10)+48;
			t2=(t % 10)+48;
			data(t1);
			data(t2);
			_delay_ms(500);
			
				
				if(t2=='1')
				{
					cmd(0x01);
					cmd(0x80);
					string("Finger matched");
					cmd(0xC0);
					string("with onkar");
					flag1=1;
					flag0=0;
					_delay_ms(3000);
				}
				else if(t2=='2')
				{
					cmd(0x01);
					cmd(0x80);
					string("Finger matched");
					
					cmd(0xC0);
					string("with vishal");
					flag1=1;
					flag0=0;
					_delay_ms(3000);
				}
				else if(t2=='3')
				{
					cmd(0x01);
					cmd(0x80);
					string("Finger matched");
					
					cmd(0xC0);
					string("with shravan");
					flag1=1;
					flag0=0;
					_delay_ms(3000);
				}
				else
				{
				cmd(0x01);
				cmd(0x80);
				string("Finger Not");
				cmd(0xC0);
				string("Matched");
				flag0=0;
				_delay_ms(2000);
				}
		}
		else if(flag1==1)
		{
			cmd(0x01);
			cmd(0x80);
			string("Waiting for");
			cmd(0xC0);
			string("face ID...");
			_delay_ms(10000);
			while(dk==0);
			if(dk=='1')
			{
				cmd(0x01);
				cmd(0x80);
				string("Face Match");
				cmd(0xC0);
				string("with Onkar");
				_delay_ms(2000);
				cmd(0x01);
				cmd(0x80);
				string("Access");
				cmd(0xC0);
				string("Granted");
				_delay_ms(2000);
				PORTB=0xA0;
				flag1=0;
				flag=0;
			}
			else if(dk=='2')
			{
				cmd(0x01);
				cmd(0x80);
				string("Face Match");
				cmd(0xC0);
				string("with vishal");
				_delay_ms(2000);
				cmd(0x01);
				cmd(0x80);
				string("Access");
				cmd(0xC0);
				string("Granted");
				_delay_ms(2000);
				PORTB=0xA0;
				flag1=0;
				flag=0;
			}
			else if(dk=='3')
			{
				cmd(0x01);
				cmd(0x80);
				string("Face Match");
				cmd(0xC0);
				string("with shravan");
				_delay_ms(2000);
				cmd(0x01);
				cmd(0x80);
				string("Access");
				cmd(0xC0);
				string("Granted");
				_delay_ms(2000);
				PORTB=0xA0;
				flag1=0;
				flag=0;
			}
			else if(dk=='0')
			{
				cmd(0x01);
				cmd(0x80);
				string("Face");
				cmd(0xC0);
				string("Not matched"); ////  Face not matched
				_delay_ms(3000);
				PORTB=0x00;
				
			}
			
			
		
		}			
				
	} 
		
		
}
	
