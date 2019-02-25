/*	Partner(s) Name & E-mail: Cameron Morin cmori007@ucr.edu
						      Raudel Blazquez Munoz rblaz001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab 10  Exercise 3
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <bit.h>
#include <timer.h>
#include <stdio.h>
#include "io.c"

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
    unsigned long int c;
    while(1){
        c = a%b;
        if(c==0){return b;}
        a = b;
		b = c;
    }
    return 0;
}
//--------End find GCD function ----------------------------------------------

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
    /*Tasks should have members that include: state, period,
        a measurement of elapsed time, and a function pointer.*/
    signed char state; //Task's current state
    unsigned long int period; //Task period
    unsigned long int elapsedTime; //Time elapsed since last task tick
    int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------

//--------Shared Variables----------------------------------------------------
unsigned char sm1_output = 0x00;
//unsigned char SM2_output = 0x00;
//unsigned char SM3_output = 0x00;
//unsigned char pause = 0;

//--------End Shared Variables------------------------------------------------

//--------User defined FSMs---------------------------------------------------
//Enumeration of states.
enum SM1_States { SM1_Start, SM1_running};

// Monitors button connected to PA0. 
// When button is pressed, shared variable "pause" is toggled.
int SMTick1(int state) {
 

    //State machine transitions
    switch (state) {
		case SM1_Start:
			state = SM1_running;
			break;
		
		case SM1_running:
			break;
		
		default:
			break;
    }

    //State machine actions
	
    switch (state) {
        case SM1_Start:
			break;
        
        case SM1_running:

    			PORTC = 0xEF; // Enable col 4 with 0, disable others with 1�s
    			asm("nop"); // add a delay to allow PORTC to stabilize before checking
    			if (GetBit(PINC,0)==0) { sm1_output = '1'; }
    			if (GetBit(PINC,1)==0) { sm1_output = '4'; }
    			if (GetBit(PINC,2)==0) { sm1_output = '7'; }
    			if (GetBit(PINC,3)==0) { sm1_output = '*'; }

    			// Check keys in col 2
    			PORTC = 0xDF; // Enable col 5 with 0, disable others with 1�s
    			asm("nop"); // add a delay to allow PORTC to stabilize before checking
    			if (GetBit(PINC,0)==0) { sm1_output = '2'; }
				if (GetBit(PINC,1)==0) { sm1_output = '5'; }
				if (GetBit(PINC,2)==0) { sm1_output = '8'; }
				if (GetBit(PINC,3)==0) { sm1_output = '0'; }
    			// ... *****FINISH*****

    			// Check keys in col 3
    			PORTC = 0xBF; // Enable col 6 with 0, disable others with 1�s
    			asm("nop"); // add a delay to allow PORTC to stabilize before checking
				if (GetBit(PINC,0)==0) { sm1_output = '3'; }
				if (GetBit(PINC,1)==0) { sm1_output = '6'; }
				if (GetBit(PINC,2)==0) { sm1_output = '9'; }
				if (GetBit(PINC,3)==0) { sm1_output = '#'; }
    			// ... *****FINISH*****

    			// Check keys in col 4
    			// ... *****FINISH*****
				PORTC = 0x7F; // Enable col 6 with 0, disable others with 1�s
				asm("nop"); // add a delay to allow PORTC to stabilize before checking
				if (GetBit(PINC,0)==0) { sm1_output = 'A'; }
				if (GetBit(PINC,1)==0) { sm1_output = 'B'; }
				if (GetBit(PINC,2)==0) { sm1_output = 'C'; }
				if (GetBit(PINC,3)==0) { sm1_output = 'D'; }

			break;
        
        default:
			break;
    }

    return state;
}

//Enumeration of states.
enum SM2_States { SM2_Start, SM2_running };

// If paused: Do NOT toggle LED connected to PB0
// If unpaused: toggle LED connected to PB0
int SMTick2(int state) {

    //State machine transitions
    switch (state) {
		case SM2_Start:
			state = SM2_running;
		case SM2_running:
			break;
		default:
			break;
	}

	switch (state) {
		case SM2_Start:
			break;
		case SM2_running:
			switch (sm1_output) {
    			case '\0': break; // All 5 LEDs on
    			case '1': LCD_DisplayString(1, "1"); break; // hex equivalent
    			case '2': LCD_DisplayString(1, "2"); break;
				case '3': LCD_DisplayString(1, "3"); break;
				case '4': LCD_DisplayString(1, "4"); break;
				case '5': LCD_DisplayString(1, "5"); break;
				case '6': LCD_DisplayString(1, "6"); break;
				case '7': LCD_DisplayString(1, "7"); break;
				case '8': LCD_DisplayString(1, "8"); break;
				case '9': LCD_DisplayString(1, "9"); break;
				case 'A': LCD_DisplayString(1, "A"); break;
				case 'B': LCD_DisplayString(1, "B"); break;
				case 'C': LCD_DisplayString(1, "C"); break;
    			case 'D': LCD_DisplayString(1, "D"); break;
    			case '*': LCD_DisplayString(1, "*"); break;
    			case '0': LCD_DisplayString(1, "0"); break;
    			case '#': LCD_DisplayString(1, "#"); break;
    			default: LCD_DisplayString(1, "!"); break;
				break;
		    }
		default:
			break;
	}

    return state;
}

/*

//Enumeration of states.
enum SM3_States { SM3_Start, SM3_running };

// If paused: Do NOT toggle LED connected to PB1
// If unpaused: toggle LED connected to PB1

int SMTick3(int state) {
    //State machine transitions

	static const unsigned char PHRASE[] = {'C', 'S', '1', '2', '0', 'B', ' ', 'I', 'S', ' ', 'L', 'E', 'G', 'E', 'N', 'D', '.', '.', '.', ' ', 'W', 'A', 'I', 'T', ' ', 'F', 'O', 'R', ' ', 'I', 'T', ' ', 'D', 'A', 'I', 'R', 'Y', ' '};
	static unsigned char cnt = 0;
	static unsigned char i = 0;
	static const unsigned char PHRASE_SIZE = sizeof(PHRASE)/sizeof(PHRASE[0]);

    switch (state) {
		case SM3_Start:
			state = SM3_running;
			break;
		case SM3_running:
			break;
		default:
			break;
	}

	    switch (state) {
		case SM3_Start:
			break;
		case SM3_running:
			if(cnt < (PHRASE_SIZE + 15)){
				for(unsigned char j=0; j < 16; j++){
					LCD_Cursor(j+1);
					LCD_WriteData(PHRASE[ (i+j) % (PHRASE_SIZE)]);
				}
				i++;
				cnt++;
			}
			else{
				cnt = 0;
				i = 1;
			}
			break;
		default:
			break;
	}

	return state;
}

/*
//Enumeration of states.
enum SM4_States { SM4_display };

// Combine blinking LED outputs from SM2 and SM3, and output on PORTB
int SMTick4(int state) {
    // Local Variables

    unsigned char output;

    //State machine transitions
    switch (state) {
		case SM4_display:    
		break;

    default:		
		state = SM4_display;
        break;
    }

    //State machine actions
    switch(state) {
		case SM4_display:    
		output = SM2_output | SM3_output; // write shared outputs
                                    // to local variables
		break;

    default:        
		break;
    }

    PORTB = output;    // Write combined, shared output variables to PORTB

    return state;
}
*/

// --------END User defined FSMs-----------------------------------------------

// Implement scheduler code from PES.
int main()
{
// Set Data Direction Registers
// Buttons PORTA[0-7], set AVR PORTA to pull down logic
DDRA = 0xFF; PORTA = 0x00;
DDRC = 0xF0; PORTC = 0x0F;
DDRB = 0xFF; PORTB = 0x00;
DDRD = 0xFF; PORTD = 0x00;


// Period for the tasks
unsigned long int SMTick1_calc = 100;
unsigned long int SMTick2_calc = 100;
//unsigned long int SMTick3_calc = 500;
//unsigned long int SMTick4_calc = 10;

//Calculating GCD
unsigned long int tmpGCD = 1;
tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
//tmpGCD = findGCD(tmpGCD, SMTick3_calc);
//tmpGCD = findGCD(tmpGCD, SMTick4_calc);

//Greatest common divisor for all tasks or smallest time unit for tasks.
unsigned long int GCD = tmpGCD;

//Recalculate GCD periods for scheduler
unsigned long int SMTick1_period = SMTick1_calc/GCD;
unsigned long int SMTick2_period = SMTick2_calc/GCD;
//unsigned long int SMTick3_period = SMTick3_calc/GCD;
//unsigned long int SMTick4_period = SMTick4_calc/GCD;

//Declare an array of tasks 
static task task1, task2; //, task3; //, task4;
task *tasks[] = { &task1, &task2 }; // , &task3}; //, &task4 };
const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

// Task 1
task1.state = SM1_Start;//Task initial state.
task1.period = SMTick1_period;//Task Period.
task1.elapsedTime = SMTick1_period;//Task current elapsed time.
task1.TickFct = &SMTick1;//Function pointer for the tick.

// Task 2
task2.state = SM2_Start;//Task initial state.
task2.period = SMTick2_period;//Task Period.
task2.elapsedTime = SMTick2_period;//Task current elapsed time.
task2.TickFct = &SMTick2;//Function pointer for the tick.

/*
// Task 3
task3.state = SM3_Start;//Task initial state.
task3.period = SMTick3_period;//Task Period.
task3.elapsedTime = SMTick3_period; // Task current elapsed time.
task3.TickFct = &SMTick3; // Function pointer for the tick.

/*
// Task 4
task4.state = -1;//Task initial state.
task4.period = SMTick4_period;//Task Period.
task4.elapsedTime = SMTick4_period; // Task current elasped time.
task4.TickFct = &SMTick4; // Function pointer for the tick.

*/

// Set the timer and turn it on
TimerSet(GCD);
TimerOn();
LCD_init();

unsigned short i; // Scheduler for-loop iterator

while(1) {
    // Scheduler code
    for ( i = 0; i < numTasks; i++ ) {
        // Task is ready to tick
        if ( tasks[i]->elapsedTime == tasks[i]->period ) {
            // Setting next state for task
            tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
            // Reset the elapsed time for next tick.
            tasks[i]->elapsedTime = 0;
        }
        tasks[i]->elapsedTime += 1;
    }
    while(!TimerFlag);
    TimerFlag = 0;
}

return 0;
}
