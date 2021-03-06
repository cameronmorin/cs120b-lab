/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 5  Exercise 2
 *	Exercise Description: Two butotns are connected to increment and decrement a count.
	if the count excedes 9, it does not increase. If the tries to decrease lower than 0, it stays
	at 0. If both buttons are pressed, the count is reset to 0.
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

//Bit access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

//Temporary variable used to hold input
unsigned char bA0;
unsigned char bA1;
unsigned char tmpA;
unsigned char tmpB;

enum States {Start, wait, inc, dec, rst} state;
	
void Tick(){
	
	tmpA = ~PINA;
	bA0 = GetBit(tmpA, 0);
	bA1 = GetBit(tmpA, 1);
	tmpB = PORTB;
	
	switch(state){
		case Start:
			state = wait;
			break;
		case wait:
			if(!bA0 && !bA1){
				break;
			}
			else if(bA0 && !bA1){
				state = inc;
				if(tmpB < 9){
					tmpB++;
				}
				break;
			}
			else if(!bA0 && bA1){
				state = dec;
				if(tmpB > 0){
					tmpB--;
				}
				break;
			}
			else if (bA0 && bA1){
				state = rst;
				tmpB = 0;
			}
			break;
		case inc:
			if(!bA0 && !bA1){
				state = wait;
				break;
			}
			else if(bA0 && !bA1){
				break;
			}
			else if(!bA0 && bA1){
				state = dec;
				if(tmpB > 0){
					tmpB--;
				}
				break;
			}
			else if (bA0 && bA1){
				state = rst;
				tmpB = 0;
			}
			break;
		case dec:
			if(!bA0 && !bA1){
				state = wait;
				break;
			}
			else if(bA0 && !bA1){
				state = inc;
				if(tmpB < 9){
					tmpB++;
				}
				break;
			}
			else if(!bA0 && bA1){
				break;
			}
			else if (bA0 && bA1){
				state = rst;
				tmpB = 0;
			}
			break;
		case rst:
			if(!bA0 && !bA1){
				state = wait;
				break;
			}
			else if(bA0 && !bA1){
				state = inc;
				if(tmpB < 9){
					tmpB++;
				}
				break;
			}
			else if(!bA0 && bA1){
				state = dec;
				if(tmpB > 0){
					tmpB--;
				}
				break;
			}
			else if (bA0 && bA1){
			}
			break;
		default:
			break;
	}
	PORTB = tmpB;	
}

int main(void){
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	PORTB = 0; // Initialize outputs
	state = Start;
	while(1){Tick();}
}

