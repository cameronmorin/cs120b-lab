/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 2 
 *	Exercise Description: Car seat sensor used for airbag display in car
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}


int main(void)
{
	DDRB = 0x00; PORTB = 0x01; // Configure port B's first pin as input
	DDRD = 0x00; PORTD = 0xFF; // Configure port D's 8 pins as inputs
	

	unsigned char tmpD;
	unsigned char tmpB;
	unsigned short totalWeight = 0x0000; 

	const unsigned char SEVENTY = 0x46;
	const unsigned char FIVE = 0x05;

	while(1)
	{
		tmpD = PIND;
		tmpB = PINB & 0x01;
		
		totalWeight = (tmpD << 1) + tmpB;
		
		if (totalWeight >= SEVENTY) {
			tmpB = SetBit(tmpB,1, 1);
		}
		else if (totalWeight < SEVENTY && totalWeight > FIVE) {
			tmpB = SetBit(tmpB,2, 1); 
		}
		else {
			tmpB = tmpB & 0x00;
		}
		
		PORTB = tmpB;
	}
}