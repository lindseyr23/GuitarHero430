/*
 * serial_com.h
 *
 *  Created on: Mar 27, 2024
 *      Author: Natalia Mendiola
 */

#ifndef SERIAL_COM_H_
#define SERIAL_COM_H_

#include <msp430.h>
//#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>



//declare functions
void serial_init_spi(void);
void ser_output(char *str);
void itoa(int n, char s[]);
void reverse(char s[]);

#endif /* SERIAL_COM_H_ */
