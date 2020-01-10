/************************************************************************************************************************
 *  [FILE NAME]   :      <dot.c>                                                                                        *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 1, 2020>                                                                                  *
 *  [Description} :      <Source file for the dot functions>                                                            *
 ************************************************************************************************************************/

/*-------------------------------------------------INCLUDES-------------------------------------------------------------*/
#include "glcd.h"
#include "dot.h"

/*--------------------------------------------FUNCTIONS DEFINITIONS-----------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------
 [Function Name]:  dotEliminator
 [Description]  :  This function is responsible for dot erasing
 [Args]         :
 [IN]           :
                     uint8 *const column_Ptr
                     	 	this argument shall contain the address of column variable which indicate Dot X coordinate
                     uint8 *const page_Ptr
                     	 	this argument shall contain the address of column variable which indicate Dot Y coordinate
                     uint8 *const i_Ptr
                          	this argument shall contain the address of column variable which indicate Dot Level coordinate
 [Returns]      :  This function returns void
 ------------------------------------------------------------------------------------------------------------------------*/
void dotEliminator(uint8 *const column_Ptr, uint8 *const page_Ptr,
		uint8 *const i_Ptr) {
	uint8 state;
	uint8 columnTemp;
	uint8 n;
	cli();
	if (*column_Ptr < 64) {
		columnTemp = *column_Ptr;
		GLCD_CTRL_PORT |= (1 << CS1); /* Select Left half of display */
		GLCD_CTRL_PORT &= ~(1 << CS2);
	} else if (*column_Ptr >= 64) {
		columnTemp = *column_Ptr - 64;
		GLCD_CTRL_PORT |= (1 << CS2); /* Select Right half of display */
		GLCD_CTRL_PORT &= ~(1 << CS1);
	}
	GLCD_sendCommand(0x40 + columnTemp); /* Set column address (column=0) */
	GLCD_sendCommand(0xB8 + *page_Ptr); /* Set page address (page=0) */
	for (n = 0; n < 2; n++) {
		state = GLCD_readData();
	}
	GLCD_sendCommand(0x40 + columnTemp); /* Set column address (column=0) */
	GLCD_sendCommand(0xB8 + *page_Ptr); /* Set page address (page=0) */
	GLCD_sendData(state & ~(0x80 >> *i_Ptr));
	sei();
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  dotCreator
 [Description]  :  This function is responsible for dot creator
 [Args]         :
 [IN]           :
                     uint8 *const column_Ptr
                     	 	this argument shall contain the address of column variable which indicate Dot X coordinate
                     uint8 *const page_Ptr
                     	 	this argument shall contain the address of column variable which indicate Dot Y coordinate
                     uint8 *const i_Ptr
                          	this argument shall contain the address of column variable which indicate Dot Level coordinate
                     uint8 *const directions_Ptr
                            this argument shall contain the address of direction variable
                     uint8 *const flagSlider_Ptr
                             this argument shall contain the address of flagSlider check variable
                     uint16 *const score_Ptr
                              this argument shall contain the address of score variable
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void dotCreator(uint8 *const flagSlider_Ptr, uint8 *const column_Ptr,
		uint8 *const page_Ptr, uint8 *const directions_Ptr, uint8 *const i_Ptr,
		uint16 *const score_Ptr) {
	uint8 state;
	uint8 columnTemp;
	uint8 n;
	cli();
	if (*column_Ptr < 64) {
		columnTemp = *column_Ptr;
		GLCD_CTRL_PORT |= (1 << CS1); /* Select Left half of display */
		GLCD_CTRL_PORT &= ~(1 << CS2);
	} else if (*column_Ptr >= 64) {
		columnTemp = *column_Ptr - 64;
		GLCD_CTRL_PORT |= (1 << CS2); /* Select Right half of display */
		GLCD_CTRL_PORT &= ~(1 << CS1);
	}
	GLCD_sendCommand(0x40 + columnTemp); /* Set column address (column=0) */
	GLCD_sendCommand(0xB8 + *page_Ptr); /* Set page address (page=0) */
	for (n = 0; n < 2; n++) {
		state = GLCD_readData();
	}
	if ((state & (0x80 >> (*i_Ptr)))) {
		if (*page_Ptr == 7) {
			*flagSlider_Ptr = 1;
		} else {
			(*score_Ptr)++;
		}
		switch (*directions_Ptr) {
		case UPPER_RIGHT:
			*directions_Ptr = DOWN_RIGHT;
			break;
		case UPPER_LEFT:
			*directions_Ptr = DOWN_LEFT;
			break;
		case DOWN_LEFT:
			*directions_Ptr = UPPER_LEFT;
			break;
		case DOWN_RIGHT:
			*directions_Ptr = UPPER_RIGHT;
			break;

		}
	}
	GLCD_sendCommand(0x40 + columnTemp); /* Set column address (column=0) */
	GLCD_sendCommand(0xB8 + *page_Ptr); /* Set page address (page=0) */
	GLCD_sendData((state & ~(0x80 >> *i_Ptr)) | 0x80 >> (*i_Ptr));
	sei();
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  locationGenerator
 [Description]  :  This function is responsible for location of the dot
 [Args]         :
 [IN]           :
                     uint8 *const column_Ptr
                     	 	this argument shall contain the address of column variable which indicate Dot X coordinate
                     uint8 *const page_Ptr
                     	 	this argument shall contain the address of column variable which indicate Dot Y coordinate
                     uint8 *const i_Ptr
                          	this argument shall contain the address of column variable which indicate Dot Level coordinate
                     uint8 *const directions_Ptr
                            this argument shall contain the address of direction variable
                     uint8 *const loseFlag_Ptr
                             this argument shall contain the address of loseFlag check Variable
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void locationGenerator(uint8 *const column_Ptr, uint8 *const page_Ptr,
		uint8 *const directions_Ptr, uint8 *const i_Ptr,
		uint8 *const loseFlag_Ptr) {
	cli();
	switch (*directions_Ptr) {
	case DOWN_RIGHT: //DOWN_RIGHT Direction
		*column_Ptr = *column_Ptr + 1;
		(*i_Ptr)--;
		if (*column_Ptr == 127) {
			*directions_Ptr = DOWN_LEFT;
		}
		if ((*i_Ptr) > 7) {
			(*i_Ptr) = 7;
			(*page_Ptr)++;
		}
		if (*page_Ptr == 8) {
			(*loseFlag_Ptr) = 1;
		}
		break;
	case DOWN_LEFT:  //DOWN_LEFT Direction
		*column_Ptr = *column_Ptr - 1;
		(*i_Ptr)--;
		if (*column_Ptr == 0) {
			*directions_Ptr = DOWN_RIGHT;
		}
		if ((*i_Ptr) > 7) {
			(*i_Ptr) = 7;
			(*page_Ptr)++;
		}
		if (*page_Ptr == 8) {
			(*loseFlag_Ptr) = 1;
		}
		break;
	case UPPER_LEFT:   //UPPER_LEFT Direction
		*column_Ptr = *column_Ptr - 1;
		(*i_Ptr)++;
		if (*column_Ptr == 0) {
			*directions_Ptr = UPPER_RIGHT;
		}
		if ((*i_Ptr) == 8) {
			(*i_Ptr) = 0;
			(*page_Ptr)--;
		}
		break;
	case UPPER_RIGHT:  //UPPER_RIGHT Direction
		*column_Ptr = *column_Ptr + 1;
		(*i_Ptr)++;
		if (*column_Ptr == 127) {
			*directions_Ptr = UPPER_LEFT;
		}
		if ((*i_Ptr) == 8) {
			*i_Ptr = 0;
			(*page_Ptr)--;
		}
		break;
	}
	sei();
}
