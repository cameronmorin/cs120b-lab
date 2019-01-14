/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 2  Exercise 2 
 *	Exercise Description: Parking Garage Sensor
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>

int main(void)
{
	DDRC = 0xFF; // Configure port C's 8 pins as outputs
	PORTC = 0x00; // Initialize PORTC output to 0's
	unsigned char tmpA = 0x00; //Temporary value to hold the value of A
	unsigned char tmpC = 0x00; //Temporary value to hold the value of C
	while(1)
	{
		tmpA = PINA & 0x0F; //Read in the last four bits of A
		if (tmpA == 0x00) { //Check if all spaces are empty
			tmpC = (tmpC & 0xF0) | 0x04; //Clear the rightmost 4 bits and declare 4 free spaces
		}
		else if (tmpA == 0x01 || tmpA == 0x02 || tmpA == 0x04 || tmpA == 0x08) {
			tmpC = (tmpC & 0xF0) | 0x03; //Clear the rightmost 4 bits and declare 3 free spaces
		}
		else if (tmpA == 0x03 || tmpA == 0x05 || tmpA == 0x06 || tmpA == 0x09 || tmpA == 0x0A || tmpA == 0x0C) {
			tmpC = (tmpC & 0xF0) | 0x02; //Clear the rightmost 4 bits and declare 2 free spaces
		}
		else if (tmpA == 0x07 || tmpA == 0x0B || tmpA == 0x0D || tmpA == 0x0E) {
			tmpC = (tmpC & 0xF0) | 0x01; //Clear the rightmost 4 bits and declare 1 free space
		}
		else {
			tmpC = (tmpC & 0xF0) | 0x00; //Clear the rightmost 4 bits and declare 0 free spaces
		}
		PORTC = tmpC; //Write output
	}
	return 0;
}
