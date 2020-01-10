/************************************************************************************************************************
 *  [FILE NAME]   :      <dot.h>                                                                                        *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 1, 2020>                                                                                  *
 *  [Description} :      <Header file for the dot functions>                                                            *
 ************************************************************************************************************************/
#ifndef DOT_H_
#define DOT_H_

/*-------------------------------------------------DEFINES-------------------------------------------------------------*/
#define UPPER_RIGHT 1
#define DOWN_RIGHT 4
#define UPPER_LEFT 2
#define DOWN_LEFT 3



/*------------------------------------------FUNCTIONS DEFINITIONS------------------------------------------------------*/
void dotCreator(uint8 *const flagSlider_Ptr, uint8 *const column_Ptr,
		uint8 *const page_Ptr, uint8 *const directions_Ptr, uint8 *const i_Ptr,
		uint16 *const score_Ptr);
void locationGenerator(uint8 *const column_Ptr, uint8 *const page_Ptr,
		uint8 *const directions_Ptr, uint8 *const i_Ptr,
		uint8 *const loseFlag_Ptr);
void dotEliminator(uint8 *const column_Ptr, uint8 *const page_Ptr,
		uint8 *const i_Ptr);

#endif /* DOT_H_ */
