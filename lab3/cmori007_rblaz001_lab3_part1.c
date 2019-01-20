/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 1 
 *	Exercise Description: Pin counter for port A and B
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

// Bit-access function
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	
	// initialize to 0s
	unsigned char tmpA = 0x00; // Intermediate variable for port update
	unsigned char tmpB = 0x00; // Intermediate variable for port update
	
	unsigned char cnt; // variable used to count number of ones in A and B
	unsigned char i; // For loop variable

	while(1)
	{
		// initialize to zero
		cnt = 0;
		
		// for loop iterates through A and B to count bits set to 1
		for (i = 0; i < 8; i++){
			if(GetBit(PINA, i)){
				cnt++;
			}
			
			if(GetBit(PINB, i)){
				cnt++;
			}
		}
		
		PORTC = cnt;
	}
}





