/*
 * rgb_interface.h
 *
 *  Created on: Mar 12, 2021
 *      Author: ckemere
 */

#ifndef RGB_INTERFACE_H_
#define RGB_INTERFACE_H_
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
extern unsigned int current_note;
extern unsigned int songLength;
extern unsigned int *note_position;
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

void set_green(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5, unsigned int led6, unsigned int led7);
void set_red(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5, unsigned int led6, unsigned int led7);
void set_yellow(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5, unsigned int led6, unsigned int led7);
void set_blue(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5, unsigned int led6, unsigned int led7);
void set_orange(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5, unsigned int led6, unsigned int led7);
void ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7);

void g_ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7);
void r_ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7);
void y_ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7);
void b_ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7);
void o_ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7);

void PlaySound(unsigned int *notes, unsigned int length);

void win_screen(void);
void loss_screen(void);
//void PlaySound(unsigned int *g_notes, unsigned int *r_notes, unsigned int *y_notes, unsigned int *b_notes, unsigned int *o_notes, unsigned int *duration, unsigned int length);

#endif /* RGB_INTERFACE_H_ */
