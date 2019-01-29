/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 5  Exercise 3
 *	Exercise Description: Festive LED display using one button input
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

//Temporary variable used to hold input
unsigned char tmpA;
unsigned char tmpB;
unsigned char cnt;

//Initialize LED display array
const unsigned char display[10] = {0x20, 0x10, 0x28, 0x14, 0x2A, 0x15, 0x2A, 0x14, 0x28, 0x10};
const unsigned char MAX = 10;

enum States {Start, wait, next} state;
	
void Tick(){
	
	tmpA = ~PINA & 0x01;
	
	tmpB = PORTB;
	
	switch(state){
		case Start:
			state = wait;
			break;
		case wait:
			if(tmpA){
				state = next;
				if(cnt < (MAX-1)){
					tmpB = display[++cnt];
					state = next;
					break;
				}
				else
				{
					cnt = 0;
					tmpB = display[cnt];
					state = next;
					break;
				}
			}
			break;
		case next:
			if(tmpA){
				break;
			}
			state = wait;
			break;
		default:
			break;
	}
	PORTB = tmpB;	
}

int main(void){
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as inputs, initialize to 1s
	
	// Initialize outputs
	PORTB = 0x20;
	cnt = 0;
	state = Start;
	while(1){Tick();}
}



