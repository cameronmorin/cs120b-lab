#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>



//Temporary variable used to hold input

unsigned char tmpA = 0x00;

unsigned char tmpB = 0x00;

unsigned char bLeft; //Bit 0 used to determine if sequence is going right or left


enum States {Start, ST_B0, ST_B1, ST_B2, ST_wait, ST_pause} state;



void Tick(){

	

	tmpA = ~PINA & 0x01;

	switch(state){

		case Start:
			state = ST_B0;
			break;

		case ST_B0:
			if(!tmpA){
				state = ST_B1;
				bLeft = 0x01;
			} 
			else{
				state = ST_wait;	
			}
			break;

		case ST_B1:
			if(!tmpA){
				state = bLeft ? ST_B2 : ST_B0;
			}
			else{
				state = ST_wait;
			}
			break;

		case ST_B2:
			if(!tmpA){
				state = ST_B1;
				bLeft = 0x00;
			}
			else{
				state = ST_wait;
			}
			break;
			
		case ST_wait:
			if (tmpA){
				break;
			}
			state = ST_pause;
			break;
			
		case ST_pause:
			if (!tmpA){
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
			break;
	
		case ST_pause:
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
	
	TimerSet(300);
	TimerOn();

	while(1){
		Tick();
		while(!TimerFlag){};
			TimerFlag = 0;
		}

}