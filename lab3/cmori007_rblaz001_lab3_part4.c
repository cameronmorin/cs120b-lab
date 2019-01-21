/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 4 
 *	Exercise Description: Write the upper nibble of input A to the lower nibble of B.
						  Write the lower nibble of input A to the upper nibble of C.
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

// Bit-access function

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	
	// initialize to 0s
	unsigned char tmpLowerA; // Intermediate variable used to set lower nibble of A to upper nibble of C
	unsigned char tmpUpperA; // Intermediate variable used to set upper nibble of A to lower nibble of B

	while(1)
	{
		tmpLowerA = PINA & 0x0F;
		tmpUpperA = PINA & 0xF0;
		
		PORTB = PORTB & 0xF0;
		PORTB = PORTB | (tmpUpperA >> 4);
		
		PORTC = PORTC & 0x0F;
		PORTC = PORTC | (tmpLowerA << 4);
	}
}