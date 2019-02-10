/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 8  Exercise 3
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>

//Temporary variable to hold output
unsigned char tmpB = 0x00;

const unsigned short MAX = 128;  //Needs to be updated with max result of exercise 2

unsigned short tmpADC = 0x0000;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

enum States {Start, ST_Running} state;



void Tick(){
	
	tmpADC = ADC;

	switch(state){
		
		case Start:
			state = ST_Running;
			break;

		case ST_Running:
			break;

		default:
			break;

	}
	

	switch(state){

		case Start:
			break;

		case ST_Running:
			
			if(tmpADC >= MAX*7/8){
				tmpB = 0xFF;
			}
			else if(tmpADC >= MAX*6/8){
				tmpB = 0x7F;
			}
			else if(tmpADC >= MAX*5/8){
				tmpB = 0x3F;
			}
			else if(tmpADC >= MAX*4/8){
				tmpB = 0x1F;
			}
			else if(tmpADC >= MAX*3/8){
				tmpB = 0x0F;
			}
			else if(tmpADC >= MAX*2/8){
				tmpB = 0x07;
			}
			else if(tmpADC >= MAX/8){
				tmpB = 0x03;
			}
			else if(tmpADC >= 0){
				tmpB = 0x01;
			}
			
			break;

		default:
			break;

	}

	PORTB = tmpB;

}



int main(void){

	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as output, initialize to 0s

	// Initialize outputs
	
	ADC_init();

	PORTB = 0x00;

	state = Start;
	
	TimerSet(100);
	TimerOn();

	while(1){
		Tick();
		while(!TimerFlag){};
		TimerFlag = 0;
	}

}