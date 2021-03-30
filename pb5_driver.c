/*
 * Lab5
 *
 * Created: 2/3/2020 12:05:29 PM
 * Author : John Hormis and Jeffrey Cheuk
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include "pb5_driver.h"

#define BTN_PB5_DDR DDRB
#define BTN_PB5_PIN PINB
#define PB5_num 5

void configure_pb5()
{
	BTN_PB5_DDR &=  ~(1<<PB5_num)  ;
	PORTB|= (1<<PB5_num);
}

_Bool pb5_is_pressed()
{
	return !(BTN_PB5_PIN & ( 1<<PB5_num ));
}

_Bool pb5_is_released()
{
	return (BTN_PB5_PIN & ( 1<<PB5_num ));
}
