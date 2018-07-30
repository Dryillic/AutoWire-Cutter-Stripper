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

volatile float dutyCycle;


class Servo {
	float angle;
	public:
	void Intialize();
	void MoveToAngle(float);
	};

void Servo::Intialize(){
	
	DDRD = (1 << PORTD6); //Sets PWM pin to output. We are using OCR0A
	
	TCCR0A = (1 << COM0A1)|(1 << WGM00)|(1<< WGM01); //Set register for fast PWM on compare.
	TIMSK0 = (1<< TOIE0); //resets timer on overflow
	
	OCR0A = (dutyCycle/100)*255; //sets duty cycle as fractional percentage of 255
	
	sei();	
	
	TCCR0B = (1 << CS02); //Set pre-scaler to 256 for around 120Hz (Freq accuracy not important) STARTS TIMER
}	

ISR(TIMER0_OVF_vect){
	OCR0A = (dutyCycle/100)*255; //sets duty cycle as fractional percentage of 255. Putting this in ISR allows recalculation of dutyCycle rapidly
}

void Servo::MoveToAngle(float angle){
	/* For 120Hz Signal 27% Duty Cycle amounts to an angle of 180deg
	/  9%  Duty Cycle amounts to an angle of 0deg
	/  Therefore, angle*((27-9)/180)+9 is the correct angle to duty cycle calculation
	*/
	dutyCycle = (angle*(18.0/180.0)+9.0); //STATIC VALUES HAVE TO BE FLOATS TO CALCULATE CORRECTLY! God damn.
}

#endif /* INCFILE1_H_ */