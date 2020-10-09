/* Driver for Atmega16 EXTERNAL INTERRUPTS modules */

#include "external_interrupts.h"


/* Global pointers to functions holding the address of
 * the call back function of each external interrupt mode */
static volatile void (*g_int0_callback)(void) = NULL_PTR;
static volatile void (*g_int1_callback)(void) = NULL_PTR;
static volatile void (*g_int2_callback)(void) = NULL_PTR;


/* Interrupt Sevice Routines of all external interrupt modules */
ISR(INT0_vect) {
	if (g_int0_callback != NULL_PTR)
		(*g_int0_callback)();
}

ISR(INT1_vect) {
	if (g_int1_callback != NULL_PTR)
		(*g_int1_callback)();
}

ISR(INT2_vect) {
	if (g_int2_callback != NULL_PTR)
		(*g_int2_callback)();
}


void INT_init(const INT_ConfigType * const config_ptr) {
	if (config_ptr->num == INT_2) {
		/* set INT2 (PB2) pin as input */
		CLEAR_BIT(DDRB,PB2);
		
		/* set INT2 trigger event */
		(config_ptr->trigger & 1) ? SET_BIT(MCUCSR,ISC2) : CLEAR_BIT(MCUCSR,ISC2);
		
		/* enable INT2 */
		SET_BIT(GICR,INT2);
	}

	else if (config_ptr->num == INT_0) {
		/* set INT0 (PD2) pin as input */
		CLEAR_BIT(DDRD,PD2);

		/* set INT0 trigger event */
		MCUCR = (MCUCR & 0xFC) | (config_ptr->trigger);

		/* enable INT0 */
		SET_BIT(GICR,INT0);
	}

	else if (config_ptr->num == INT_1) {
		/* set INT1 (PD3) pin as input */
		CLEAR_BIT(DDRD,PD3);
		
		/* set INT1 trigger event */
		MCUCR = (MCUCR & 0xF3) | (config_ptr->trigger << 2);

		/* enable INT1 */
		SET_BIT(GICR,INT1);
	}
}

void INT_setCallBack(INT_Num num, void (*f_ptr)(void)) {
	if (num == INT_2)
		g_int2_callback = f_ptr;
	else if (num == INT_0)
		g_int0_callback = f_ptr;
	else if (num == INT_1)
		g_int1_callback = f_ptr;
}

void INT_deInit(INT_Num num) {
	if (num == INT_2)
		CLEAR_BIT(GICR,INT2);
	else if (num == INT_0)
		CLEAR_BIT(GICR,INT0);
	else if (num == INT_1)
		CLEAR_BIT(GICR,INT1);
}
