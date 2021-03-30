/*
 * project_junior_design.c
 *
 * Created: 4/24/2020 7:28:53 PM
 * Author : Jeffrey Cheuk and John Hormis
 */ 

#include <avr/io.h>
#include <stdint.h>

void turn_simplification(char *saved_turn_array, uint8_t array_index); 
void turn_based_on_memory(char *saved_array, uint8_t SPEED, uint8_t index_num);