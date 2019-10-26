/*
 * Project 4
 *
 * Created: 5/20/2019 12:13:13 AM
 * Authors: 
 *	Mina Bedwany   69543570
 *	Jace Mica
 */ 

#include <stdio.h>
#include <avr/io.h>
#include "avr.c"
#include "lcd.c"

#define SAMPLING_PERIOD 500

unsigned short current_reading;
unsigned short max_reading;
unsigned short min_reading;
unsigned long counter;
unsigned long total_sum;
int sample;
int reset;

int is_pressed(int r, int c);
int get_key();

unsigned short get_sample();
void output_LCD();
void output_reset();

int get_key()
{
	int r, c;
	for (r = 0; r < 4; ++r)
		for (c = 0; c <4; ++c)
		{
		if(is_pressed(r,c))
			return (r*4+c) +1;
	}
	return 0;
}

int is_pressed(int r, int c)
{
	DDRC = 0;
	PORTC = 0;

	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	
	CLR_BIT(DDRC, c+4);
	SET_BIT(PORTC, c+4);
	
	avr_wait(1);
	
	if(GET_BIT(PINC, c+4))
	{
		return 0;
	}
	
	return 1;
}

unsigned short get_sample()
{
	CLR_BIT(ADMUX,0);
	CLR_BIT(ADMUX,1);
	CLR_BIT(ADMUX,2);
	CLR_BIT(ADMUX,3);
	CLR_BIT(ADMUX,4);
	
	SET_BIT(ADCSRA,7);
	SET_BIT(ADCSRA,6);
	
	while(GET_BIT(ADCSRA,6)){}
	return ADC;
}

void output_reset()
{
	char * top = "Vt:---- Av:----";
	char * bottom = "Mn:---- Mx:----";

	lcd_clr();
	lcd_pos(0,0);
	lcd_puts2(top);
	lcd_pos(1,0);
	lcd_puts2(bottom);
}

void output_LCD()
{

	/*float voltage = ((float)current_reading/1023) * 5.0;
	float avg = (float)total_sum / (float)counter;
	float average_voltage = (avg/1023) * 5.0;
	float minimum = ((float)min_reading/1023) * 5.0;
	float maximum= ((float)max_reading/1023) * 5.0;
	
	char top[16];
	sprintf(top, "Vt:%.2f Av:%.2f", voltage, average_voltage);
	
	char bottom[16];
	sprintf(bottom, "Mn:%.2f Mx:%.2f", minimum, maximum);*/
	
	unsigned int convert_volt = ((unsigned long)(current_reading) * 5 * 100) / 1023;
	unsigned int digit_volt = convert_volt / 100;
	unsigned int decimal_volt = convert_volt % 100;
	
	unsigned long average = total_sum / counter;
	unsigned int convert_avg = (average * 5 * 100) / 1023;
	unsigned int digit_avg = convert_avg / 100;
	unsigned int decimal_avg = convert_avg % 100;
	
	unsigned int convert_min = ((unsigned long)(min_reading) * 5 * 100) / 1023;
	unsigned int digit_min = convert_min / 100;
	unsigned int decimal_min = convert_min % 100;
	
	unsigned int convert_max = ((unsigned long)(max_reading) * 5 * 100) / 1023;
	unsigned int digit_max = convert_max / 100;
	unsigned int decimal_max = convert_max % 100;
	
	char top[16];
	sprintf(top, "Vt:%d.%02d Av:%d.%02d", digit_volt, decimal_volt, digit_avg, decimal_avg);
	
	char bottom[16];
	sprintf(bottom, "Mn:%d.%02d Mx:%d.%02d", digit_min, decimal_min, digit_max, decimal_max);
	
	lcd_clr();
	lcd_pos(0,0);
	lcd_puts2(top);
	lcd_pos(1,0);
	lcd_puts2(bottom);
}


int main(void)
{
	avr_init();
	lcd_init();
	
	sample = 0;
	reset = 0;
	current_reading = 0;
	max_reading = 0;
	min_reading = 1023;
	counter = 0;
	total_sum = 0;
	
	const char* start_message = "1:Start Sampling";
	lcd_pos(0,0);
	lcd_puts2(start_message);
	
	const char* start_message1 = "0:Reset";
	lcd_pos(1,0);
	lcd_puts2(start_message1);
	
	CLR_BIT(ADMUX,7);
	SET_BIT(ADMUX,6);
	
	while (1) 
    {
		/* KEY LEGEND:
			0 button (key 14) = Reset
			1 button (key 1) = Start sampling
		*/
			int key = get_key();
			switch(key) 
			{
				case 1: 
					sample = 1;
					reset = 0;
					break;
				case 14:
					reset = 1;
					sample = 0;
					break;
				default:
					break;
			}
			
			if(sample)
			{
				avr_wait(SAMPLING_PERIOD);
				unsigned short mySample = get_sample();
				counter++;
				total_sum += mySample;
				
				current_reading = mySample;
				if(mySample < min_reading)
					min_reading = mySample;
				if(mySample > max_reading)
					max_reading = mySample;
				if(counter >= 4294967295)
					counter = 0;	
				if(total_sum >= 4294967295)
					total_sum = 0;
				
				output_LCD();
			}
			
			if(reset)
			{
				current_reading = 0;
				max_reading = 0;
				min_reading = 1023;
				counter = 0;
				total_sum = 0;
				
				output_reset();
			}
	}
}