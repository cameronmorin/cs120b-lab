/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 4  Exercise 1 
 *	Exercise Description: A system with two LEDs and one button. One LED is always on
 *						  by pressing and releasing the button, the active LED turns off and the other turns on.
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

enum States {Start, state1, state2, state3, state4} state;
	
void Tick(){
	
	bA0 = PINA & 0x01;
	
	switch(state){
		case Start:
			state = state1;
			break;
		case state1:
			state = bA0 ? state2 : state1; //If button is pressed move on to next state
			break;
		case state2:
			state = bA0 ? state2 : state3; //If button remains pressed remain in state
			break;
		case state3:
			state = bA0 ? state4 : state3; //If button is pressed move on to next state
			break;
		case state4:
			state = bA0 ? state4 : state1; //If button remains pressed remain in state
		default:
			break;
	}
	
		switch(state){
			case Start:			//B0 and B1 are already initialized in main.
				break;
			case state1:
				break;
			case state2:		//B0 = 0; B1 = 1;
				PORTB = SetBit(PORTB, 0, 0);
				PORTB = SetBit(PORTB, 1, 1);
				break;
			case state3:
				break;
			case state4:		//B0 = 1; B1 = 0;
				PORTB = SetBit(PORTB, 0, 1);
				PORTB = SetBit(PORTB, 1, 0);
				break;
			default:
				break;
		}
		
}

int main(void){
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	PORTB = 0x00; // Initialize outputs
	state = Start;
	while(1){Tick();}
}

