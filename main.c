/************************************************************************************************************************
 *  [FILE NAME]   :      <main.c>                                                                                       *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 1, 2020>                                                                                  *
 *  [Description} :      <Source file for the GAME>                                                                     *
 ************************************************************************************************************************/


/*-------------------------------------------------INCLUDES-------------------------------------------------------------*/

#include "micro_config.h"
#include "glcd.h"
#include "std_types.h"
#include "common_macros.h"
#include "External_interrupts.h"
#include "timer1.h"
#include "slider.h"
#include "dot.h"


/*-------------------------------------------------DEFINITIONS-----------------------------------------------------------*/
#define LEFT_SCREEN 0
#define RIGHT_SCREEN 1
#define UPPER_RIGHT 1
#define DOWN_RIGHT 4
#define UPPER_LEFT 2
#define DOWN_LEFT 3
#define CTRL_PORT_SEVEN_SEGMENTS_DIRECTION DDRB
#define CTRL_PORT_SEVEN_SEGMENTS PORTB
#define DATA_PORT_SEVEN_SEGMENTS_DIRECTION DDRD
#define DATA_PORT_SEVEN_SEGMENTS PORTD
#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define SLIDER_X_COORDINATE 6
#define GAME_ON 1
#define GAME_OFF 0
#define EIGHT_PAGES 8
#define ZERO 0
#define ONE 1
#define PAGE_7 7
#define PAGE_3 3
#define PAGE_2 2
#define PAGE_5 5
#define PAGE_4 4
#define CTRL_PORT_SEVEN_SEGMENTS_BITS 0x07
#define DOT_X_COORDINATE 50
#define TIMER_COMPARE_VALUE 4375
#define MILLISEC_4000 4000
#define MILLISEC_20 20
#define MILLISEC_10 10
#define MILLISEC_3000 3000
#define TEN 10
#define ONE_HUNDRED 100
#define COLUMN_SETTING_ADDRESS 0x40
#define PAGE_SETTING_ADDRESS 0xB8
#define I_BIT 7
#define NOT_LOSE 0
#define LOSE 1
#define DOT_LEVEL_COORDINATE 2




/*-------------------------------------------------FUNCTIONS DEFINITIONS---------------------------------------------------*/
void gameInitiate(uint8);
void writingOnSevenSegments(void);
void moveLeftCallBackFunction(void);
void moveRightCallBackFunction(void);
void SevenSegment_Init(void);

/*----------------------------------------------------GLOBAL VARIABLES----------------------------------------------------*/

uint8 leftMove = ZERO;
uint8 rightMove = ZERO;
uint8 restartFlag = ZERO;
uint16 score = ZERO;

int main() {

	uint8 i;
	uint8 loseFlag = ZERO;
	uint8 gamePlay = GAME_ON;
	uint8 flagSlider = ZERO;
	uint8 originPointSlider = SLIDER_X_COORDINATE;
	uint8 column = DOT_X_COORDINATE;
	uint8 page = PAGE_7;
	uint8 directions = UPPER_RIGHT;
	Timer1_configType configType = { CTC_OCR1A, F_CPU_64, ZERO };
	Timer1_compare_1A configTypecompare = { ZERO, compareMode_1A, TIMER_COMPARE_VALUE };
	E_Interrupts_configType interruptsConfig = { FALLING_EDGE_INT0,
			FALLING_EDGE_INT1, OFF_INT2 };
	DATA_PORT_SEVEN_SEGMENTS_DIRECTION |= 0xF0;
	DATA_PORT_SEVEN_SEGMENTS = (DATA_PORT_SEVEN_SEGMENTS & 0x0F)
			| ((ZERO & 0x0F) << 4);

	CTRL_PORT_SEVEN_SEGMENTS_DIRECTION |= CTRL_PORT_SEVEN_SEGMENTS_BITS; /////////////////////////////////
	CTRL_PORT_SEVEN_SEGMENTS &= ~(CTRL_PORT_SEVEN_SEGMENTS_BITS);

	SevenSegment_Init();

	Timer1_setCallBack(writingOnSevenSegments);
	Timer1_init(&configType);
	Timer1_compareMode_OCR1A(&configTypecompare);
	SREG |= (ONE << I_BIT);

	ExternalInterrupts_INT0_setCallBack(moveLeftCallBackFunction);
	ExternalInterrupts_INT1_setCallBack(moveRightCallBackFunction);
	ExternalInterrupts_init(&interruptsConfig);
	GLCD_Init();
	GLCD_clearScreen();
	GLCD_displayString(PAGE_3, "         Welcome            Break Smash Game");
	_delay_ms(MILLISEC_4000);
	GLCD_clearScreen();
	gameInitiate(gamePlay);
	TIMSK = (ONE << 4);
	while (ONE) {
		flagSlider = ZERO;
		originPointSlider = SLIDER_X_COORDINATE;
		column = DOT_X_COORDINATE;
		page = PAGE_7;
		directions = UPPER_RIGHT;
		i = DOT_LEVEL_COORDINATE;
		gamePlay = GAME_ON;
		loseFlag = ZERO;
		leftMove = ZERO;
		rightMove = ZERO;
		score = ZERO;
		slideMoverRight(&originPointSlider);
		CTRL_PORT_SEVEN_SEGMENTS &= ~(CTRL_PORT_SEVEN_SEGMENTS_BITS);
		while (gamePlay) {
			locationGenerator(&column, &page, &directions, &i, &loseFlag);
			switch (loseFlag) {
			case NOT_LOSE:
				dotCreator(&flagSlider, &column, &page, &directions, &i,
						&score);
				_delay_ms(MILLISEC_20);
				if (!flagSlider) {
					dotEliminator(&column, &page, &i);
				} else {
					flagSlider = ZERO;
				}
				break;
			case LOSE:
				gamePlay = GAME_OFF;
				break;
			}
			if (leftMove) {
				slideMoverLeft(&originPointSlider);
				leftMove = ZERO;
			} else if (rightMove) {
				slideMoverRight(&originPointSlider);
				rightMove = ZERO;
			}
		}
		TIMSK ^= (ONE << 4);
		CTRL_PORT_SEVEN_SEGMENTS |= (CTRL_PORT_SEVEN_SEGMENTS_BITS);
		GLCD_sendCommand(COLUMN_SETTING_ADDRESS); /* Set Y address (column=0) */
		GLCD_sendCommand(PAGE_SETTING_ADDRESS); /* Set x address (page=0) */
		GLCD_CTRL_PORT |= (ONE << CS1); /* Select Left half of display */
		GLCD_CTRL_PORT &= ~(ONE << CS2);
		GLCD_clearScreen();
		restartFlag = ZERO;

		GLCD_displayString(PAGE_2, "Sorry, you just lost  the game");
		GLCD_displayString(PAGE_4, " to play again press  any button");
		_delay_ms(MILLISEC_3000);
		switch (restartFlag) {
		case 1:
			gamePlay = GAME_ON;
			GLCD_sendCommand(COLUMN_SETTING_ADDRESS); /* Set Y address (column=0) */
			GLCD_sendCommand(PAGE_SETTING_ADDRESS); /* Set x address (page=0) */
			GLCD_clearScreen();
			gameInitiate(gamePlay);
			Timer1_compareMode_OCR1A(&configTypecompare);
			TIMSK ^= (ONE << 4);
			break;
		case 0:
			GLCD_sendCommand(COLUMN_SETTING_ADDRESS); /* Set Y address (column=0) */
			GLCD_sendCommand(PAGE_SETTING_ADDRESS); /* Set x address (page=0) */
			GLCD_clearScreen();
			GLCD_displayString(PAGE_2, "      Thank You :')");
			restartFlag = 1;
			break;
		}

		while (!gamePlay) {

		}

	}

	return 0;
}


/*----------------------------------------------------------------------------------------------------
 [Function Name]:  gameInitiate
 [Description]  :  This function is responsible for initiating the game
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void gameInitiate(uint8 gamePlay) {
	int column, page, page_add = PAGE_SETTING_ADDRESS;
	float page_inc = 0.5;

	cli();
	GLCD_CTRL_PORT |= (ONE << CS1); /* Select Left half of display */
	GLCD_CTRL_PORT &= ~(1 << CS2);

	for (page = ZERO; page < EIGHT_PAGES; page++) /* Print pages(4 page of each half)*/
	{
		for (column = ZERO; column < 64; column++) {
			if (gamePlay)
				GLCD_sendData(0xFF);/* Print 64 column of each page */
			else {
				GLCD_sendData(0x00);
			}
		}
		GLCD_CTRL_PORT ^= (ONE << CS1);/* Change segment controller */
		GLCD_CTRL_PORT ^= (ONE << CS2);
		GLCD_sendCommand((page_add + page_inc));/* Increment page address*/
		page_inc = page_inc + 0.5; /* Increment pointer */
	}
	GLCD_sendCommand(COLUMN_SETTING_ADDRESS); /* Set Y address (column=0) */
	GLCD_sendCommand(PAGE_SETTING_ADDRESS); /* Set x address (page=0) */
	sei();
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  SevenSegment_Init
 [Description]  :  This function is responsible for initiating seven segment
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void SevenSegment_Init(void)
{
	DDRD &= (~(ONE << PD2));
	DDRD &= (~(ONE << PD3));
	PORTD |= (ONE << PD2);
	PORTD |= (ONE <<PD3);
}


/*----------------------------------------------------------------------------------------------------
 [Function Name]:  writingOnSevenSegments
 [Description]  :  This function is responsible for displaying score on seven segment
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void writingOnSevenSegments(void) {
	uint8 SEVEN_SEGMENT_SELECTOR = ZERO;
	CTRL_PORT_SEVEN_SEGMENTS = (CTRL_PORT_SEVEN_SEGMENTS & 0xF8)
			| ~(ONE << SEVEN_SEGMENT_SELECTOR);
	DATA_PORT_SEVEN_SEGMENTS = (DATA_PORT_SEVEN_SEGMENTS & 0x0F)
			| (((uint8) (score % TEN)) << 4);
	SEVEN_SEGMENT_SELECTOR++;
	_delay_ms(MILLISEC_10);
	CTRL_PORT_SEVEN_SEGMENTS = (CTRL_PORT_SEVEN_SEGMENTS & 0xF8)
			| ~(ONE << SEVEN_SEGMENT_SELECTOR);
	DATA_PORT_SEVEN_SEGMENTS = (DATA_PORT_SEVEN_SEGMENTS & 0x0F)
			| (((uint8) ((score / TEN) % TEN)) << 4);

	SEVEN_SEGMENT_SELECTOR++;
	_delay_ms(MILLISEC_10);
	CTRL_PORT_SEVEN_SEGMENTS = (CTRL_PORT_SEVEN_SEGMENTS & 0xF8)
			| ~(ONE << SEVEN_SEGMENT_SELECTOR);
	DATA_PORT_SEVEN_SEGMENTS = (DATA_PORT_SEVEN_SEGMENTS & 0x0F)
			| (((uint8) ((score / ONE_HUNDRED) % TEN)) << 4);
	_delay_ms(MILLISEC_10);

}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  moveLeftCallBackFunction
 [Description]  :  This function is responsible for calling by ISR when slider slider moves to the left
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void moveLeftCallBackFunction(void) {
	leftMove = ONE;
	restartFlag = ONE;
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  moveRightCallBackFunction
 [Description]  :  This function is responsible for calling by ISR when slider slider moves to the Right
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void moveRightCallBackFunction(void) {
	rightMove = ONE;
	restartFlag = ONE;
}




