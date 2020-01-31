/*******************************************************************************************************
 *  [FILE NAME]   :      <External_interrupts.c>                                                       *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                             *
 *  [DATE CREATED]:      <Dec 4, 2019>                                                                 *
 *  [Description} :      <source file for external interrupts Driver>                                  *
 ******************************************************************************************************/

/*----------------------------------------INCLUDES----------------------------------------------------*/

#include "External_interrupts.h"

/*--------------------------------------DEFINITIONS---------------------------------------------------*/
#define NULL_PTR ((void*)0)

/*------------------------------------GLOBAL VARIABLES------------------------------------------------*/
static volatile void (*	g_INT0_callBackPtr)(void) = NULL_PTR;
static volatile void (*	g_INT1_callBackPtr)(void) = NULL_PTR;
static volatile void (*	g_INT2_callBackPtr)(void) = NULL_PTR;

/*----------------------------------ISR DEFINITIONS---------------------------------------------------*/
ISR(INT0_vect)
{
	if (g_INT0_callBackPtr != NULL_PTR) {
			(*g_INT0_callBackPtr)();
}
}

ISR(INT1_vect)
{
	if (g_INT1_callBackPtr != NULL_PTR) {
			(*g_INT1_callBackPtr)();
}

}

ISR(INT2_vect)
{
	if (g_INT2_callBackPtr != NULL_PTR) {
			(*g_INT2_callBackPtr)();
}

}
/*--------------------------------------FUNCTIONS DEFINITIONS----------------------------------------*/

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  ExternalInterrupts_init
 [Description]  :  This function is responsible for initialize external interrupts
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void ExternalInterrupts_init(
		E_Interrupts_configType * E_Interrupts_configType_Ptr) {
	if (E_Interrupts_configType_Ptr->INT0_configType == OFF_INT0) {
		GICR &= ~(1<<INT0);
	} else {
		DDRD &=~(1<<PD2);
		PORTD |= (1<<PD2);
		GICR |= (1<<INT0);
		MCUCR = (MCUCR & 0b11111100) | ((E_Interrupts_configType_Ptr->INT0_configType & 0b00000011)<<(ISC00));

	}
	if (E_Interrupts_configType_Ptr->INT1_configType == OFF_INT1) {
		GICR &= ~(1<<INT1);

	} else {
		DDRD &=~(1<<PD3);
		PORTD |= (1<<PD3);
		GICR |= (1<<INT1);
		MCUCR = (MCUCR & 0b11111100) | ((E_Interrupts_configType_Ptr->INT1_configType & 0b00000011)<<(ISC10));

	}

	if (E_Interrupts_configType_Ptr->INT2_configType == OFF_INT2) {
		GICR &= ~(1<<INT2);
	} else {
		SREG   &= ~(1<<7);      // Disable interrupts by clearing I-bit
		DDRB   &= (~(1<<PB2));   // Configure INT2/PB2 as input pin
		PORTB  |= (1<<PB2);
		GICR   |= (1<<INT2);	// Enable external interrupt pin INT2
		MCUCSR = (MCUCSR & 0b10111111) | ((E_Interrupts_configType_Ptr->INT2_configType & 0b00000001)<<(ISC2));
		SREG   |= (1<<7);       // Enable interrupts by setting I-bit
	}

}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  ExternalInterrupts_Deinit
 [Description]  :  This function is responsible for deinitilization of external interrupts
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void ExternalInterrupts_Deinit()
{
	GICR &= ~(1<<INT0);
	GICR &= ~(1<<INT1);
	GICR &= ~(1<<INT2);
}

void ExternalInterrupts_INT0_setCallBack(void (*INT0_setCallBack_Ptr)(void)) {
	g_INT0_callBackPtr = INT0_setCallBack_Ptr;
}

void ExternalInterrupts_INT1_setCallBack(void (*INT1_setCallBack_Ptr)(void)) {
	g_INT1_callBackPtr = INT1_setCallBack_Ptr;
}

void ExternalInterrupts_INT2_setCallBack(void (*INT2_setCallBack_Ptr)(void)) {
	g_INT2_callBackPtr = INT2_setCallBack_Ptr;
}

