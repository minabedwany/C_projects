/*
 * Project 2.c
 *
 * Created: 5/9/2019 12:13:13 AM
 * Authors: 
 *	Mina Bedwany   69543570
 *	Jace Mica
 */ 

#include <stdio.h>
#include <avr/io.h>
#include "avr.c"
#include "lcd.c"

#define nill 0
#define C2 65
#define D2 73
#define E2 82
#define F2 87
#define G2 98
#define GS2 104
#define A2 110
#define B2 123
#define C3 131
#define D3 147
#define DS3 156 
#define E3 165
#define F3 175
#define G3 196
#define A3 220
#define B3 247

#define BDAY_SONG 1
#define BDAY_SIZE 25
#define FUR_ELISE 2
#define FUR_ELISE_SIZE 37

struct note {
	unsigned int freq;
	unsigned int dur;
};

struct note birthday_song[BDAY_SIZE];
struct note fur_elise[FUR_ELISE_SIZE];


void play_song(struct note * song, int index);
void play_note(unsigned int freq, unsigned int duration);
void wait1(unsigned int freq);
void init_songs();

int is_pressed(int r, int c);
int get_key();

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

void play_song(struct note * song, int index)
{
	int i, n;
	char * m = "Now Playing:";
	char * m2;
	if(index == BDAY_SONG) {
		n = BDAY_SIZE; 
		m2 = "Happy Birthday!";
	}
	if(index == FUR_ELISE){
		n = FUR_ELISE_SIZE;
		m2 = "Fur Elise";
	}
	lcd_clr();
	lcd_pos(0,0);
	lcd_puts2(m);
	lcd_pos(1,0);
	lcd_puts2(m2);
	double speed = 1;
	double pitch = 0.25;
	int key;
	for(i = 0; i < n; i++)
	{
		key = get_key();
		if(key == 4)
		{
			speed = 0.5;
		}
		if(key == 8)
		{
			speed = 2;
		}
		if(key == 1)
		{
			speed = 1;
			pitch = 0.25;
		}
		if(key == 12)
		{
			pitch = 0.5;
		}
		if(key == 16)
		{
			pitch = 0.125;
		}

		play_note(song[i].freq * pitch, song[i].dur * speed);
	}
}

void play_note(unsigned int freq, unsigned int duration)
{
	int i, n = duration * freq;
	for (i = 0; i < n; i++)
	{
		SET_BIT(PORTB, 4);
		wait1(freq);
		CLR_BIT(PORTB, 4);
		wait1(freq);
	}
}

void wait1(unsigned int freq)
{
	//fix this?
	TCCR0 = 2;
	unsigned int period = 10000 / freq;	
	unsigned int half_period = period / 2;
	while (half_period--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 8) * 0.000005);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

void init_songs() {
	
	birthday_song[0].freq = G2;
	birthday_song[0].dur = 2;
	
	birthday_song[1].freq = G2;
	birthday_song[1].dur = 2;
	
	birthday_song[2].freq = A2;
	birthday_song[2].dur = 4;
	
	birthday_song[3].freq = G2;
	birthday_song[3].dur = 4;
	
	birthday_song[4].freq = C3;
	birthday_song[4].dur = 4;
	
	birthday_song[5].freq = B2;
	birthday_song[5].dur = 8;
	
	birthday_song[6].freq = G2;
	birthday_song[6].dur = 2;
	
	birthday_song[7].freq = G2;
	birthday_song[7].dur = 2;
	
	birthday_song[8].freq = A2;
	birthday_song[8].dur = 4;
	
	birthday_song[9].freq = G2;
	birthday_song[9].dur = 4;
	
	birthday_song[10].freq = D3;
	birthday_song[10].dur = 4;
	
	birthday_song[11].freq = C3;
	birthday_song[11].dur = 8;
	
	birthday_song[12].freq = G2;
	birthday_song[12].dur = 2;
	
	birthday_song[13].freq = G2;
	birthday_song[13].dur = 2;
	
	birthday_song[14].freq = G3;
	birthday_song[14].dur = 4;
	
	birthday_song[15].freq = E3;
	birthday_song[15].dur = 4;
	
	birthday_song[16].freq = C3;
	birthday_song[16].dur = 4;
	
	birthday_song[17].freq = B2;
	birthday_song[17].dur = 4;
	
	birthday_song[18].freq = A2;
	birthday_song[18].dur = 4;
	
	birthday_song[19].freq = F3;
	birthday_song[19].dur = 2;
	
	birthday_song[20].freq = F3;
	birthday_song[20].dur = 2;
	
	birthday_song[21].freq = E3;
	birthday_song[21].dur = 4;
	
	birthday_song[22].freq = C3;
	birthday_song[22].dur = 4;
	
	birthday_song[23].freq = D3;
	birthday_song[23].dur = 4;
	
	birthday_song[24].freq = C3;
	birthday_song[24].dur = 8;

	//End birthday song
	
	fur_elise[0].freq = E3;
	fur_elise[0].dur = 4;
	
	fur_elise[1].freq = DS3;
	fur_elise[1].dur = 4;
	
	fur_elise[2].freq = E3;
	fur_elise[2].dur = 4;
	
	fur_elise[3].freq = DS3;
	fur_elise[3].dur = 4;
	
	fur_elise[4].freq = E3;
	fur_elise[4].dur = 4;
	
	fur_elise[5].freq = B2;
	fur_elise[5].dur = 4;
	
	fur_elise[6].freq = D3;
	fur_elise[6].dur = 4;
	
	fur_elise[7].freq = C3;
	fur_elise[7].dur = 4;
	
	fur_elise[8].freq = A2;
	fur_elise[8].dur = 4;
	
	fur_elise[9].freq = nill;
	fur_elise[9].dur = 4;
	
	fur_elise[9].freq = C2;
	fur_elise[9].dur = 4;
	
	fur_elise[10].freq = E2;
	fur_elise[10].dur = 4;
	
	fur_elise[11].freq = A2;
	fur_elise[11].dur = 4;
	
	fur_elise[12].freq = B2;
	fur_elise[12].dur = 8;
	
	fur_elise[13].freq = nill;
	fur_elise[13].dur = 4;
	
	fur_elise[14].freq = E2;
	fur_elise[14].dur = 4;
	
	fur_elise[15].freq = GS2;
	fur_elise[15].dur = 4;
	
	fur_elise[16].freq = B2;
	fur_elise[16].dur = 4;
	
	fur_elise[17].freq = C3;
	fur_elise[17].dur = 8;
	
	fur_elise[18].freq = E3;
	fur_elise[18].dur = 4;
	
	fur_elise[19].freq = DS3;
	fur_elise[19].dur = 4;
	
	fur_elise[20].freq = E3;
	fur_elise[20].dur = 4;
	
	fur_elise[21].freq = DS3;
	fur_elise[21].dur = 4;
	
	fur_elise[22].freq = E3;
	fur_elise[22].dur = 4;
	
	fur_elise[23].freq = B2;
	fur_elise[23].dur = 4;
	
	fur_elise[24].freq = D3;
	fur_elise[24].dur = 4;
	
	fur_elise[25].freq = C3;
	fur_elise[25].dur = 4;
	
	fur_elise[26].freq = A2;
	fur_elise[26].dur = 8;
	
	fur_elise[27].freq = nill;
	fur_elise[27].dur = 4;
	
	fur_elise[28].freq = C2;
	fur_elise[28].dur = 4;
	
	fur_elise[29].freq = E2;
	fur_elise[29].dur = 4;
	
	fur_elise[30].freq = A2;
	fur_elise[30].dur = 4;
	
	fur_elise[31].freq = B2;
	fur_elise[31].dur = 8;
	
	fur_elise[32].freq = nill;
	fur_elise[32].dur = 4;
	
	fur_elise[33].freq = E2;
	fur_elise[33].dur = 4;
	
	fur_elise[34].freq = C3;
	fur_elise[34].dur = 4;
	
	fur_elise[35].freq = B2;
	fur_elise[35].dur = 4;
	
	fur_elise[36].freq = A2;
	fur_elise[36].dur = 12;
}


int main(void)
{
	avr_init();
	lcd_init();
	init_songs();
	
	SET_BIT(DDRB, 4);

	lcd_clr();
	const char* start_message = "1:Happy Birthday";
	lcd_pos(0,0);
	lcd_puts2(start_message);
	
	const char* start_message1 = "2:Fur Elise";
	lcd_pos(1,0);
	lcd_puts2(start_message1);
	
	while (1) 
    {
		/* KEY LEGEND:
			1 button (key 1) = play birthday song
			2 button (key 2) = play fur elise
			A button (key 4) = Higher tempo (faster speed)
			B button (key 8) = Slower tempo (slower speed)
			C button (key 12) = Higher pitch (higher frequency)
			D button (key 16) = Lower pitch (lower frequency)
		*/
			int key = get_key();
			avr_wait(100);
			switch(key) 
			{
				case 1: 
					play_song(birthday_song, BDAY_SONG);
					break;
				case 2:
					play_song(fur_elise, FUR_ELISE);
					break;
				case 3:
					
					break;
				case 4:
					break;
				case 5:
					
					break;
				case 6:
					
					break;
				case 7:
					
					break;
				case 8:
					break;
				case 13:
					break;
				case 14:
					
					break;
				case 15:
					break;
				default:
					break;
			}
	}
}

