/*
 * Lab5
 *
 * Created: 2/3/2020 12:05:29 PM
 * Author : John Hormis and Jeffrey Cheuk
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include "pb4_driver.h"

#define BTN_PB4_DDR DDRB
#define BTN_PB4_PIN PINB
#define PB4_num 4

void configure_pb4()
{
	BTN_PB4_DDR &=  ~(1<<PB4_num)  ;
	PORTB|= (1<<PB4_num);
}

_Bool pb4_is_pressed()
{
	return !(BTN_PB4_PIN & ( 1<<PB4_num ));
}

_Bool pb4_is_released()
{
	return (BTN_PB4_PIN & ( 1<<PB4_num ));
}
