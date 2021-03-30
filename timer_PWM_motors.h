/*
 * lab6
 *
 * Created: 3/9/2020 11:55:37 AM
 * Author : John Hormis and Jeffrey Cheuk
 */ 

#include <avr/io.h>
#include <stdint.h>

enum MOTOR_CONTROL
{
	COAST, BACKWARD, FORWARD, BRAKE
};

void initialize_left_motor();
void initialize_right_motor();
void left_motor_control(enum MOTOR_CONTROL left_control);
void right_motor_control(enum MOTOR_CONTROL right_control);
void set_left_motor_duty_cycle(uint8_t left_duty_cycle);
void set_right_motor_duty_cycle(uint8_t right_duty_cycle);
