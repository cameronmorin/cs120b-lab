/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munos rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 2  Exercise 1 
 *	Exercise Description: Parking Garage Sensor
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>

int main(void)
{
	DDRB = 0xFF; // Configure port B's 8 pins as outputs
	PORTB = 0x00; // Initialize PORTB output to 0’s
	unsigned char tmpA = 0x00; //Temporary value to hold the value of A
	unsigned char tmpB = 0x00; //Temporary value to hold the value of B
	while(1)
	{
		tmpA = PINA & 0x03; //Read in the last two bits of A
		if (tmpA == 0x01) { //Check if PA1 is 0 and PA0 is 1
			tmpB = (tmpB & 0xFC) | 0x01; //Clear the rightmost 2 bits and set to 0x01
		}
		else {
			tmpB = (tmpB & 0xFC) | 0x00; //Clear the rightmost 2 bits and set to 0x00
		}
		PORTB = tmpB; //Write output
	}
	return 0;
}
