/*
 * lab6
 *
 * Created: 3/9/2020 11:55:37 AM
 * Author : John Hormis and Jeffrey Cheuk
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "timer_PWM_motors.h"

#define left_motor_DDR DDRD
#define left_motor_PORT PORTD
#define left_b 5
#define left_a 6
#define right_motor_DDRB DDRB
#define right_motor_DDRD DDRD
#define right_motor_PORTD PORTD
#define right_motor_PORTB PORTB
#define right_a 3
#define right_b 3

void initialize_left_motor()
{
	left_motor_DDR |= (1<<left_a);
	left_motor_DDR |= (1<<left_b);
	TCCR0A = 0x03;
	TCCR0B = 0x02;
	left_motor_control(BRAKE);
}

void initialize_right_motor()
{
	right_motor_DDRD |= (1<<right_a);
	right_motor_DDRB |= (1<<right_b);
	TCCR2A = 0x03;
	TCCR2B = 0x02;
	right_motor_control(BRAKE);
}

void left_motor_control(enum MOTOR_CONTROL left_control)
{
	switch(left_control)
	{
		case COAST:
			TCCR0A &= ~0xF0;
			left_motor_PORT &= ~(1<<left_a);
			left_motor_PORT &= ~(1<<left_b);
			break;
		
		case BACKWARD:
			TCCR0A &= ~0xF0;
			left_motor_PORT |= (1<<left_a);
			left_motor_PORT |= (1<<left_b);
			TCCR0A |= ( (1<<COM0A0) | (1<<COM0A1) );
			break;
		
		case FORWARD:
			TCCR0A &= ~0xF0;
			PORTD |= (1<<5);
			PORTD |= (1<<6);
			TCCR0A |= ( (1<<COM0B0) | (1<<COM0B1) );
			break;
			
		default:
		case BRAKE:
			TCCR0A &= ~0xF0;
			left_motor_PORT |= (1<<left_a);
			left_motor_PORT |= (1<<left_b);
			break;
	}	
}

void right_motor_control(enum MOTOR_CONTROL right_control)
{
	switch(right_control)
	{
		case COAST:
			TCCR2A &= ~0xF0;
			right_motor_PORTD &= ~(1<<right_a);
			right_motor_PORTB &= ~(1<<right_b);
			break;
		
		case BACKWARD:
			TCCR2A &= ~0xF0;
			right_motor_PORTD |= (1<<right_a);
			right_motor_PORTB |= (1<<right_b);
			TCCR2A |= ( (1<<COM2A0) | (1<<COM2A1) ); //wait... doesn't this set them to 0?
			break;
		
		case FORWARD:
			TCCR2A &= ~0xF0;
			right_motor_PORTD |= (1<<right_a);
			right_motor_PORTB |= (1<<right_b);
			TCCR2A |= ( (1<<COM2B0) | (1<<COM2B1) );
			break;
			
		default:
		case BRAKE:
			TCCR2A &= ~0xF0;
			right_motor_PORTD |= (1<<right_a);
			right_motor_PORTB |= (1<<right_b);
			break;
	}
}

void set_left_motor_duty_cycle(uint8_t left_duty_cycle)
{
	OCR0A = left_duty_cycle;
	OCR0B = left_duty_cycle;
}

void set_right_motor_duty_cycle(uint8_t right_duty_cycle)
{
	OCR2A = right_duty_cycle;
	OCR2B = right_duty_cycle;
}