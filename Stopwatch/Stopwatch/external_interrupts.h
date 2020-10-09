/* Driver for Atmega16 EXTERNAL INTERRUPTS modules */

#ifndef EXTERNAL_INTERRUPTS_H_
#define EXTERNAL_INTERRUPTS_H_


#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"


typedef enum {
	INT_0, INT_1, INT_2
} INT_Num;

typedef enum {
	LOW_LEVEL, ANY_CHANGE, FALLING_INT, RISING_INT
} INT_Trigger;

typedef struct {
	INT_Num num;
	INT_Trigger trigger;
} INT_ConfigType;


/* Initialize an EXTERNAL INTERRUPT module */
void INT_init(const INT_ConfigType * const config_ptr);

/* Set the Call Back function address */
void INT_setCallBack(INT_Num num, void (*f_ptr)(void));

/* Disable an EXTERNAL INTERRUPT module */
void INT_deInit(INT_Num num);


#endif /* EXTERNAL_INTERRUPTS_H_ */
