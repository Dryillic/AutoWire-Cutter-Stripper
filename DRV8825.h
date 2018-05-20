#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>

volatile long count=0;

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
	DDRD = 0b0001110; //set ports 2,3,4 to output
	TCCR0A = (1 << WGM01); //Set CTC bit
	OCR0A = 48; //Set tick amount to 48 for 10.5kHz Square Wave, (Toggle output)
	TIMSK0 = (1 << OCIE0A); //Set ISR interrupt
	
	sei(); //Set I bit. aka set external interrupt
	
	TCCR0B = (1 << CS01); //Timer pre-scaler set to div 8
}

ISR(TIMER0_COMPA_vect) //Runs every 10.5khz
{
	count++; //take care not to overflow. Overflows in about 2 days.
	PORTD ^= (1 << PORTD4); //toggle port d4 for square wave
	if (count > 2000000000) {
		count = 0;
	}
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
	long countcurrent = count;
	this->Direction(directionCont);
	while (count < (countcurrent+stepCount+1)) { //+1 to account for setup time when the enable pin goes high
		this->Enable(true);
	}
	this->Enable(false);
}


