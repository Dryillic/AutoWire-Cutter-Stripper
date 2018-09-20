#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>

volatile long countStepper=0;

class DRV8825 {
	bool enableCont, directionCont;
	bool dir, enbl;
	int stepCount;
	public:
	void Enable(bool);
	void Direction(bool);
	void Initialize();
	void Runtostep(int, bool);
	bool directionRot() {return dir;}
	bool enablestatus() {return enbl;}
};

void DRV8825::Initialize()
{
	PORTD = (1 << PORTD2);
	DDRD |= (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4); //set ports 2,3,4 to output (6 is output for the servo)
	TCCR1B |= (1 << WGM12); //Set CTC bit timer 1
	OCR1A = 48; //Set tick amount to 48 for 10.5kHz Square Wave, (Toggle output)
	TIMSK1 = (1 << OCIE1A); //Set ISR interrupt
	
	/*note: the timer actually has a frequency of 10.5kHz * 2 = ~21kHz
	The ISR is a port toggle so this frequency has to be double for both rising and falling edges of square wave*/
	
	//SET IN MAIN INSTEAD
	//sei(); //Set I bit. aka set external interrupt
}

void DRV8825::Enable (bool enableCont) { //Enable Stepper Controller uses PORTD2
	if (enableCont) {
		enbl = true;
		PORTD = (0 << PORTD2);
	}
	else {
		enbl = false;
		PORTD = (1 << PORTD2);
	}	
}

void DRV8825::Direction(bool directionCont) { //Set Stepper Direction uses PORTD3
	if (directionCont) { //true is counter-clockwise, the direction the wire normally feeds
	dir = true;
	PORTD = (1 << PORTD3);
	}
	else { //false is clockwise
	dir = false;
	PORTD = (0 << PORTD3);
	}	
}

void DRV8825::Runtostep(int stepCount, bool directionCont) { //200*32 microsteps per revolution meaning 6400 is one revolution
	UCSR0B = (0 << TXEN0) | (0 << RXEN0); /* Serial Transmit and Recieve Disable */
	TCCR1B |= (1 << CS11); //Timer pre-scaler set to div 8 START TIMER
	
	if (countStepper > 2000000000) {
		countStepper = 0;
	}
	long countcurrent = countStepper;
	this->Direction(directionCont);
	while (countStepper < (countcurrent+stepCount+1)) { //+1 to account for setup time when the enable pin goes high
		this->Enable(true);
	}
	this->Enable(false);
	
	TCCR1B |= (0 << CS11); //Timer pre-scaler set to off STOP TIMER
	UCSR0B = (1 << TXEN0) | (1 << RXEN0); /* Serial Transmit and Recieve Enable */
}

ISR(TIMER1_COMPA_vect) //Runs every 10.5khz
{
	countStepper++; //take care not to overflow. Overflows in about 2 days.
	PORTD ^= (1 << PORTD4); //toggle port d4 for square wave
}
