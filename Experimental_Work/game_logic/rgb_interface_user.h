/*
 * rgb_interface_user.c
 *
 *  Created on: April 13, 2024
 *      Author: Natalia M
 */

#ifndef RGB_INTERFACE_USER_H_
#define RGB_INTERFACE_USER_H_
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

//#define Q 4000  //quarter note duration

// Declare and define the variables used for LED control
extern int led_1;
extern int led_2;
extern int led_3;
extern int led_4;
extern int led_5;
extern int reset;

//int song;
int current_note;
extern int songLength;
//extern int *note_position;
//extern int *g_note_position;
//extern int *r_note_position;
//extern int *y_note_position;
//extern int *b_note_position;
//extern int *o_note_position;
//extern int *duration_position;

void rgb_init_spi(void);
void rgb_send_frame(const uint8_t *frame1, bool wait_for_completion);
void rgb_send_start();
void rgb_send_end();
void set_temperature(int led1, int led2, int led3, int led4, int led5);

//void set_green(int led1, int led2, int led3, int led4, int led5, int led6, int led7);
//void set_red(int led1, int led2, int led3, int led4, int led5, int led6, int led7);
//void set_yellow(int led1, int led2, int led3, int led4, int led5, int led6, int led7);
//void set_blue(int led1, int led2, int led3, int led4, int led5, int led6, int led7);
//void set_orange(int led1, int led2, int led3, int led4, int led5, int led6, int led7);
//void ripple(int next_seq, int led_1, int led_2, int led_3, int led_4, int led_5, int led_6, int led_7);
//
//void g_ripple(int next_seq, int led_1, int led_2, int led_3, int led_4, int led_5, int led_6, int led_7);
//void r_ripple(int next_seq, int led_1, int led_2, int led_3, int led_4, int led_5, int led_6, int led_7);
//void y_ripple(int next_seq, int led_1, int led_2, int led_3, int led_4, int led_5, int led_6, int led_7);
//void b_ripple(int next_seq, int led_1, int led_2, int led_3, int led_4, int led_5, int led_6, int led_7);
//void o_ripple(int next_seq, int led_1, int led_2, int led_3, int led_4, int led_5, int led_6, int led_7);
//
//void PlaySound(int *g_notes, int *r_notes, int *y_notes, int *b_notes, int *o_notes, int *duration, int length);

#endif /* RGB_INTERFACE_USER_H_ */
