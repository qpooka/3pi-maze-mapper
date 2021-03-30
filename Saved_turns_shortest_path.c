/*
 * project_junior_design.c
 *
 * Created: 4/24/2020 7:28:53 PM
 * Author : Jeffrey Cheuk and John Hormis
 */ 

#define F_CPU 20000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "Saved_turns_shortest_path.h"
#include "reflectance_sensors_3pi_drivers.h"
#include "timer_PWM_motors.h"

#define black 1 
#define white 0 
#define middle 2
#define rightmost 4
#define leftmost 0
#define middleleft 1
#define middleright 3

void turn_simplification(char *saved_turn_array, uint8_t array_index)
{
	if( (saved_turn_array[array_index+1] == 'S') && (saved_turn_array[array_index-1] == 'S') )
	{
		saved_turn_array[array_index-1] = 'U';
	}
	else if( (saved_turn_array[array_index+1] == 'L') && (saved_turn_array[array_index-1] == 'L') )
	{
		saved_turn_array[array_index-1] = 'S';
	}
	else if( ((saved_turn_array[array_index+1] == 'S') && (saved_turn_array[array_index-1] == 'L')) ||  ((saved_turn_array[array_index+1] == 'L') && (saved_turn_array[array_index-1] == 'S')) )
	{
		saved_turn_array[array_index-1] = 'R';
	}
	else if( ((saved_turn_array[array_index+1] == 'L') && (saved_turn_array[array_index-1] == 'R')) ||  ((saved_turn_array[array_index+1] == 'R') && (saved_turn_array[array_index-1] == 'L')) )
	{
		saved_turn_array[array_index-1] = 'U';
	}
	saved_turn_array[array_index] = 'E';
	saved_turn_array[array_index+1] = 'E';
}

void turn_based_on_memory(char *saved_array, uint8_t SPEED, uint8_t index_num)
{
	
	left_motor_control(FORWARD);
	right_motor_control(FORWARD);	
	uint8_t sensor;
	
	if(saved_array[index_num] == 'L') 
	{
		set_left_motor_duty_cycle(SPEED-40); //sharp veer to the left
		set_right_motor_duty_cycle(SPEED+40);
		
		while(1)
		{
			sensor = read_reflectance_sensor();
			if( (sensor & (1<<middleleft)) == white)
				break;
		}
	}
	else if(saved_array[index_num] == 'R') 
	{
		set_left_motor_duty_cycle(SPEED+40); //sharp veer to the right
		set_right_motor_duty_cycle(SPEED-40);
		while(1)
		{
			sensor = read_reflectance_sensor();
			if( ((sensor & (1<<middleright)) == white)  )
				break;
		}
	}
	else if(saved_array[index_num] == 'S')
	{
		set_left_motor_duty_cycle(SPEED);
		set_right_motor_duty_cycle(SPEED);
		_delay_ms(300);
	}
	
}