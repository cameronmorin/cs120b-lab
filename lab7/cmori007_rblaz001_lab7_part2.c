/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description: LED reaction light game. Three LED lights alternate every 300ms, pressing the button when the center LED light is on
 *						  increments the score on the LCD display. Every failure decrements the score. Reaching 9 points displays victory screen.	
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>
#include "io.c"

//Temporary variable used to hold input

unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;
unsigned char win;
unsigned char score;
unsigned char cnt;
unsigned char x;
unsigned char from;
enum States {Start, L1, L2, L3, PRESS, WAIT_1, WAIT_2} state;


void press(const unsigned char n) {
	if (n == 0) {
		if (score > 0) {
			score--;
			LCD_WriteData(score + '0');
		}
	}
	else {
		if (score == 8) {
			score = 5;
			LCD_ClearScreen();
			LCD_DisplayString(1, "Winner!!");
			win = 1;
		}
		else {
			score++;
			LCD_WriteData(score + '0');
		}
	}
}

void Tick(){
	tmpA = ~PINA & 0x01;

	switch(state) {
		case Start:
			cnt = 0;
			score = 5;
			win = 0;
			LCD_Cursor(1);
			LCD_WriteData(score + '0');
			break;
		case L1:
			if (!tmpA && cnt >= 5) {
				state = L2;
				cnt = 0;
				from = 1;
			}
			else if (tmpA) {
				state = PRESS;
				cnt = 0;
				x = 0;
				press(x);
			}
			break;
		case L2:
			if (!tmpA && cnt >= 5 && from) {
				state = L3;
				cnt = 0;
				from = 0;
			}
			else if (!tmpA && cnt >= 5 && !from) {
				state = L1;
				cnt = 0;
			}
			else if (tmpA) {
				state = PRESS;
				cnt = 0;
				x = 1;
				press(x);
			}
			break;
		case L3:
			if (!tmpA && cnt >= 5) {
				state = L2;
				cnt = 0;
			}
			else if (tmpA) {
				state = PRESS;
				cnt = 0;
				x = 0;
				press(x);
			}
			break;
		case PRESS:
			if (!tmpA) {
				state = WAIT_1;
			}
			break;
		case WAIT_1:
			if (tmpA) {
				state = WAIT_2;
			}
			break;
		case WAIT_2:
			if (!tmpA) {
				if (win == 1) {
					LCD_ClearScreen();
					LCD_Cursor(1);
					LCD_WriteData(score + '0');
					win = 0;
				}
			}
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start:
			state = L1;
			break;
		case L1:
			tmpB = 0x01;
			cnt++;
			break;
		case L2:
			tmpB = 0x02;
			cnt++;
			break;
		case L3:
			tmpB = 0x04;
			cnt++;
			break;
		case PRESS:
			break;
		case WAIT_1:
			break;
		case WAIT_2:
			break;
		default:
			state = Start;
			break;
	}

	PORTB = tmpB;

}



int main(void){
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as inputs, initialize to 1s
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	// Initialize outputs

	PORTB = 0x00;
	state = Start;
	
	LCD_Init();
	
	TimerSet(100);
	TimerOn();

	while(1){
		Tick();
		while(!TimerFlag){};
		TimerFlag = 0;
	}

}