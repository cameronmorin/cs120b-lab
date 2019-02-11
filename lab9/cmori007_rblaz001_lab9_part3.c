/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 9  Exercise 3
 *	Exercise Description: One button 5-second melody, plays to completion when pressed.
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

//C E F E C E G E C E F E D C

const unsigned char MAX = 13;

double noteArray[14] = {261.63, 329.63, 349.23, 329.63, 261.63, 329.63, 392.00, 329.63, 261.63, 329.63, 349.23, 329.63, 293.66, 261.63};
const double pauseArray[14] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
const double playArray[14] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0};

unsigned char cnt = 0;
unsigned char noteCnt = 0;
unsigned char bOn = 0;

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound2
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
	TCCR0A = (1 << COM0A0) | (1 << WGM00);
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

void toggle(){
	bOn = bOn ? 0 : 1;
	
	if(bOn){
		set_PWM(noteArray[cnt]);
	}
	else{
		set_PWM(0);
	}
}

enum States {Start, wait, play, pause, buffer} state;



void Tick(){
	
	tmpA = ~PINA;
	bA0 = (tmpA & 0x01);
	bA1 = (tmpA & 0x02);
	bA2 = (tmpA & 0x04);

	switch(state){
		
		case Start:
			state = wait;
			break;

		case wait:
			if(bA0){
				state = play;
				bOn = 1;
			}
			break;
			
		case play:
			if(cnt < playArray[noteCnt]){
				cnt++;	
			}
			else {
				cnt = 0;
				state = pause;
			}
			break;
			
		case pause:
			if(cnt < pauseArray[noteCnt]){
				cnt++;
			}
			else {
				cnt = 0;
				
				if(noteCnt < MAX){
					state = play;
					noteCnt++;
				}
				else{
					state = buffer;
				}
			}
			break;
		
		case buffer:
			if(!bA0){
				state = wait;
				noteCnt = 0;
			}
			break;
		
		default:
			break;
	}
	
	switch(state){
		
		case Start:
			break;

		case wait:
			break;
			
		case play:
			set_PWM(noteArray[noteCnt]);
			break;
			
		case pause:
			set_PWM(0);
			break;
		
		case buffer:
			break;
		
		default:
			break;
	}


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