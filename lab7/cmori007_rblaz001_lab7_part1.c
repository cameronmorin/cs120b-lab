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
			state = wait;   //Initialize to wait state
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData(tmpB);
			break;
		case wait:
			//Remain in wait state if no button is pressed
			if(!bA0 && !bA1){
				break;
			}
			//if A0 is pressed set cnt to 0 and enter inc state
			else if(bA0 && !bA1){
				cnt = 0;
				state = inc;
				break;
			}
			//if A1 is pressed set cnt to 0 and enter dec state
			else if(!bA0 && bA1){
				cnt = 0;
				state = dec;
				break;
			}
			//if both A0 & A1 is pressed enter rst state and set B to 0
			else if (bA0 && bA1){
				state = rst;
				tmpB = 0;
			}
			LCD_WriteData(tmpB);
			break;
		case inc:
			//if button is released enter wait sate and increment B if less than 9
			if(!bA0 && !bA1){
				state = wait;
				if(tmpB < 9){
					tmpB++;
				}
				break;
			} 
			//if A0 remains pressed, remain in inc state for 1 second and increment B if less than 9 and return to wait
			else if(bA0 && !bA1){
				if(cnt < 20){
					cnt++;
				}
				else{
					cnt = 0;
					state = wait;
					if(tmpB < 9){
						tmpB++;
					}
				}
				break;
			}
			//if A1 is pressed and A0 is released set state to dec, increment B, and set cnt to 0
			else if(!bA0 && bA1){
				cnt = 0;
				state = dec;
				if(tmpB < 9){
					tmpB++;
				}
				break;
			}
			//if both A0 & A1 is pressed set cnt to 0, set state to rst, and set B to 0
			else if (bA0 && bA1){
				cnt = 0;
				state = rst;
				tmpB = 0;
			}
			LCD_WriteData(tmpB);
			break;
		case dec:
			//if A1 is released set cnt to 0, set state to wait, and decrement B if B is greater than 0
			if(!bA0 && !bA1){
				cnt = 0;
				state = wait;
				if(tmpB > 0){
					tmpB--;
				}
				break;
			}
			//if A0 is pressed and A1 is released set state to dec, decrement B, and set cnt to 0
			else if(bA0 && !bA1){
				cnt = 0;
				state = inc;
				if(tmpB > 0){
					tmpB--;
				}
				break;
			}
			//if A1 remains pressed, remain in dec state for 1 second and decrement B if less than 9 and return to wait
			else if(!bA0 && bA1){
				if(cnt < 20){
					cnt--;
				}
				else{
					cnt = 0;
					state = wait;
					if(tmpB > 0){
						tmpB--;
					}
				}
				break;
			}
			//if both A0 & A1 is pressed, set cnt to 0, enter rst state, and set B to 0
			else if (bA0 && bA1){
				cnt = 0;
				state = rst;
				tmpB = 0;
			}
			LCD_WriteData(tmpB);
			break;
		case rst:
			//if both A0 & A1 is released enter wait state
			if(!bA0 && !bA1){
				state = wait;
				break;
			}
			//if A1 is released and A1 remains pressed, remain in rst state until both are released
			else if(bA0 && !bA1){
				break;
			}
			//if A0 is released and A1 remains pressed, remain in rst state until both are released
			else if(!bA0 && bA1){
				break;
			}
			//if both A0 & A1 remained pressed remain in rst state
			else if (bA0 && bA1){
			}
			break;
		default:
			break;
	}
	PORTB = tmpB;	
}

int main(void){
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	PORTB = 0; // Initialize outputs
	cnt = 0;
	state = Start;
	
	TimerSet(50);
	TimerOn();

	while(1){
		Tick();
		
		while(!TimerFlag){};
			TimerFlag = 0;
	}
}

