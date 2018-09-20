/*
 * Servo.h
 *
 * Created: 5/22/2018 6:34:06 PM
 *  Author: dnatov
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include <avr/io.h>
#include <avr/interrupt.h>

float dutyCycle;


class Servo {
	float angle;
	bool power;
	public:
	void Intialize();
	void Enable(bool);
	void MoveToAngle(float);
	};

void Servo::Intialize(){
	
	DDRD |= (1 << PORTD6) ; //Sets PWM pin to output. We are using OC0A
	
	TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1<< WGM01); //Set register for fast PWM on compare.
	TIMSK0 = (1 << TOIE0); //resets timer on overflow
	
	OCR0A = 35; //sets duty cycle as fractional percentage of 255
	this->Enable(true);
}	

void Servo::Enable(bool power) {
	if (power) 
	{
		TCCR0B = (1 << CS02); 
	} //Starts Timer
	else 
	{ 
		TCCR0B = (0 << CS02); 
		PORTD |= (0 << PORTD6);
	} //Stops Timer	
}

void Servo::MoveToAngle(float angle){
	/* For 120Hz Signal 27% Duty Cycle amounts to an angle of 180deg
	/  9%  Duty Cycle amounts to an angle of 0deg
	/  Therefore, angle*((27-9)/180)+9 is the correct angle to duty cycle calculation
	*/
	dutyCycle = (angle*(18.0/180.0)+9.0); //STATIC VALUES HAVE TO BE FLOATS TO CALCULATE CORRECTLY! God damn.
	OCR0A = (dutyCycle/100)*255; //Previously was in ISR but that would take more CPU usage. It's easy just to recalculate it here.
}

EMPTY_INTERRUPT(TIMER0_OVF_vect); //Uses PWM without the need for an interrupt to do anything

#endif /* INCFILE1_H_ */