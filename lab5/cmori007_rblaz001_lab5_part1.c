/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 5  Exercise 1 
 *	Exercise Description: Car fuel level sensors
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	
	// initialize to 0s
	unsigned char tmpB; // Intermediate variable for port update
	unsigned char tmpA;

	while(1)
	{
		// Assign tmpA the lower nibble of input A
		tmpA = ~PINA & 0x0F;
		
		tmpB = 0x00;
		
		switch(tmpA){
			case  0:
				tmpB = tmpB | 0x40;
				break;
			case  1:
			case  2:
				tmpB = tmpB | 0x60;
				break;
			case  3:
			case  4:
				tmpB = tmpB | 0x70;
				break;
			case  5:
			case  6:
				tmpB = tmpB | 0x38;
				break;
			case  7:
			case  8:
			case  9:
				tmpB = tmpB | 0x3C;
				break;
			case 10:
			case 11:
			case 12:
				tmpB = tmpB | 0x3E;
				break;
			case 13:
			case 14:
			case 15:
				tmpB = tmpB | 0x3F;
			default :
				break;
		}
		
		PORTB = tmpB;
	}
}