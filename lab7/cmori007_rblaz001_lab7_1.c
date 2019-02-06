/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu

						      Raudel Blazquez Munoz rblaz001@ucr.edu

 *	Lab Section: 21

 *	Assignment: Lab 7  Exercise 1

 *	Exercise Description: Two buttons are connected to increment and decrement a count.

	if the count exceeds 9, it does not increase. If the tries to decrease lower than 0, it stays

	at 0. If both buttons are pressed, the count is reset to 0. If a button is held the count is updated every second.

 *	

 *	I acknowledge all content contained herein, excluding template or example

 *	code, is my own original work.

 */



#include <avr/io.h>

#include <avr/interrupt.h>

#include <timer.h>

#include "io.c"



//Bit access function

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {

	return (b ? x | (0x01 << k) : x & ~(0x01 << k));

}

unsigned char GetBit(unsigned char x, unsigned char k) {

	return ((x & (0x01 << k)) != 0);

}



//Temporary variable used to hold input

unsigned char bA0;   //Variable to track the 1 bit input A0

unsigned char bA1;   //Variable to track the 1 bit input A1

unsigned char tmpA;  //Variable to track the 8 bit input A





unsigned char tmpB;	  //Variable to track the 8 bit output B

unsigned char cnt;    //Variable used to track how long button is pressed



enum States {Start, wait, inc, dec, rst} state;

	

void Tick(){
	tmpA = ~PINA;				//Flip the input since the button is set up in pull up mode
	bA0 = GetBit(tmpA, 0);
	bA1 = GetBit(tmpA, 1);
	tmpB = PORTB;


	switch(state){
		case Start:
            state = wait;
			break;
		case wait:
			if(bA0){                //Increase count
				state = inc;
				cnt = 0;
                tmpB++;
				break;
			}
            else if(bA1){           //Decrease count
				state = dec;
				cnt = 0;
                tmpB--;
				break;
			}
            else if(bA0 && bA1){    //Reset
				state = rst;
                tmpB = 0;
			}
			break;
		case inc:                   //Incease Transistions
			if(bA0 && bA1){
				state = rst;
                tmpB = 0;
			}
            else if(!bA0){
				state = wait;
			}
		case dec:                   //Decrease Transistions
            if(bA0 && bA1){
				state = rst;
                tmpB = 0;
			} 
            else if(!bA1){
				state = wait;
			}
		case rst:                   //Reset Transistions
            if(!bA0 && !bA1) {
                state = wait;
            }
			break;
		default:
            state = Start;
			break;

	}

	switch (state){
		case Start:
			break;
		case wait:
			break;
		case inc:                   //Increase State
			if(cnt < 10){
				cnt++;
			}
			else{
				if(tmpB < 9){
    				tmpB++;
				}
				cnt = 0;
			}
			break;
		case dec:                   //Decrease State
			if(cnt < 10){
    			cnt++;
			}
			else{
    			if(tmpB > 0){
        			tmpB--;
    			}
    			cnt = 0;
			}
			break;
        case rst:
            break;
		default:
			break;
    }

	PORTB = tmpB;	

}



int main(void){
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	PORTB = 0; // Initialize outputs
	cnt = 0;
	state = Start;

	LCD_init();
	TimerSet(100);
	TimerOn();

	while(1){
		Tick();
		while(!TimerFlag){};
		TimerFlag = 0;
	}
}