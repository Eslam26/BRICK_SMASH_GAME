/*******************************************************************************************************
 *  [FILE NAME]   :      <glcd.h>                                                                      *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                             *
 *  [DATE CREATED]:      <JAN 1, 2020>                                                                *
 *  [Description} :      <Header file for graphical lcd interface driver>                              *
 ******************************************************************************************************/

#ifndef GLCD_H_
#define GLCD_H_


/*---------------------------------------INCLUDES-----------------------------------------------------*/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*-------------------------------------DEFINITIONS----------------------------------------------------*/
#define GLCD_DATA_PORT	         PORTA		/* Defines data port for GLCD data pins */
#define GLCD_DATA_PIN            PINA
#define GLCD_CTRL_PORT	         PORTC		/* Defines control port for GLCD control pins*/
#define GLCD_DATA_PORT_DIRECTION DDRA		/* Defines data port for GLCD data pins */
#define GLCD_CTRL_PORT_DIRECTION DDRC		/* Defines command port for GLCD control pins */
#define RS		 PC0		/* Defines RS pin */
#define RW		 PC1        /* Defines RW pin */
#define EN		 PC2        /* Defines EN pin */
#define CS1		 PC3        /* Defines CS1 pin */
#define CS2		 PC4        /* Defines CS2 pin */
#define RST		 PC5        /* Defines RST pin */
#define TotalPage	 8


/*------------------------------------FUNCTIONS DEFINITIONS-------------------------------------------*/
void GLCD_Init();
void GLCD_clearScreen();
void GLCD_displayImage(const char* image);
void GLCD_displayString(char page_no, char *str);
void GLCD_sendCommand(char Command);
void GLCD_sendData(char Data);
uint8 GLCD_readData();
#endif /* GLCD_H_ */
