/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 9  Exercise 1
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>

//Temporary variable to hold input
unsigned char tmpA = 0x00;
unsigned char bA0 = 0x00;
unsigned char bA1 = 0x00;
unsigned char bA2 = 0x00;

//Temporary variable to hold output
unsigned char tmpB = 0x00;

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}


enum States {Start, ST_Running} state;



void Tick(){
	
	tmpA = ~PINA;
	bA0 = (tmpA & 0x01);
	bA1 = (tmpA & 0x02);
	bA2 = (tmpA & 0x04);

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
			if(bA0 && bA1 && bA2){
				   
				   set_PWM(0);	    //Speak makes no sound
				   break;
			   }
			
			else if(bA0 && !bA1 && !bA2){
				
				set_PWM(261.63);     //Set music note frequency to C_4
				break;
			}
			else if(!bA0 && bA1 && !bA2){
				
				set_PWM(293.66);	 //Set music note frequency to D_4
			}
			else if(bA0 == 0x00 &&
			bA1 == 0x00 &&
			bA2 == 0x01){
				
				set_PWM(329.63);		 //Set music note frequency to E_4
			}
			else{
				set_PWM(0);			 //Speak makes no sound
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

	state = Start;
	PWM_on();
	
	TimerSet(100);
	TimerOn();

	while(1){
		Tick();
		while(!TimerFlag){};
		TimerFlag = 0;
	}

}