/*
 * project_junior_design.c
 *
 * Created: 4/24/2020 7:28:53 PM
 * Author : Jeffrey Cheuk and John Hormis
 */ 

#define F_CPU 20000000
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdfix.h>
#include <avr/interrupt.h>
#include <util/delay.h> 

#include "PID_turn_control.h"
#include "reflectance_sensors_3pi_drivers.h"
#include "timer_PWM_motors.h"

#define black 1
#define white 0 
#define middle 2
#define rightmost 4
#define leftmost 0
#define left 10
#define middleleft 1
#define middleright 3
#define right_straight 8
#define complete_180 6

uint8_t PID_control(uint8_t SPEED)
{	
	left_motor_control(FORWARD);
	right_motor_control(FORWARD);
	uint8_t sensor;
	accum i_error = 0;
	accum control = 0;
	accum sensor_avg = 0, error = 0, d_error = 0, last_error = 0;
	int16_t steer_amount;
	uint8_t max_failsafe_loop = 0;
	
	uint8_t turn_direction = 0;

	while(1)
	{
		sensor = read_reflectance_sensor();
		
		if( ((sensor & (1<<leftmost)) != white) && ((sensor & (1<<rightmost)) == white) ) 
		{
			turn_direction = left;
			break;
		}
		if( ((sensor & (1<<rightmost)) != white) && ((sensor & (1<<leftmost)) == white) ) 
		{
			
			max_failsafe_loop = 0;
			turn_direction = right_straight;
			while(max_failsafe_loop < 130)
			{
				sensor = read_reflectance_sensor();
				if( (sensor & (1<<leftmost)) != white)
				{
					turn_direction = left;
					break;
				}
				set_left_motor_duty_cycle(SPEED);
				set_right_motor_duty_cycle(SPEED);
				_delay_ms(1);
				max_failsafe_loop++;
			}
			break;
		}
		
		if(  ((sensor & (1<<middleleft)) == white) &&  ((sensor & (1<<middle)) == white) && ((sensor & (1<<middleright)) == white) )
		{
			turn_direction = complete_180;
			
			break;
		}
		
		sensor_avg = ( (3. * sensor_avg) + line_pos(sensor)) / 4.; 

		error = 0. - sensor_avg;
		d_error = error - last_error;
		last_error = error;
		i_error += error;
		control = 0.8*error + 9.5*d_error + 1.0/2048*i_error; 
		
		steer_amount = control * SPEED; 
		if(steer_amount >= SPEED) steer_amount = SPEED - 1;
		if(steer_amount <= -SPEED) steer_amount = -SPEED + 1; 
		
		set_left_motor_duty_cycle(SPEED + steer_amount);
		set_right_motor_duty_cycle(SPEED - steer_amount);
		
	}
	
	return turn_direction;
}

char left_bias_turn(uint8_t direction, uint8_t SPEED)
{
	uint8_t sensor = 0;
	_Bool end_180 = false;
	_Bool end_right = false;
	char saved_turn = ' ';
	
	if(direction == left)
	{
		saved_turn = 'L';
		set_left_motor_duty_cycle(SPEED-40); //sharp veer to the left
		set_right_motor_duty_cycle(SPEED+40);
		
		while(1)
		{
			sensor = read_reflectance_sensor();
			if( (sensor & (1<<middleleft)) == white)
				break;
		}
			
	}
	else if(direction == right_straight)
	{
		while(1)
		{
			sensor = read_reflectance_sensor();
			if( (sensor & (1<<rightmost)) == white)
			{
				if( ((sensor & (1<<middle)) == white) )
				{
					saved_turn = 'R';
					right_motor_control(BACKWARD);
					left_motor_control(FORWARD);
					set_right_motor_duty_cycle(SPEED);
					set_left_motor_duty_cycle(SPEED);
					while(end_right == false)
					{
						sensor = read_reflectance_sensor();
						if( (sensor & (1<<rightmost)) )
						{
							while(1)
							{
								sensor = read_reflectance_sensor();
								if( ((sensor & (1<<rightmost)) == white ) )
								{
									end_right = true;
									break;
								}
							}
							
						}
					}
					right_motor_control(BRAKE);
					left_motor_control(BRAKE);
					right_motor_control(FORWARD);
					left_motor_control(FORWARD);
					break;
				}
				else  //straight turn
				{
					
					saved_turn = 'S';
					set_left_motor_duty_cycle(SPEED);
					set_right_motor_duty_cycle(SPEED);
					_delay_ms(200);
					break;
				}
				
			}
			set_left_motor_duty_cycle(SPEED);
			set_right_motor_duty_cycle(SPEED);
		}
		
	}
	else if(direction == complete_180)
	{
		saved_turn = 'U';
		left_motor_control(BRAKE);
		right_motor_control(BRAKE);
		left_motor_control(FORWARD);
		right_motor_control(BACKWARD); //turn in right direction (clockwise)
		
		set_left_motor_duty_cycle(SPEED);
		set_right_motor_duty_cycle(SPEED);
		while(end_180 == false) //turn until sensor 4 hits black, then white
		{
			sensor = read_reflectance_sensor();
			if( (sensor & (1<<rightmost)) )
			{
				while(1)
				{
					sensor = read_reflectance_sensor();
					if( (sensor & (1<<rightmost)) == white)
					{
						end_180 = true;
						break;
					}
				}
			}
		}
		
		left_motor_control(BRAKE);
		right_motor_control(BRAKE);
		left_motor_control(FORWARD);
		right_motor_control(FORWARD);
	}
	
	return saved_turn;
}


