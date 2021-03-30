/*
 * Lab5
 *
 * Created: 2/3/2020 12:05:29 PM
 * Author : John Hormis and Jeffrey Cheuk
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include "pb1_driver.h"

#define BTN_PB1_DDR DDRB
#define BTN_PB1_PIN PINB
#define PB1_num 1

void configure_pb1()
{
	BTN_PB1_DDR &=  ~(1<<PB1_num)  ;
	PORTB|= (1<<1);
}

_Bool pb1_is_pressed()
{
	return !(BTN_PB1_PIN & ( 1<<PB1_num ));
}

_Bool pb1_is_released()
{
	return (BTN_PB1_PIN & ( 1<<PB1_num ));
}
