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
//unsigned char bA0;
//unsigned char bA1;
//unsigned char bA2;
unsigned char bA7;
unsigned char tmpA;
unsigned char tmpB;
unsigned char keypress;
unsigned char cnt;

//Initialize passcode in array: Pass code = # - X - Y - X
const unsigned char passcode[4] = {0x04, 0x01, 0x02, 0x01};
const unsigned char MAX = 4;

enum States {Start, wait, pass, lock} state;
	
void Tick(){
	
	tmpA = PINA;
	//bA0 = GetBit(tmpA, 0);
	//bA1 = GetBit(tmpA, 1);
	//bA2 = GetBit(tmpA, 2);
	keypress = tmpA & 0x07;
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
				cnt = 0;
				break;
			}
			
			//If keypress matches next pass code entry
			//Accept it as part of the pass code
			if(keypress == passcode[cnt]){	
				state = pass;
				PORTC = state;
				cnt++;
				if(cnt == MAX){
					tmpB = GetBit(tmpB, 0) ? SetBit(tmpB, 0, 0) : SetBit(tmpB, 0, 1);
					cnt = 0;
				}
				break;
			}
			cnt = 0;
			break;
		case pass:
			//If indoor lock button is pressed
			if(bA7){
				state = lock;
				PORTC = state;
				tmpB = SetBit(tmpB, 0, 0);	 //Lock door by setting B0 to 0
				cnt = 0;
				break;
			}
			
			if(!cnt){
				state = (keypress == passcode[MAX - 1]) ? pass : wait;
				break;
			}
			
			//If last passcode entry is still active
			//Button is still pressed, remain in pass state
			if(keypress == passcode[cnt - 1]){
				break;
			}
			
			//If no button is pressed return to wait state
			if(keypress == 0x00){
				state = wait;
				break;
			}
			
			//If keypress matches next pass code entry
			//Accept it as part of the pass code
			if(keypress == passcode[cnt]){	
				cnt++;
				if(cnt == MAX){
					tmpB = GetBit(tmpB, 0) ? SetBit(tmpB, 0, 0) : SetBit(tmpB, 0, 1);
					cnt = 0;
				}
				break;
			}
			
			//Any other input is processed as incorrect
			//returns to wait state
			state = wait;
			PORTC = state;
			cnt = 0;
			break;
		case lock:
			//If indoor lock button is pressed
			if(bA7){
				break;
			}
			
			//If keypress matches next pass code entry
			//Accept it as part of the pass code
			if(keypress == passcode[cnt]){
				state = pass;
				PORTC = state;
				cnt++;
				if(cnt == MAX){
					tmpB = GetBit(tmpB, 0) ? SetBit(tmpB, 0, 0) : SetBit(tmpB, 0, 1);
					cnt = 0;
				}
				break;
			}
			state = wait;
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
	cnt = 0;
	PORTB = 0x00;
	PORTC = Start; 
	
	state = Start;
	while(1){Tick();}
}



