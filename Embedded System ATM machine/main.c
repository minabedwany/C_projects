/*
 * Project 5
 *
 * Created: 6/9/2019 12:13:13 AM
 * Authors: 
 *	Mina Bedwany   69543570
 *	Jace Mica
 */ 

#include <stdio.h>
#include <avr/io.h>
#include "avr.c"
#include "lcd.c"

#define F2 87
#define G2 98

#define ALARM_SIZE 10
#define NUM_USERS 2

typedef struct note {
	unsigned int freq;
	unsigned int dur;
}note;

typedef struct user {
	unsigned int password;
	int balance;
	int locked;
}user;

note alarm_sound[ALARM_SIZE];
user users[NUM_USERS];
static int exit_flag;
static int current_user;
static int lock_counter0 = 0;
static int lock_counter1 = 0;

int play_song(note * song);
void play_note(unsigned int freq, unsigned int duration);
void wait1(unsigned int freq);
void init_songs();

int is_pressed(int r, int c);
int get_key();

void alarm();
int checkpass(user * userr);
int input_pass();
int input_amount1();
int input_choice();
//void input_amount(char* myString);
void manage_account(user * userr);
//void concatenate(char p[], char q[]);
void unlock(int num);

void unlock(int num) {
	users[num].locked = 0;
	if(num == 0)
		lock_counter0 = 0;
	if(num == 1)
		lock_counter1 = 0;
}

void manage_account(user * userr) {
	int in_account = 1;
	while(in_account) {
		lcd_clr();
		char balance[14];
		sprintf(balance, "Balance: $%d", userr[current_user].balance);
		lcd_pos(0,0);
		lcd_puts2(balance);
		
		const char * choices = "1:Wthdrw 2:Dpost";
		lcd_pos(1,0);
		lcd_puts2(choices);
		
		//const char * enter_amount = "Enter Amount:";
		const char * line1= "1: $50  2: $100";
		const char * line2= "3: $500 4: $1000";
		
		int correct_amount = 0;
		
		int key = get_key();
		avr_wait(300);
		switch(key) {
			/* Button 0 = Log Out
			   Button 1 = Withdraw
			   Button 2 = Deposit
			   Button A = Change PIN
			 */
			
			case 1:
				/*lcd_clr();
				lcd_pos(0,0);
				lcd_puts2(enter_amount);*/
				
				lcd_clr();
				lcd_pos(0,0);
				lcd_puts2(line1);
				
				lcd_pos(1,0);
				lcd_puts2(line2);
				
				//correct_amount = input_amount1();
				correct_amount = input_choice();
				
				if(userr[current_user].balance - correct_amount < 0) {
					lcd_clr();
					const char * negative = "Not Enough Funds";
					lcd_pos(0,0);
					lcd_puts2(negative);
					avr_wait(2500);
				}
				else {
					userr[current_user].balance -= correct_amount;
				}
				
				avr_wait(500);
				break;
			case 2:
				/*lcd_clr();
				lcd_pos(0,0);
				lcd_puts2(enter_amount);*/
				
				lcd_clr();
				lcd_pos(0,0);
				lcd_puts2(line1);
				
				lcd_pos(1,0);
				lcd_puts2(line2);
				
				//correct_amount = input_amount1();
				correct_amount = input_choice();
				
				
				if(userr[current_user].balance + correct_amount > 9999) {
					lcd_clr();
					const char * max = "Exceeded Limit!";
					lcd_pos(0,0);
					lcd_puts2(max);
					avr_wait(2500);
				}
				else {
					userr[current_user].balance += correct_amount;
				}
				avr_wait(500);
				break;
			case 4:
				lcd_clr();
				const char * new_pin = "Enter new PIN:";
				lcd_pos(0,0);
				lcd_puts2(new_pin);
				
				int pass = input_pass();
				userr[current_user].password = pass;
				
				lcd_clr();
				const char * changed = "PIN changed to:";
				lcd_pos(0,0);
				lcd_puts2(changed);
				
				char pin[4];
				sprintf(pin, "%d", userr[current_user].password);
				lcd_pos(1,0);
				lcd_puts2(pin);
				
				avr_wait(2000);
				break;
			case 14:
				in_account = 0;
				exit_flag = 1;
				break;
			
			default:
				break;
		}
	}
}

/*void concatenate(char p[], char q[])
{
	int c, d;
	
	c = 0;
	
	while (p[c] != '\0') {
		c++;
	}
	
	d = 0;
	
	while (q[d] != '\0') {
		p[c] = q[d];
		d++;
		c++;
	}
	
	p[c] = '\0';
}

void input_amount(char * myString) {
	int i = 0;
	char num[1];
	int done = 0;
	
	while(!done && i < 4) {
		int key = get_key();
		switch(key) {
			//Button A = Done (Finished entering amount)
			
			case 1:
				//amount += (digit[i]*1);
				sprintf(num, "%d", 1);
				lcd_pos(1,i);
				lcd_puts2(num);
				concatenate(myString, "1");
				i++;
				break;
			case 2:
				
				//amount += (digit[i]*2);
				sprintf(num, "%d", 2);
				lcd_pos(1,i);
				lcd_puts2(num);
				concatenate(myString, "2");
				i++;
				break;
			case 3:
				
				//amount += (digit[i]*3);
				sprintf(num, "%d", 3);
				lcd_pos(1,i);
				lcd_puts2(num);
				concatenate(myString, "3");
				i++;
				break;
			case 4:
				done = 1;
				break;
			case 5:
				
				//amount += (digit[i]*4);
				sprintf(num, "%d", 4);
				lcd_pos(1,i);
				lcd_puts2(num);
				concatenate(myString, "4");
				i++;
				break;
			case 6:
				
				//amount += (digit[i]*5);
				sprintf(num, "%d", 5);
				lcd_pos(1,i);
				lcd_puts2(num);
				concatenate(myString, "5");
				i++;
				break;
			case 7:
				
				//amount += (digit[i]*6);
				sprintf(num, "%d", 6);
				lcd_pos(1,i);
				lcd_puts2(num);
				concatenate(myString, "6");
				i++;
				break;
			case 9:
				
				//amount += (digit[i]*7);
				sprintf(num, "%d", 7);
				lcd_pos(1,i);
				lcd_puts2(num);
				concatenate(myString, "7");
				i++;
				break;
			case 10:
				
				//amount += (digit[i]*8);
				sprintf(num, "%d", 8);
				lcd_pos(1,i);
				lcd_puts2(num);
				concatenate(myString, "8");
				i++;
				break;
			case 11:
				
				//amount += (digit[i]*9);
				sprintf(num, "%d", 9);
				lcd_pos(1,i);
				lcd_puts2(num);
				concatenate(myString, "9");
				i++;
				break;
			case 14:
				
				//amount += (digit[i]*0);
				sprintf(num, "%d", 0);
				lcd_pos(1,i);
				lcd_puts2(num);
				concatenate(myString, "0");
				i++;
				break;
			//default:
				//break;
		}
		avr_wait(300);
	}
	//return myString;
}*/

int input_pass() {
	int pass = 0;
	int i = -1;
	const char * star ="*";
	int digit[4] = {1000, 100, 10, 1};
	
	while(i < 3) {
		int key = get_key();
		switch(key) {
			case 1:
				i++;
				pass += (digit[i]*1);
				lcd_pos(1,i);
				lcd_puts2(star);
				break;
			case 2:
				i++;
				pass += (digit[i]*2);
				lcd_pos(1,i);
				lcd_puts2(star);
				break;
			case 3:
				i++;
				pass += (digit[i]*3);
				lcd_pos(1,i);
				lcd_puts2(star);
				break;
			case 5:
				i++;
				pass += (digit[i]*4);
				lcd_pos(1,i);
				lcd_puts2(star);
				break;
			case 6:
				i++;
				pass += (digit[i]*5);
				lcd_pos(1,i);
				lcd_puts2(star);
				break;
			case 7:
				i++;
				pass += (digit[i]*6);
				lcd_pos(1,i);
				lcd_puts2(star);
				break;
			case 9:
				i++;
				pass += (digit[i]*7);
				lcd_pos(1,i);
				lcd_puts2(star);
				break;
			case 10:
				i++;
				pass += (digit[i]*8);
				lcd_pos(1,i);
				lcd_puts2(star);
				break;
			case 11:
				i++;
				pass += (digit[i]*9);
				lcd_pos(1,i);
				lcd_puts2(star);
				break;
			case 14:
				i++;
				pass += (digit[i]*0);
				lcd_pos(1,i);
				lcd_puts2(star);
				break;
			//default:
				//break;
		}
		avr_wait(300);
	}
	return pass;
}

int input_choice() {
	int still_picking = 1;
	int result = 0;
	while(still_picking) {
		int key = get_key();
		switch(key) {
			case 1:
				result = 50;
				still_picking = 0;
				break;
			case 2:
				result = 100;
				still_picking = 0;
				break;
			case 3:
				result = 500;
				still_picking = 0;
				break;
			case 5:
				result = 1000;
				still_picking = 0;
				break;
			default:
				break;
		}
	}
	return result;
}

int input_amount1() {
	int pass = 0;
	int i = -1;
	char num1[1];
	int digit[3] = {100, 10, 1};
	
	while(i < 2) {
		int key = get_key();
		switch(key) {
			case 1:
				i++;
				pass += (digit[i]*1);
				sprintf(num1, "%d", 1);
				lcd_pos(1,i);
				lcd_puts2(num1);
			break;
			case 2:
				i++;
				pass += (digit[i]*2);
				sprintf(num1, "%d", 2);
				lcd_pos(1,i);
				lcd_puts2(num1);
				break;
			case 3:
			i++;
				pass += (digit[i]*3);
				sprintf(num1, "%d", 3);
				lcd_pos(1,i);
				lcd_puts2(num1);
				break;
			case 5:
				i++;
				pass += (digit[i]*4);
				sprintf(num1, "%d", 4);
				lcd_pos(1,i);
				lcd_puts2(num1);
				break;
			case 6:
				i++;
				pass += (digit[i]*5);
				sprintf(num1, "%d", 5);
				lcd_pos(1,i);
				lcd_puts2(num1);
				break;
			case 7:
				i++;
				pass += (digit[i]*6);
				sprintf(num1, "%d", 6);
				lcd_pos(1,i);
				lcd_puts2(num1);
				break;
			case 9:
				i++;
				pass += (digit[i]*7);
				sprintf(num1, "%d", 7);
				lcd_pos(1,i);
				lcd_puts2(num1);
				break;
			case 10:
				i++;
				pass += (digit[i]*8);
				sprintf(num1, "%d", 8);
				lcd_pos(1,i);
				lcd_puts2(num1);
				break;
			case 11:
				i++;
				pass += (digit[i]*9);
				sprintf(num1, "%d", 9);
				lcd_pos(1,i);
				lcd_puts2(num1);
				break;
			case 14:
				i++;
				pass += (digit[i]*0);
				sprintf(num1, "%d", 0);
				lcd_pos(1,i);
				lcd_puts2(num1);
				break;
			default:
				break;
		}
		avr_wait(300);
	}
	return pass;
}

int checkpass(user * userr) {
	lcd_clr();
	const char * start_pin = "Enter PIN:";
	
	if(userr[current_user].locked){
		return 0;
	}
	
	lcd_pos(0,0);
	lcd_puts2(start_pin);
	
	int pass = input_pass();
	if(pass == userr[current_user].password) {
		lcd_clr();
		const char * accept = "PIN Authorized!";
		lcd_pos(0,0);
		lcd_puts2(accept);
		avr_wait(2500);
		return 1;
	}
	
	lcd_clr();
	const char * decline = "Wrong PIN!";
	lcd_pos(0,0);
	lcd_puts2(decline);
	alarm();
	alarm();
	return 0;	
}

void alarm() {
	SET_BIT(PORTA, 0);
	avr_wait(100);
	CLR_BIT(PORTA, 0);
	avr_wait(100);
	SET_BIT(PORTA, 0);
	avr_wait(100);
	CLR_BIT(PORTA, 0);
	avr_wait(100);
	
	if(play_song(alarm_sound)) {
		SET_BIT(PORTA, 0);
		avr_wait(100);
		CLR_BIT(PORTA, 0);
		avr_wait(100);
		SET_BIT(PORTA, 0);
		avr_wait(100);
		CLR_BIT(PORTA, 0);
		avr_wait(100);
	}
}

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

int play_song(note * song)
{
	int i, n;

	n = ALARM_SIZE; 

	for(i = 0; i < n; i++)
	{
		play_note(song[i].freq, song[i].dur);
	}
	return 1;
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
	
	alarm_sound[0].freq = G2;
	alarm_sound[0].dur = 2;
	
	alarm_sound[1].freq = F2;
	alarm_sound[1].dur = 2;
	
	alarm_sound[2].freq = G2;
	alarm_sound[2].dur = 2;
	
	alarm_sound[3].freq = F2;
	alarm_sound[3].dur = 2;
	
	alarm_sound[4].freq = G2;
	alarm_sound[4].dur = 2;
	
	alarm_sound[5].freq = F2;
	alarm_sound[5].dur = 2;
	
	alarm_sound[6].freq = G2;
	alarm_sound[6].dur = 2;
	
	alarm_sound[7].freq = F2;
	alarm_sound[7].dur = 2;
	
	alarm_sound[8].freq = G2;
	alarm_sound[8].dur = 2;
	
	alarm_sound[9].freq = F2;
	alarm_sound[9].dur = 2;
	
}


int main(void)
{
	avr_init();
	lcd_init();
	init_songs();
	
	users[0].password = 1234;
	users[0].balance = 5000;
	
	users[1].password = 0000;
	users[1].balance = 9999;
	
	exit_flag = 0;
	
	SET_BIT(DDRB, 4);
	SET_BIT(DDRA, 0);

	lcd_clr();
	const char * start_message = "Welcome to";
	lcd_pos(0,0);
	lcd_puts2(start_message);
	
	const char * start_message1 = "Bank ATM!";
	lcd_pos(1,0);
	lcd_puts2(start_message1);
	
	avr_wait(2500);
	
	
	while (1) 
    {
		lcd_clr();
		const char * start_message2 = "1:Mina's Account";
		lcd_pos(0,0);
		lcd_puts2(start_message2);
		
		const char * start_message3 = "2:Jace's Account";
		lcd_pos(1,0);
		lcd_puts2(start_message3);
		
		if(!exit_flag) {
			int key = get_key();
			int authorized;
			avr_wait(300);
			switch(key) 
			{
				case 1:
					current_user = 0;
					authorized = checkpass(users);
					if(!authorized) {
						authorized = checkpass(users);
						if(!authorized) {
							lcd_clr();
							const char * banned1 = "YOU GOT LOCKED!";
							lcd_pos(0,0);
							lcd_puts2(banned1);
							avr_wait(1500);
							users[current_user].locked = 1;
							exit_flag = 1;
							break;
						}
						else if(authorized)
							manage_account(users);
					}
					else if(authorized) {
						manage_account(users);
					}
					break;
				case 2:
					current_user = 1;
					authorized = checkpass(users);
					if(!authorized) {
						authorized = checkpass(users);
						if(!authorized) {
							lcd_clr();
							const char * banned2 = "YOU GOT LOCKED!";
							lcd_pos(0,0);
							lcd_puts2(banned2);
							avr_wait(2500);
							users[current_user].locked = 1;
							exit_flag = 1;
							break;
						}
						else if(authorized)
							manage_account(users);
					}
					else if(authorized) {
						manage_account(users);
					}
					break;
					
					case 12:
						lock_counter0++;
						if(lock_counter0 == 4) {
							unlock(0);
						}
						break;
					
					case 16:
						lock_counter1++;
						if(lock_counter1 == 4) {
							unlock(1);
						}
						break;
				
				default:
					break;
			}
		}
		else if(exit_flag) {
			lcd_clr();
			const char * thank_you = "Thank you! Bye.";
			lcd_pos(0,0);
			lcd_puts2(thank_you);
			avr_wait(2500);
			exit_flag = 0;
		}
	}
}

