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
#include <stdfix.h>
#include "reflectance_sensors_3pi_drivers.h"

#define SENSOR_DDR DDRC
#define SENSORPORT PORTC
#define SENSORPIN PINC

#define black 1
#define white 0
#define middle 2
#define rightmost 4
#define leftmost 0
#define middleleft 1
#define middleright 3

void configure_reflectance_sensor()
{
	SENSOR_DDR |= (1<<leftmost) | (1<<middleleft) | (1<<middle) | (1<<middleright) | (1<<rightmost); 
	SENSORPORT &= ~(1<<leftmost) & ~(1<<middleleft) & ~(1<<middle) & ~(1<<middleright) & ~(1<<rightmost); 
	SENSORPORT |= (1<<leftmost) | (1<<middleleft) | (1<<middle) | (1<<middleright) | (1<<rightmost);
	_delay_ms(1);
	SENSOR_DDR &= ~(1<<leftmost) & ~(1<<middleleft) & ~(1<<middle) & ~(1<<middleright) & ~(1<<rightmost);
}

uint8_t read_reflectance_sensor()
{
	uint8_t return_5b = 0x00;
	if ( (SENSORPIN & (1<<leftmost)) )    return_5b |= (1<<0);
	if ( (SENSORPIN & (1<<middleleft)) )  return_5b |= (1<<1);
	if ( (SENSORPIN & (1<<middle)) )      return_5b |= (1<<2);
	if ( (SENSORPIN & (1<<middleright)) ) return_5b |= (1<<3);
	if ( (SENSORPIN & (1<<rightmost)) )   return_5b |= (1<<4);
	
	return return_5b;
}

short accum line_pos(uint8_t value)
{
	short accum retval = 0;
	if( (value & (1<<middleleft)) == white)  retval -=0.75;
	if( (value & (1<<middleright)) == white) retval +=0.75;
	
	return retval;
}