/*
 * ServoLibraryTest.cpp
 *
 * Created: 5/22/2018 6:33:13 PM
 * Author : dnatov
 */ 

#define F_CPU 8000000UL /* 8Mhz clock rate */

#include <avr/io.h>
#include <util/delay.h>
#include "Servo.h"

#define BAUD 2400
#define USART_BAUDRATE 2400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#define display_On 0x16 /*  Parralax cmd: Display on with no cursor and no blink */
#define first_position 0x80 /* Parralax cmd: Move cursor to line 0 position 0 */
#define form_feed 0x0C /* Parralax cmd: Clear all text, move to line 0 pos 0 */
#define line_feed 0x0A /* Parralax cmd: Cursor moved to next line */
#define backlight_on 0x11 /* Parralax cmd: LCD Backlight on */
#define backlight_off 0x12 /* Parralax cmd: LCD Backlight off */


int main(void)
{
	Servo MainServo;
	MainServo.Intialize();
    while (1) 
    {
		_delay_ms(2000);
		MainServo.MoveToAngle(0);
		_delay_ms(2000);
		MainServo.MoveToAngle(45);
		_delay_ms(2000);
		MainServo.MoveToAngle(90);
		_delay_ms(2000);
		MainServo.MoveToAngle(135);
		_delay_ms(2000);
		MainServo.MoveToAngle(180);
		_delay_ms(2000);
    }
}

