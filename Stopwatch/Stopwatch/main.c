/*
 * main.c
 *
 *  Created on: Aug 26, 2020
 *      Author: Khalid Muhammad
 */


#include "external_interrupts.h"
#include "timers.h"


uint8 s = 0;
uint8 m = 0;
uint8 h = 0;


void update_time(void);
void reset(void);
void pause(void);
void resume(void);


int main(){
	TIMERS_ConfigType timers_config = {TIMER1A, CTC_OCR1A, F_CPU_64, DISCONNECT_OC, 0, 15625};
	INT_ConfigType int0_config = {INT_0, FALLING_INT};
	INT_ConfigType int1_config = {INT_1, RISING_INT};
	INT_ConfigType int2_config = {INT_2, FALLING_INT};

	DDRA |= 0x0F;					/* set the decoder pins as output */
	DDRC |= 0x3F;					/* set the motor pins as output */
	PORTA &= 0xF0;					/* initialize the decoder pins with 0 */
	PORTC &= 0xC0;					/* disable all 7-segments */

	SREG |= (1 << 7);				/* enable all interrupts */
	INT_init(&int0_config);			/* initialize interrupt 0 */
	SET_BIT(PORTD,PD2);				/* enable internal pull up at INT0 */
	INT_setCallBack(INT_0, reset);	/* set callback function of int 0 */
	INT_init(&int1_config);			/* initialize interrupt 1 */
	INT_setCallBack(INT_1, pause);	/* set callback function of int 1 */
	INT_init(&int2_config);			/* initialize interrupt 2 */
	SET_BIT(PORTB,PB2);				/* enable internal pull up at INT2 */
	INT_setCallBack(INT_2, resume);	/* set callback function of int 2 */
	TIMERS_init(&timers_config);	/* initialize timer1 */
	TIMERS_setCallBack(TIMER1A, CTC_OCR1A, update_time);

	while(1) {
		PORTC = (PORTC & 0xC0) | (1 << 0);
		PORTA = (PORTA & 0xF0) | (s % 10);
		_delay_ms(2);
		PORTC = (PORTC & 0xC0) | (1 << 1);
		PORTA = (PORTA & 0xF0) | (s / 10);
		_delay_ms(2);
		PORTC = (PORTC & 0xC0) | (1 << 2);
		PORTA = (PORTA & 0xF0) | (m % 10);
		_delay_ms(2);
		PORTC = (PORTC & 0xC0) | (1 << 3);
		PORTA = (PORTA & 0xF0) | (m / 10);
		_delay_ms(2);
		PORTC = (PORTC & 0xC0) | (1 << 4);
		PORTA = (PORTA & 0xF0) | (h % 10);
		_delay_ms(2);
		PORTC = (PORTC & 0xC0) | (1 << 5);
		PORTA = (PORTA & 0xF0) | (h / 10);
		_delay_ms(2);
	}
}


void update_time(void) {
	if (s < 59)
		s++;
	else {
		s = 0;
		if (m < 59)
			m++;
		else {
			m = 0;
			if (h < 99)
				h++;
			else
				h = 0;
		}

	}
}

void reset(void) {
	h = m = s = 0;
	TCCR1B &= ~(1 << CS11) & ~(1 << CS10);
}

void pause(void) {
	TCCR1B &= ~(1 << CS11) & ~(1 << CS10);
}

void resume(void) {
	TCCR1B |= (1 << CS11) | (1 << CS10);
}
