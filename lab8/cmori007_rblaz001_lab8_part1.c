/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 8  Exercise 1
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
unsigned char tmpD = 0x00;

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
			tmpB = (char)tmpADC;			//set the lower 8 bit ADC to tmpB
			tmpD = (char)(tmpADC >> 8);		//set the upper 10 bit of ADC to tmpD
			break;

		default:
			break;

	}

	PORTB = tmpB;
	PORTD = tmpD;

}



int main(void){

	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as output, initialize to 0s
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as output, initialized to 0s

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