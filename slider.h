/*******************************************************************************************************
 *  [FILE NAME]   :      <slider.h>                                                                    *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                             *
 *  [DATE CREATED]:      <JAN 1, 2020>                                                                *
 *  [Description} :      <Header file for graphical lcd interface driver>                              *
 ******************************************************************************************************/

#ifndef SLIDER_H_
#define SLIDER_H_

/*-----------------------------------------INCLUDES---------------------------------------------------*/
#include "common_macros.h"
#include "std_types.h"
/*---------------------------------------FUNCTIONS DEFINITIONS----------------------------------------*/
void slideMoverLeft(uint8 * const originPointSlider_Ptr);
void slideMoverRight(uint8 * const originPointSlider_Ptr);

#endif /* SLIDER_H_ */
