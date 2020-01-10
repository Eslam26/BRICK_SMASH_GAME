/************************************************************************************************************************
 *  [FILE NAME]   :      <slider.c>                                                                                     *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 1, 2020>                                                                                  *
 *  [Description} :      <Source file for slider functions>                                                             *
 ************************************************************************************************************************/

/*-------------------------------------------------INCLUDES-------------------------------------------------------------*/
#include "glcd.h"
#include "slider.h"


/*----------------------------------------------------------------------------------------------------
 [Function Name]:  locationGenerator
 [Description]  :  This function is responsible for location of the dot
 [Args]         :
 [IN]           :
  	  	  	      uint8 * const originPointSlider_Ptr
  	  	  	      	  	  This argument shall contain the address of X-cordinate of the slider
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void slideMoverRight(uint8 * const originPointSlider_Ptr) {
	uint8 state, counterTemp, n, counter;
	cli();
	*originPointSlider_Ptr = *originPointSlider_Ptr + 4;
	if (*originPointSlider_Ptr + 8 >= 128) {
		*originPointSlider_Ptr = *originPointSlider_Ptr - 4;
	} else {
		counter = *originPointSlider_Ptr;
		counterTemp = counter - 4;
		while (counterTemp != counter) {
			if (counterTemp < 64) {
				GLCD_CTRL_PORT |= (1 << CS1); /* Select Left half of display */
				GLCD_CTRL_PORT &= ~(1 << CS2);
				GLCD_sendCommand(0x40 + counterTemp);
				GLCD_sendCommand(0xB8 + 7);
				for (n = 0; n < 2; n++) {
					state = GLCD_readData();
				}
				GLCD_sendCommand(0x40 + counterTemp);
				GLCD_sendCommand(0xB8 + 7);
				GLCD_sendData(state & ~(0x80));
			} else {
				GLCD_CTRL_PORT |= (1 << CS2); /* Select Right half of display */
				GLCD_CTRL_PORT &= ~(1 << CS1);
				GLCD_sendCommand(0x40 + counterTemp - 64);
				GLCD_sendCommand(0xB8 + 7);
				for (n = 0; n < 2; n++) {
					state = GLCD_readData();
				}
				GLCD_sendCommand(0x40 + counterTemp - 64);
				GLCD_sendCommand(0xB8 + 7);
				GLCD_sendData(state & ~(0x80));
			}
			counterTemp++;
		}
		sei();
		cli();
		for (; counterTemp <= counter + 8; counterTemp++) {
			if (counterTemp < 64) {
				GLCD_CTRL_PORT |= (1 << CS1); /* Select Left half of display */
				GLCD_CTRL_PORT &= ~(1 << CS2);
				GLCD_sendCommand(0x40 + counterTemp);
				GLCD_sendCommand(0xB8 + 7);
				for (n = 0; n < 2; n++) {
					state = GLCD_readData();
				}
				GLCD_sendCommand(0x40 + counterTemp);
				GLCD_sendCommand(0xB8 + 7);
				GLCD_sendData((state & ~(0x80)) | 0x80);
			} else {
				GLCD_CTRL_PORT |= (1 << CS2); /* Select Right half of display */
				GLCD_CTRL_PORT &= ~(1 << CS1);
				GLCD_sendCommand(0x40 + counterTemp - 64);
				GLCD_sendCommand(0xB8 + 7);
				for (n = 0; n < 2; n++) {
					state = GLCD_readData();
				}
				GLCD_sendCommand(0x40 + counterTemp - 64);
				GLCD_sendCommand(0xB8 + 7);
				GLCD_sendData((state & ~(0x80)) | 0x80);
			}
		}
	}
	sei();
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  slideMoverLeft
 [Description]  :  This function is responsible for location of the dot
 [Args]         :
 [IN]           :
  	  	  	      uint8 * const originPointSlider_Ptr
  	  	  	      	  	  This argument shall contain the address of X-cordinate of the slider
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void slideMoverLeft(uint8 * const originPointSlider_Ptr) {
	uint8 state, counterTemp,n,counter;
	cli();
	*originPointSlider_Ptr = *originPointSlider_Ptr - 4;
	if (*originPointSlider_Ptr >= 128) {
		*originPointSlider_Ptr = *originPointSlider_Ptr + 4;
	} else {
		counter = *originPointSlider_Ptr;
		counterTemp = counter + 8;
		for (counterTemp = counter; counterTemp <= counter + 4; counterTemp++) {
			if (counterTemp < 64) {
				GLCD_CTRL_PORT |= (1 << CS1); /* Select Left half of display */
				GLCD_CTRL_PORT &= ~(1 << CS2);
				GLCD_sendCommand(0x40 + counterTemp);
				GLCD_sendCommand(0xB8 + 7);
				for (n = 0; n < 2; n++) {
					state = GLCD_readData();
				}
				GLCD_sendCommand(0x40 + counterTemp);
				GLCD_sendCommand(0xB8 + 7);
				GLCD_sendData((state & ~(0x80)) | 0x80);
			} else {
				GLCD_CTRL_PORT |= (1 << CS2); /* Select Right half of display */
				GLCD_CTRL_PORT &= ~(1 << CS1);
				GLCD_sendCommand(0x40 + counterTemp - 64);
				GLCD_sendCommand(0xB8 + 7);
				for (n = 0; n < 2; n++) {
					state = GLCD_readData();
				}
				GLCD_sendCommand(0x40 + counterTemp - 64);
				GLCD_sendCommand(0xB8 + 7);
				GLCD_sendData((state & ~(0x80)) | 0x80);
			}
		}
		sei();
		cli();
		for (counterTemp = counter + 8; counterTemp <= counter + 8 + 4;
				counterTemp++) {
			if (counterTemp < 64) {
				GLCD_CTRL_PORT |= (1 << CS1); /* Select Left half of display */
				GLCD_CTRL_PORT &= ~(1 << CS2);
				GLCD_sendCommand(0x40 + counterTemp);
				GLCD_sendCommand(0xB8 + 7);
				for (n = 0; n < 2; n++) {
					state = GLCD_readData();
				}
				GLCD_sendCommand(0x40 + counterTemp);
				GLCD_sendCommand(0xB8 + 7);
				GLCD_sendData(state & ~(0x80));
			} else {
				GLCD_CTRL_PORT |= (1 << CS2); /* Select Right half of display */
				GLCD_CTRL_PORT &= ~(1 << CS1);
				GLCD_sendCommand(0x40 + counterTemp - 64);
				GLCD_sendCommand(0xB8 + 7);
				for (n = 0; n < 2; n++) {
					state = GLCD_readData();
				}
				GLCD_sendCommand(0x40 + counterTemp - 64);
				GLCD_sendCommand(0xB8 + 7);
				GLCD_sendData(state & ~(0x80));
			}
		}
	}
	sei();

}

