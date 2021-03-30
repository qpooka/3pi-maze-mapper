/*
 * lab4.c
 *
 * Created: 2/17/2020 11:56:49 AM
 * Author : John Hormis and Jeffrey Cheuk
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <stdfix.h>

void configure_reflectance_sensor();
uint8_t read_reflectance_sensor();
short accum line_pos(uint8_t value);

