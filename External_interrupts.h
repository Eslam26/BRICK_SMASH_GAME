/*******************************************************************************************************
 *  [FILE NAME]   :      <External_interrupts.h>                                                       *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                             *
 *  [DATE CREATED]:      <Dec 4, 2019>                                                                 *
 *  [Description} :      <header file for external interrupts Driver>                                  *
 ******************************************************************************************************/

#ifndef EXTERNAL_INTERRUPTS_H_
#define EXTERNAL_INTERRUPTS_H_

/*----------------------------------------INCLUDES-----------------------------------*/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*----------------------------------------EXTERNS------------------------------------*/


/*-----------------------------------------ENUMS-------------------------------------*/

typedef enum {
	LOW_LEVEL_INT0, ANY_CHANGE_INT0 , FALLING_EDGE_INT0 , RISING_EDGE_INT0 , OFF_INT0
}E_Interrupts_INT0_configType;

typedef enum {
	LOW_LEVEL_INT1, ANY_CHANGE_INT1 , FALLING_EDGE_INT1 , RISING_EDGE_INT1 , OFF_INT1
}E_Interrupts_INT1_configType;

typedef enum {
	FALLING_EDGE_INT2 , RISING_EDGE_INT2 , OFF_INT2
}E_Interrupts_INT2_configType;


/*-------------------------------------STRUCTURES--------------------------------------*/

typedef struct {
	E_Interrupts_INT0_configType INT0_configType;
	E_Interrupts_INT1_configType INT1_configType;
	E_Interrupts_INT2_configType INT2_configType;
}E_Interrupts_configType;


/*-----------------------------------FUNCTIONS DECLARATIONS----------------------------*/

void ExternalInterrupts_init(E_Interrupts_configType *);
void ExternalInterrupts_Deinit(void);
void ExternalInterrupts_INT0_setCallBack(void (*INT0_setCallBack_Ptr)(void));
void ExternalInterrupts_INT1_setCallBack(void (*INT0_setCallBack_Ptr)(void));
void ExternalInterrupts_INT2_setCallBack(void (*INT0_setCallBack_Ptr)(void));

/*------------------------------------EXTERNAL VARIABLES------------------------------*/
extern E_Interrupts_configType interruptsConfig;

#endif /* EXTERNAL_INTERRUPTS_H_ */
