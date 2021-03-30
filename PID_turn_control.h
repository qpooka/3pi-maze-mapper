/*
 * project_junior_design.c
 *
 * Created: 4/24/2020 7:28:53 PM
 * Author : Jeffrey Cheuk and John Hormis
 */ 

#include <avr/io.h>
#include <stdint.h>

uint8_t PID_control(uint8_t SPEED);
char left_bias_turn(uint8_t direction, uint8_t SPEED);