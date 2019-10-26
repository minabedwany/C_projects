/*
 * Project 2.c
 *
 * Created: 4/21/2019 12:13:13 AM
 * Authors: 
 *	Mina Bedwany   69543570
 *	Jace Mica
 */ 

#include <stdio.h>
#include <avr/io.h>
#include "avr.c"
#include "lcd.c"

int seconds = 0;
int minutes = 0;
int hours = 0;
int days = 1;
int months = 1;
int years = 2010;

int is_pressed(int r, int c);
int get_key();
void incrementTime();
int isLeap();
void outputLCD();
void fixTime();

int militaryON = 1;
int blinkdays = 0;
int blinkmonths = 0;
int blinkyears = 0;
int blinkhours = 0;
int blinkminutes = 0;
int blinkseconds = 0;


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

void incrementTime(){
	if(seconds < 60)
	{
		seconds++;
		if(seconds >= 60)
		{
			seconds = 0;
			minutes++;
		}
	}

	if(minutes >= 60)
	{
		minutes = 0;
		hours++;
	}

	if(hours >= 24)
	{
		hours = 0;
		days++;
	}

	if(days > 28)
	{
		if(days > 28 && months == 2 && !(isLeap()))
		{
			days = 1;
			months++;
		}
		else if(days > 29 && months == 2 && isLeap())
		{
			days = 1;
			months++;
		}
		else if(days > 30 && (months == 4 || months == 6 || months == 9 || months == 11))
		{
			days = 1;
			months++;
		}
		else if(days > 31)
		{
			days = 1;
			months++;
		}
	}

	if(months > 12)
	{
		months = 1;
		years++;
	}
}

int isLeap()
{
	return (years % 4 == 0) && (years % 100 != 0 || years % 400 == 0);
}

void outputLCD()
{
	blinkdays = 0;
	blinkmonths = 0;
	blinkyears = 0;
	blinkhours = 0;
	blinkminutes = 0;
	blinkseconds = 0;
	char date[11];
	char time[12];
	
	sprintf(date, "%02d/%02d/%02d", days, months, years);
	lcd_pos(0, 0);
	lcd_puts2(date);
	
	if(militaryON)
	{	
		sprintf(time, "%02d:%02d:%02d", hours, minutes, seconds);
	}
	else
	{
		char * ampm;
		int regularhours;
		if(hours < 12) {
			ampm = "AM";
			regularhours = hours;
		}
		else
		{
			ampm = "PM";
			if(hours == 12)
				regularhours = 12;
			else
				regularhours = hours - 12;
		}
		sprintf(time, "%02d:%02d:%02d %s", regularhours, minutes, seconds, ampm);	
	}
	
	lcd_pos(1, 0);
	lcd_puts2(time);
}

void outputLCD2()
{
	char date[11];
	char time[12];
	
	sprintf(date, "%02d/%02d/%02d", days, months, years);
	lcd_pos(0, 0);
	lcd_puts2(date);
	avr_wait(250);
	if(blinkdays)
		sprintf(date, "  /%02d/%02d", months, years);
	if(blinkmonths)
		sprintf(date, "%02d/  /%02d", days, years);
	if(blinkyears)
		sprintf(date, "%02d/%02d/    ", days, months);
	lcd_pos(0, 0);
	lcd_puts2(date);
	avr_wait(250);
	
	
	if(militaryON)
	{
		sprintf(time, "%02d:%02d:%02d   ", hours, minutes, seconds);
		lcd_pos(1, 0);
		lcd_puts2(time);
		avr_wait(250);
		if(blinkhours)
			sprintf(time, "  :%02d:%02d   ", minutes, seconds);
		if(blinkminutes)
			sprintf(time, "%02d:  :%02d   ", hours, seconds);
		if(blinkseconds)
			sprintf(time, "%02d:%02d:     ", hours, minutes);
	}
	else
	{
		char * ampm;
		int regularhours;
		if(hours < 12) {
			ampm = "AM";
			regularhours = hours;
		}
		else
		{
			ampm = "PM";
			if(hours == 12)
			regularhours = 12;
			else
			regularhours = hours - 12;
		}
		sprintf(time, "%02d:%02d:%02d %s", regularhours, minutes, seconds, ampm);
		lcd_pos(1, 0);
		lcd_puts2(time);
		avr_wait(250);
		if(blinkhours)
			sprintf(time, "  :%02d:%02d %s", minutes, seconds, ampm);
		if(blinkminutes)
			sprintf(time, "%02d:  :%02d %s", regularhours, seconds, ampm);
		if(blinkseconds)
			sprintf(time, "%02d:%02d:   %s", regularhours, minutes, ampm);
	}
	lcd_pos(1, 0);
	lcd_puts2(time);
	avr_wait(250);
	
}

void fixTime(){
	if(years < 0){
		years = 2010;
	}

	if(months > 12){
		months = 1;
	}

	if(hours > 23){
		hours = 0;
	}

	if(minutes > 59){
		minutes = 0;
	}

	if(seconds > 59){
		seconds = 0;
	}

	if(days > 28){
		if(days > 28 && months == 2 && !(isLeap()))
			days = 1;
			
		else if(days > 29 && months == 2 && isLeap())
			days = 1;
			
		else if(days > 30 && (months == 4 || months == 6 || months == 9 || months == 11))
			days = 1;
			
		else if(days > 31)
			days = 1;
			
	}
}

int main(void)
{
	avr_init();
	lcd_init();
	int timerON = 0;
	outputLCD();
	
	while (1) 
    {
		/* KEY LEGEND:
			* button (key 13) = Start Clock Timer
			# button (key 15) = Stop Clock Timer
			A button (key 4) = Use Military time standard
			B button (key 8) = Use Regular time standard
			1 button (key 1) = Increment days
			2 button (key 2) = increment months
			3 button (key 3) = increment years
			4 button (key 5) = increment hours
			5 button (key 6) = increment minutes
			6 button (key 7) = increment seconds
			0 button (key 14) = Reset clock
			
		*/
			int key = get_key();
			avr_wait(150);
			switch(key) 
			{
				case 1: 
					if(!(timerON)) {
						blinkdays = 1;
						blinkmonths = 0;
						blinkyears = 0;
						blinkhours = 0;
						blinkminutes = 0;
						blinkseconds = 0;
						days++;
					}
					break;
				case 2:
					if(!(timerON)) {
						blinkdays = 0;
						blinkmonths = 1;
						blinkyears = 0;
						blinkhours = 0;
						blinkminutes = 0;
						blinkseconds = 0;
						months++;
					}
					break;
				case 3:
					if(!(timerON)) {
						blinkdays = 0;
						blinkmonths = 0;
						blinkyears = 1;
						blinkhours = 0;
						blinkminutes = 0;
						blinkseconds = 0;
						years++;
					}
					break;
				case 4:
					militaryON = 1;
					break;
				case 5:
					if(!(timerON)) {
						blinkdays = 0;
						blinkmonths = 0;
						blinkyears = 0;
						blinkhours = 1;
						blinkminutes = 0;
						blinkseconds = 0;
						hours++;
					}
					break;
				case 6:
					if(!(timerON)) {
						blinkdays = 0;
						blinkmonths = 0;
						blinkyears = 0;
						blinkhours = 0;
						blinkminutes = 1;
						blinkseconds = 0;
						minutes++;
					}
					break;
				case 7:
					if(!(timerON)) {
						blinkdays = 0;
						blinkmonths = 0;
						blinkyears = 0;
						blinkhours = 0;
						blinkminutes = 0;
						blinkseconds = 1;
						seconds++;
					}
					break;
				case 8:
					militaryON = 0;
					break;
				case 13:
					timerON = 1;
					break;
				case 14:
					if(!(timerON)) {
						days = 0;
						months = 0;
						years = 2010;
						hours = 0;
						minutes = 0;
						seconds = 0;
					}
					break;
				case 15:
					timerON = 0;
					break;
				default:
					break;
			}
			fixTime();
			
			outputLCD2();
			//lcd_clr();

			//avr_wait(200);
			if(timerON) 
			{
				outputLCD();
				//avr_wait(500);
				incrementTime();
			}	
    }
}

