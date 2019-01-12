/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
 *
 *	Lab Section: 21
 *	Assignment: Lab # 2 Exercise # 2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF; // Configure port A's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpC = 0x00; // Temporary variable to hold the value of C
	unsigned char tmpD = 0x00; // Temporary variable to hold the value of D
	unsigned short usSum = 0x0000; // Variable used to check the sum of A, B, and C
	unsigned char ucBalance = 0x00; // Variable used to check balance between edge seats on cart
	
	const unsigned char MAX_WEIGHT = 140; // Max combined weight allowed on cart
	const unsigned char MAX_WEIGHT_DIFF = 80; // Max weight difference between seat A and C. (req. for balancing)
	
	while(1)
	{
		// Read input
		tmpA = PINA;
		tmpB = PINB;
		tmpC = PINC;
		
		usSum = tmpA + tmpB + tmpC; //Sum the three weight sensors for seat A, B, and C

		// Check if max weight of cart has been reached
		// Assign D0 to 1 if max weight is exceeded
		if (usSum > MAX_WEIGHT) { 
			tmpD = tmpD | 0x01;
		}
		// Assign D0 to 0 if total weight is less than max weight
		else {
			tmpD = tmpD & 0xFE;
		}
		
		// Check weight of seat A is greater than C
		// Sets ucBalance variable to weight difference
		if (tmpA > tmpC) {
			ucBalance = tmpA - tmpC;
		}
		else {
			ucBalance = tmpC - tmpA;
		}
		
		// Check if the weight difference between seat A and C exceeds the balance limit
		// Assign D1 to 1 if weight difference exceeds balance limit
		if ( ucBalance > MAX_WEIGHT_DIFF) {
			tmpD = tmpD | 0x02;
		}
		//Assign D1 to 0 if weight difference is within acceptable range
		else{
			tmpD = tmpD & 0xFD;
		}
		
		// Clear bits D7-D3
		tmpD = tmpD & 0x03;
		
		// Divide variable holding total weight by 10
		usSum = usSum / 10;
		
		// Use bits D7-D3 to display approximate weight in binary
		// Range is 0 - 13. Representing acceptable weight range divided by 10
		switch(usSum){
			case 1:
				tmpD = tmpD | 0x08;
				break;
			case 2:
				tmpD = tmpD | 0x10;
				break;
			case 3:
				tmpD = tmpD | 0x18;
				break;
			case 4:
				tmpD = tmpD | 0x20;
				break;
			case 5:
				tmpD = tmpD | 0x28;
				break;
			case 6:
				tmpD = tmpD | 0x30;
				break;
			case 7:
				tmpD = tmpD | 0x38;
				break;
			case 8:
				tmpD = tmpD | 0x40;
				break;
			case 9:
				tmpD = tmpD | 0x48;
				break;
			case 10:
				tmpD = tmpD | 0x50;
				break;
			case 11:
				tmpD = tmpD | 0x58;
				break;
			case 12:
				tmpD = tmpD | 0x60;
				break;
			case 13:
				tmpD = tmpD | 0x68;
				break;
		}
		
		// Set Output
		PORTD = tmpD;
	}
	return 0;
}




