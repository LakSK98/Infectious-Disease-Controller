#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "LCD1.h"
#include "LCD2.h"
#include "LCD3.h"
#include "USART.h"
#include "MLX90614.h"

#define DEVICE_ADD 0x5A
#define RAM_ADD 0x07

#define BUZZER PD1 //define buzzer as PORTD 0th Pin
#define MOTOR PD7 //define buzzer as PORTD 7th Pin
#define STATUS(a,b) ((a & (1<<b))>>b) //Check Status of b

void RFID();
void MLX90614();
void PIR();
void ULTRASONIC();

static volatile int pulse = 0;
static volatile int i = 0;
volatile int Overflow=0,temp;

int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xF1;
	DDRC |= 0xFC;
	DDRD |= 0xB1;
	GICR |= (1<<INT0) | (1<<INT1) | (1<<INT2);
	MCUCR |= (1<<ISC00) | (1<<ISC10) | (1<<ISC11);
	MCUCSR |= (1<<ISC2);
	TCCR0 = (1<<CS00);
	TIMSK =(1<<TOIE0);
	sei();
	LCD1_Init();
	LCD2_Init();
	LCD3_Init();
	USART_Init(9600);
	MLX90614_Init();
	
	while(1)
	{
	RFID();
	while(!STATUS(PINA,PINA2));
	TCNT0 = 0x00;
	Overflow=0;
	PORTD |= 0x20;
	while(!STATUS(PINA,PINA3));
	temp=Overflow+TCNT0;
	_delay_ms(1000);
	PORTD ^= 0x20;
	TCNT0 = 0x00;
	Overflow=0;
	PORTD |= 0x08;
	while(!(temp==Overflow));
	_delay_ms(1000);
	PORTD ^= 0x08;
	MLX90614();
	PIR();
	ULTRASONIC();
	}
}

void RFID()
{
	char RFID[15];
	LCD1_Clear();
	memset(RFID,0,15);
	LCD1_String_xy(0,0,"RFID:"); /* Set row and column position at 0,0 and Print RFID*/
	
		for (int i=0;i<12;i++)
		{
			RFID[i]=USART_RxChar();
		}
		_delay_us(10);
		LCD1_String_xy(0,1,RFID);/* print 12 digit tag number on LCD */
	
}

void MLX90614()
{
	char Temp[7];
        double tempFactor,Temp_value,kelvin,celcius;    
        tempFactor=0.02;
        LCD2_Init();
        MLX90614_Init();
        _delay_ms(3000);
        LCD2_Clear();
        Temp_value=MLX90614_Read(DEVICE_ADD,RAM_ADD);
        kelvin = (Temp_value*tempFactor);
        celcius = kelvin - 273.15;
        dtostrf(celcius,2,2,Temp);
    
        if(celcius<=37){
	    LCD2_String_xy(0,0,"No Fever");
	    LCD2_String_xy(0,1,"You can access");
        }
        else{
	    LCD2_String_xy(0,0,"Dangerous");
	    LCD2_String_xy(0,1,"Temp: = ");
	    LCD2_String(Temp);
	    LCD2_Char(223);
	    LCD2_Char('C');
		PORTD |= (PORTD<<BUZZER);
		_delay_ms(10000);
		PORTD ^= (PORTD<<BUZZER);
        }
}
	
	void PIR()
	{	
			while(!STATUS(PIND,PIND6));//wait for person
			PORTD |= (1<<MOTOR);//Set MOTOR Pin as High
			_delay_ms(10000);//wait for 10 seconds
			PORTD ^= (1<<MOTOR);//Turn off MOTOR by toggle it as low
	}
	

	void ULTRASONIC()
	{
		int16_t count_a = 0;
		char show_a[16];
		_delay_ms(50);
		LCD3_Clear();
			PORTB |= 1<<PINB0;
			_delay_us(15);

			PORTB &= ~(1<<PINB0);
			count_a = (60-(pulse/58)/60);
			
			if(count_a<=15){
				LCD3_String_xy(0,0,"Refill Sanitizer");
				LCD3_String_xy(0,1,"Percentage = ");
				itoa(count_a,show_a,10);
				LCD3_String(show_a);
				LCD3_String(" %");
			}
			else if(count_a>=95){
				LCD3_String_xy(0,0,"Stop Refill Tank");
				LCD3_String_xy(0,1,"Percentage = ");
				itoa(count_a,show_a,10);
				LCD3_String(show_a);
				LCD3_String(" %");
			}
			else{
				LCD3_String_xy(0,0,"Sanitizer Level");
				LCD3_String_xy(0,1,"Percentage = ");
				itoa(count_a,show_a,10);
				LCD3_String(show_a);
				LCD3_String(" %");
			}
	}

	ISR(INT0_vect)
	{
		if(i == 0)
		{
			TCCR1B |= 1<<CS10;
			i = 1;
		}
		else
		{
			TCCR1B = 0;
			pulse = TCNT1;
			TCNT1 = 0;
			i = 0;
		}
	}
	ISR(INT1_vect)
	{
		PORTD |= (1<<BUZZER);
		_delay_ms(10000);
		PORTD ^= (1<<BUZZER);
	}
	ISR(INT2_vect)
	{
		PORTD |= (1<<BUZZER);
		_delay_ms(5000);
		PORTD ^= (1<<BUZZER);
		RFID();	
	}
