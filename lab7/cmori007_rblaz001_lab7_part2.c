/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description: LED reaction light game. Three LED lights alternate every 300ms, pressing the button when the center LED light is on
 *						  increments the score on the LCD display. Every failure decrements the score. Reaching 9 points displays victory screen.	
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>
#include <avr/io.h>

//Temporary variable used to hold input

unsigned char tmpA = 0x00;

unsigned char tmpB = 0x00;

unsigned char bLeft; //Bit 0 used to determine if sequence is going right or left
unsigned char cnt;


enum States {Start, ST_B0, ST_B1, ST_B2, ST_wait, ST_pause, ST_victory} state;



void Tick(){

	

	tmpA = ~PINA & 0x01;

	switch(state){
		
		case Start:
			state = ST_B0;  //Initialize to state ST_B0
			break;

		case ST_B0:
			//if button is not pressed continue sequence
			if(!tmpA){
				state = ST_B1;
				bLeft = 0x01;
			}
			//if button is pressed cnt is decremented and set state to ST_wait, incorrect timing
			else{
				if(cnt > 0){
					cnt--;
				}
				state = ST_wait;
			}
			break;

		case ST_B1:
			//if button is not pressed continue sequence depending on bLeft
			if(!tmpA){
				state = bLeft ? ST_B2 : ST_B0;
			}
			//if button is pressed cnt is incremented and state is set to wait, correct timing
			else{
				if(cnt < 9){
					cnt++;
				}
				
				if(cnt = 9){
					state = ST_victory;
					break;
				}
				state = ST_wait;
			}
			break;
			
		case ST_B2:
			//if button is not pressed continue sequence depending on left
			if(!tmpA){
				state = ST_B1;
				bLeft = 0x00;
			
			//if button is pressed cnt is decremented and state is set to wait, decrement timing
			else{
				if(cnt > 0){
					cnt--;
				}
				state = ST_wait;
			}
			break;
			
		case ST_wait:
			//if button remains pressed remain in state waiting
			if (tmpA){
				break;
			}
			//else state is set to ST_pause
			state = ST_pause;
			break;
			
		case ST_pause:
			//wait for button to be pressed in order to restart game
			if (!tmpA){
				break;
			}
			//if button is pressed restart game by setting state to ST_B0
			state = ST_B0;
			break;
			
		case ST_victory:
			if(!tmpA){
				state = ST_pause;
				break;
			}
			state = ST_B0;
			break;

		default:
			break;

	}
	

	switch(state){

		case Start:
			break;

		case ST_B0:
			tmpB = 0x01;
			break;

		case ST_B1:
			tmpB = 0x02;
			break;

		case ST_B2:
			tmpB = 0x04;
			break;
		
		case ST_wait:
			//display in LCD screen
			break;
		
		case ST_pause:
			break;
			
		case ST_victory:
			cnt = 0;
			break;

		default:
			break;

	}

	PORTB = tmpB;

}



int main(void){

	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s

	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as inputs, initialize to 1s

	

	// Initialize outputs

	PORTB = 0x00;
	bLeft = 0x01;

	state = Start;
	cnt = 0;
	
	TimerSet(300);
	TimerOn();

	while(1){
		Tick();
		while(!TimerFlag){};
		TimerFlag = 0;
	}

}