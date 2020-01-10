/*******************************************************************************************************
 *  [FILE NAME]   :      <glcd.c>                                                                      *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                             *
 *  [DATE CREATED]:      <JAN 1, 2020>                                                                *
 *  [Description} :      <Source file for graphical LCD interface driver>                              *
 ******************************************************************************************************/

/*-----------------------------------------INCLUDES----------------------------------------------------*/
#include "glcd.h"
#include "font_config.h"


/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GLCD_sendCommand
 [Description]  :  This function is responsible for sending command to GLCD
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void GLCD_sendCommand(char Command)		/* GLCD command function */
{
	GLCD_DATA_PORT = Command;		/* Copy command on data pin */
	GLCD_CTRL_PORT &= ~(1 << RS);	/* Make RS LOW for command register*/
	GLCD_CTRL_PORT &= ~(1 << RW);	/* Make RW LOW for write operation */
	GLCD_CTRL_PORT |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	GLCD_CTRL_PORT &= ~(1 << EN);
	_delay_us(5);
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GLCD_sendData
 [Description]  :  This function is responsible for sending data to GLCD
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void GLCD_sendData(char Data)		/* GLCD data function */
{
	GLCD_DATA_PORT = Data;		/* Copy data on data pin */
	GLCD_CTRL_PORT |=  (1 << RS);	/* Make RS HIGH for data register */
	GLCD_CTRL_PORT &= ~(1 << RW);	/* Make RW LOW for write operation */
	GLCD_CTRL_PORT |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	GLCD_CTRL_PORT &= ~(1 << EN);
	_delay_us(5);
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GLCD_readData
 [Description]  :  This function is responsible for reading data from GLCD
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
uint8 GLCD_readData()		/* GLCD data function */
{

	uint8 Data;
	GLCD_DATA_PORT_DIRECTION = 0x00;
	_delay_ms(5);
	GLCD_CTRL_PORT |=  (1 << RS);	/* Make RS HIGH for data register */
	GLCD_CTRL_PORT |= (1 << RW);	/* Make RW LOW for write operation */
	GLCD_CTRL_PORT |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Data = GLCD_DATA_PIN;
	_delay_us(5);
	GLCD_CTRL_PORT &= ~(1 << EN);
	_delay_us(5);
	GLCD_DATA_PORT_DIRECTION = 0xFF;
	return Data;
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GLCD_Init
 [Description]  :  This function is responsible for initializing GLCD
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void GLCD_Init()			/* GLCD initialize function */
{
	GLCD_DATA_PORT_DIRECTION = 0xFF;
	GLCD_CTRL_PORT_DIRECTION = 0xFF;
	/* Select both left & right half of display & Keep reset pin high */
	GLCD_CTRL_PORT |= (1 << CS1) | (1 << CS2) | (1 << RST);
	_delay_ms(20);
	GLCD_sendCommand(0x3E);		/* Display OFF */
	GLCD_sendCommand(0x40);		/* Set Y address (column=0) */
	GLCD_sendCommand(0xB8);		/* Set x address (page=0) */
	GLCD_sendCommand(0xC0);		/* Set z address (start line=0) */
	GLCD_sendCommand(0x3F);		/* Display ON */
}


/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GLCD_clearScreen
 [Description]  :  This function is responsible for clearing the screen
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void GLCD_clearScreen()			/* GLCD all display clear function */
{
	{
		int column,page,page_add=0xB8,k=0;
		float page_inc= 0.5 ;
		char byte;
		GLCD_sendCommand(0x40);		/* Set Y address (column=0) */
		GLCD_sendCommand(0xB8);		/* Set x address (page=0) */

		GLCD_CTRL_PORT |= (1 << CS1);	/* Select Left half of display */
		GLCD_CTRL_PORT &= ~(1 << CS2);

		for(page=0;page<16;page++)	/* Print pages(8 page of each half)*/
		{
			for(column=0;column<64;column++)
			{
				GLCD_sendData(0);/* Print 64 column of each page */
			}
			GLCD_CTRL_PORT ^= (1 << CS1);/* Change segment controller */
			GLCD_CTRL_PORT ^= (1 << CS2);
			GLCD_sendCommand((page_add+page_inc));/* Increment page address*/
			page_inc=page_inc+ 0.5 ;		/* Increment pointer */
		}
		GLCD_sendCommand(0x40);		/* Set Y address (column=0) */
		GLCD_sendCommand(0xB8);		/* Set x address (page=0) */
	}

}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GLCD_displayImage
 [Description]  :  This function is responsible for displaying the image
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void GLCD_displayImage(const char* image)	/* GLCD string write function */
{
	int column,page,page_add=0xB8,k=0;
	float page_inc=0.5;
	char byte;

	GLCD_CTRL_PORT |= (1 << CS1);	/* Select Left half of display */
	GLCD_CTRL_PORT &= ~(1 << CS2);

	for(page=0;page<16;page++)	/* Print pages(8 page of each half)*/
	{
		for(column=0;column<64;column++)
		{
			byte = pgm_read_byte(&image[k+column]);
			GLCD_sendData(byte);/* Print 64 column of each page */
		}
		GLCD_CTRL_PORT ^= (1 << CS1);/* Change segment controller */
		GLCD_CTRL_PORT ^= (1 << CS2);
		GLCD_sendCommand((page_add+page_inc));/* Increment page address*/
		page_inc=page_inc+0.5;
		k=k+64;			/* Increment pointer */
	}
	GLCD_sendCommand(0x40);		/* Set Y address (column=0) */
	GLCD_sendCommand(0xB8);		/* Set x address (page=0) */
}


/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GLCD_displayString
 [Description]  :  This function is responsible for displaying the string
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void GLCD_displayString(char page_no, char *str)/* GLCD string write function */
{
	unsigned int i, column;
	unsigned int Page = ((0xB8) + page_no);
	unsigned int Y_address = 0;
	float Page_inc = 0.5;

	GLCD_CTRL_PORT |= (1 << CS1);	/* Select Left half of display */
	GLCD_CTRL_PORT &= ~(1 << CS2);

	GLCD_sendCommand(Page);
	for(i = 0; str[i] != 0; i++)	/* Print char in string till null */
	{
		if (Y_address > (1024-(((page_no)*128)+FontWidth)))
		break;
		if (str[i]!=32)  /* space value */
		{
			for (column=1; column<=FontWidth; column++)
			{
				if ((Y_address+column)==(128*((int)(Page_inc+0.5))))
				{
					if (column == FontWidth)
					break;
					GLCD_sendCommand(0x40); /* set Y-address to zero */
					Y_address = Y_address + column;
					GLCD_CTRL_PORT ^= (1 << CS1);
					GLCD_CTRL_PORT ^= (1 << CS2);
					GLCD_sendCommand(Page + Page_inc);
					Page_inc = Page_inc + 0.5;
				}
			}
		}
		if (Y_address>(1024-(((page_no)*128)+FontWidth)))
		break;
		if((font[((str[i]-32)*FontWidth)+4])==0 || str[i]==32)
		{
			for(column=0; column<FontWidth; column++)
			{
				GLCD_sendData(font[str[i]-32][column]);
				if((Y_address+1)%64==0)
				{
					GLCD_CTRL_PORT ^= (1 << CS1);
					GLCD_CTRL_PORT ^= (1 << CS2);
					GLCD_sendCommand((Page+Page_inc));
					Page_inc = Page_inc + 0.5;
				}
				Y_address++;
			}
		}
		else
		{
			for(column=0; column<FontWidth; column++)
			{
				GLCD_sendData(font[str[i]-32][column]);
				if((Y_address+1)%64==0)
				{
					GLCD_CTRL_PORT ^= (1 << CS1);
					GLCD_CTRL_PORT ^= (1 << CS2);
					GLCD_sendCommand((Page+Page_inc));
					Page_inc = Page_inc + 0.5;
				}
				Y_address++;
			}
			GLCD_sendData(0);
			Y_address++;
			if((Y_address)%64 == 0)
			{
				GLCD_CTRL_PORT ^= (1 << CS1);
				GLCD_CTRL_PORT ^= (1 << CS2);
				GLCD_sendCommand((Page+Page_inc));
				Page_inc = Page_inc + 0.5;
			}
		}
	}
	GLCD_sendCommand(0x40);		/* Set Y address (column=0) */
	GLCD_sendCommand(0xB8);		/* Set x address (page=0) */
}
