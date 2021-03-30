/*
 * project_junior_design.c
 *
 * Created: 4/24/2020 7:28:53 PM
 * Author : Jeffrey Cheuk and John Hormis
 */ 

#define F_CPU 20000000
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdfix.h>
#include <util/delay.h>

#include "timer_PWM_motors.h"
#include "reflectance_sensors_3pi_drivers.h"
#include "pb1_driver.h"
#include "pb4_driver.h"
#include "pb5_driver.h"
#include "PID_turn_control.h"
#include "Saved_turns_shortest_path.h"

#define A_button 1
#define B_button 4
#define C_button 5

#define complete_180 6

_Bool PB1_Pressed = false;
_Bool PB2_Pressed = false;
_Bool PB5_Pressed = false;

int main(void)
{
	initialize_left_motor();
	initialize_right_motor();
	configure_reflectance_sensor();
	configure_pb1();
	configure_pb4();
	configure_pb5();
	
	cli();
	PCMSK0 |= ( (1<<A_button) | (1<<B_button) | (1<<C_button) );
	PCICR |= (1<<PCIE0);
	sei();
	
	uint8_t max_number_of_U_turns_including_end_goal = 0;
	uint8_t current_U_turn_number = 0;
	right_motor_control(FORWARD);
	left_motor_control(FORWARD);	
	
	while(1)
	{
		
		if(pb1_is_pressed() == 1) 
		{
			PB1_Pressed = false;
			_delay_ms(2000);
			break;
		}
		if(pb4_is_pressed() == 1) 
		{
			PB2_Pressed = false;
			max_number_of_U_turns_including_end_goal++;
			_delay_ms(300);
		}
		if(PB5_Pressed)
		{ 
			PB5_Pressed = false;
			max_number_of_U_turns_including_end_goal--;
			_delay_ms(300);
		}
	}
	
	_Bool memory_mode = false;
	uint8_t number_of_Us = 0;
	uint8_t SPEED = 44; //max is 255
	uint8_t PID_break_turn;
	char saved_turn_array[50] = "";
	uint8_t turn_number = 0;
	uint8_t array_index = 0;
	uint8_t index = 0;
	
	while(1)	//main loop
	{
		PID_break_turn = PID_control(SPEED);
		
		if(memory_mode == false)
		{
			saved_turn_array[turn_number] = left_bias_turn(PID_break_turn, SPEED);
			turn_number++;
		}
		else
		{
			turn_based_on_memory(&saved_turn_array[0], SPEED, index);
			index--;
		}

		if(PID_break_turn == complete_180) 
			current_U_turn_number++;

		if(current_U_turn_number == max_number_of_U_turns_including_end_goal)
		{
			
			left_motor_control(BRAKE);
			right_motor_control(BRAKE);
			turn_number--;
			saved_turn_array[turn_number] = '\0';
			array_index = turn_number-1; //don't include U-turn done at end goal	
			while(1)
			{		
				if(array_index == 0)//triggers every loop through array to shift data and check if there are any 'U'(U-turns) left
				{
					array_index = 0;
					number_of_Us = 0;
					index = 0;
					
					while(saved_turn_array[array_index] != '\0')
					{
						if(saved_turn_array[array_index] == 'U')
						{
							number_of_Us++;
						}
						
						if(saved_turn_array[array_index] == 'E') //shift right 2 places at a time
						{
							index = array_index;
							while( saved_turn_array[index] != '\0' )
							{
								saved_turn_array[index] = saved_turn_array[index+2];
								index++;
							}
						}
						array_index++;
					}
					if(number_of_Us == 0)
					{
						memory_mode = true;
						break;
					}
					array_index--;
				}
				
				if(saved_turn_array[array_index] == 'U')
				{
					turn_simplification(&saved_turn_array[0], array_index); 
					//function simplifies and saves new char into array[array_index-1], while also filling array_index and array_index+1 with 'E' (empty)
					if(array_index > 2)
						array_index--;
				}
					array_index--;
			}
			
			index = 0;
			while(saved_turn_array[index] != '\0') 
			{
				if(saved_turn_array[index] == 'L')
					saved_turn_array[index] = 'R';
				else if(saved_turn_array[index] == 'R')
					saved_turn_array[index] = 'L';	
				index++;
			}
			index--;
			
		}
		
	}


	return 0;
}

ISR(PCINT0_vect)
{
	if( pb1_is_pressed() ) PB1_Pressed = true;
	if( pb4_is_pressed() ) PB2_Pressed = true;
	if( pb5_is_pressed() ) PB5_Pressed = true;
}


