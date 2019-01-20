/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 3 
 *	Exercise Description: Car fuel level sensors extended with "Fasten seatbelt" indicator
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	
	// initialize to 0s
	unsigned char tmpC; // Intermediate variable for port update
	unsigned char tmpA;
	unsigned char tmpUpperA;
	

	while(1)
	{
		// Assign tmpA the lower nibble of input A
		tmpA = PINA & 0x0F;
		tmpUpperA = PINA & 0x70;
		
		
		tmpC = 0x00;
		
		switch(tmpA){
			case  0:
				tmpC = tmpC | 0x40;
				break;
			case  1:
			case  2:
				tmpC = tmpC | 0x60;
				break;
			case  3:
			case  4:
				tmpC = tmpC | 0x70;
				break;
			case  5:
			case  6:
				tmpC = tmpC | 0x38;
				break;
			case  7:
			case  8:
			case  9:
				tmpC = tmpC | 0x3C;
				break;
			case 10:
			case 11:
			case 12:
				tmpC = tmpC | 0x3E;
				break;
			case 13:
			case 14:
			case 15:
				tmpC = tmpC | 0x3F;
			default :
				break;
		}
		
		if((tmpUpperA & 0x30) == 0x30){
			tmpC = tmpC | 0x80;
		}		
		else{
			tmpC = tmpC & 0x7F;
		}
		
		PORTC = tmpC;
	}
}