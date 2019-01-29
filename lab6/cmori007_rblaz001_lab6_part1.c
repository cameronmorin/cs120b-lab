#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>



//Temporary variable used to hold input

unsigned char tmpA;

unsigned char tmpB;


enum States {Start, one, two, three} state;



void Tick(){

	

	tmpA = ~PINA & 0x01;

	

	tmpB = PORTB;

	

	switch(state){

		case Start:
			state = one;
			break;

		case one:
			state = two;
			break;

		case two:
			state = three;
			break;

		case three:
			state = one;
			break;

		default:
			break;

	}
	
	switch(state){

		case Start:
			break;

		case one:
			tmpB = 0x01;
			break;

		case two:
			tmpB = 0x02;
			break;

		case three:
			tmpB = 0x04;
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

	state = Start;
	
	TimerSet(1000);
	TimerOn();

	while(1){
		Tick();
		while(!TimerFlag){};
			TimerFlag = 0;
		}

}