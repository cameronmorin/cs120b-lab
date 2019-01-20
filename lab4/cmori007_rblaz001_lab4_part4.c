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
unsigned char bA1;
unsigned char bA2;
unsigned char bA7;
unsigned char tmpA;
unsigned char tmpB;

enum States {Start, wait, pass1, pass2, lock} state;
	
void Tick(){
	
	tmpA = PINA;
	bA0 = GetBit(tmpA, 0);
	bA1 = GetBit(tmpA, 1);
	bA2 = GetBit(tmpA, 2);
	bA7 = GetBit(tmpA, 7);
	tmpB = PORTB;
	
	switch(state){
		case Start:
			state = wait;
			PORTC = state;
			break;
		case wait:
			//If indoor lock button is pressed
			if(bA7){
				state = lock;
				PORTC = state;					 //PORTC is only written to on state transition
				tmpB = SetBit(tmpB, 0, 0);	 //Lock door by setting B0 to 0
				break;
			}
			
			//If # is the only button pressed
			//Accept it as the first part of the pass code
			if(bA2 && !bA1 && !bA0){	
				state = pass1;
				PORTC = state;
			}
			break;
		case pass1:
			//If indoor lock button is pressed
			if(bA7){
				state = lock;
				PORTC = state;
				tmpB = SetBit(tmpB, 0, 0);	 //Lock door by setting B0 to 0
				break;
			}
			
			//if no button is pressed remain in pass1
			if(bA2 && bA1 && bA0){
				break;
			}
			
			//If y is the only button pressed
			//Accept it as the second part of the pass code
			if(!bA2 && bA1 && !bA0){
				state = pass2;
				PORTC = state;
				tmpB = GetBit(tmpB, 7) ? SetBit(tmpB, 0, 0) : SetBit(tmpB, 0, 1);	 //If Door is unlocked lock door else unlock door
				break;
			}
			
			//Any other input is processed as incorrect
			//returns to wait state
			state = wait;
			PORTC = state;
			break;
			
		case pass2:
			//If indoor lock button is pressed
			if(bA7){
				state = lock;
				PORTC = state;
				tmpB = SetBit(tmpB, 0, 0);	 //Lock door by setting B0 to 0
				break;
			}
			
			//If y is the only button pressed
			//Remain in pass2 until button is released
			if(!bA2 && bA1 && !bA0){
				break;
			}
			
			//If # is the only button pressed
			//Accept it as the first part of the pass code
			if(bA2 && !bA1 && !bA0){
				state = pass1;
				PORTC = state;
				break;
			}
			
			//Any other input returns to wait state
			state = wait;
			PORTC = state;
			break;
		case lock:
			//If indoor lock button is pressed
			if(bA7){
				break;
			}
			
			//If # is the only button pressed
			//Accept it as the first part of the pass code
			if(bA2 && !bA1 && !bA0){
				state = pass1;
				PORTC = state;
				break;
			}
			
			//Any other input returns to wait state
			state = wait;
			PORTC = state;
			break;
		default:
			break;
	}
	PORTB = tmpB;	
}

int main(void){
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as inputs, initialize to 1s
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	
	// Initialize outputs
	PORTB = 0x00;
	PORTC = Start; 
	
	state = Start;
	while(1){Tick();}
}



