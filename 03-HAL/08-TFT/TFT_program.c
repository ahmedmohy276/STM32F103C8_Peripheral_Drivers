/*****************************************/
/* Author  :  Ahmed Mohy                 */
/* Date    :  5 January 2022             */
/* Version :  V01                        */
/*****************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"
#include "SYSTICK_interface.h"
#include "SPI_interface.h"

#include "TFT_interface.h"
#include "TFT_private.h"
#include "TFT_config.h"


void HTFT_voidInit(void)
{
	/* Reset pulse */
	MGPIO_voidSetPin(TFT_RESET_PIN) ;
	MSTK_voidSetBusyWait(100) ;
	
	MGPIO_voidClearPin(TFT_RESET_PIN) ;
	MSTK_voidSetBusyWait(1) ;
	
	MGPIO_voidSetPin(TFT_RESET_PIN) ;
	MSTK_voidSetBusyWait(100) ;
	
	MGPIO_voidClearPin(TFT_RESET_PIN) ;
	MSTK_voidSetBusyWait(100) ;
	
	MGPIO_voidSetPin(TFT_RESET_PIN) ;
	MSTK_voidSetBusyWait(120000) ;
	
	/* Sleep out command */
	voidWriteCommand(0X11);
	/* Wait 150 ms */
	MSTK_voidSetBusyWait(150000) ;
	/* Color mode command */
	voidWriteCommand(0X3A);
	voidWriteData(0X05) ;
	/* Display on command */
	voidWriteCommand(0X29) ;
	
}


void HTFT_voidDisplayImage(const u16 *Copy_Image)
{
	u16 Counnter = 0;
	/* Set X address */
	voidWriteCommand(0X2A) ;
	voidWriteData(0) ;
	voidWriteData(0) ;
	voidWriteData(0) ;
	voidWriteData(127) ;

	/* Set Y address */
	voidWriteCommand(0X2B) ;
	voidWriteData(0) ;
	voidWriteData(0) ;
	voidWriteData(0) ;
	voidWriteData(159) ;

	/* RAM write */
	voidWriteCommand(0X2C) ;
	
	for (Counnter=0 ; Counnter<20480 ; Counnter++)
	{
		/* Write the high byte */
		voidWriteData(Copy_Image[Counnter] >> 8) ;
		/* Write the low byte */
		voidWriteData((u8)Copy_Image[Counnter]) ;
	}

}



void HTFT_voidFillColour(u16 Copy_u16Colour)
{
	u16 Counnter = 0;
	/* Set X address */
	voidWriteCommand(0X2A) ;
	voidWriteData(0) ;
	voidWriteData(0) ;
	voidWriteData(0) ;
	voidWriteData(127) ;

	/* Set Y address */
	voidWriteCommand(0X2B) ;
	voidWriteData(0) ;
	voidWriteData(0) ;
	voidWriteData(0) ;
	voidWriteData(159) ;

	/* RAM write */
	voidWriteCommand(0X2C) ;

	for (Counnter=0 ; Counnter<20480 ; Counnter++)
	{
		/* Write the high byte */
		voidWriteData(Copy_u16Colour >> 8) ;
		/* Write the low byte */
		voidWriteData((u8)Copy_u16Colour) ;
	}

}



void HTFT_voidDrawRect(u8 x1 , u8 x2 , u8 y1 , u8 y2 ,u16 Copy_u16Colour)
{
	u16 Counnter = 0;
	u16 size = (x2 - x1) * (y2 - y1) ;
	/* Set X address */
	voidWriteCommand(0X2A) ;
	voidWriteData(0) ;
	voidWriteData(x1) ;
	voidWriteData(0) ;
	voidWriteData(x2) ;

	/* Set Y address */
	voidWriteCommand(0X2B) ;
	voidWriteData(0) ;
	voidWriteData(y1) ;
	voidWriteData(0) ;
	voidWriteData(y2) ;

	/* RAM write */
	voidWriteCommand(0X2C) ;

	for (Counnter=0 ; Counnter<size ; Counnter++)
	{
		/* Write the high byte */
		voidWriteData(Copy_u16Colour >> 8) ;
		/* Write the low byte */
		voidWriteData((u8)Copy_u16Colour) ;
	}

}



static void voidWriteCommand(u8 Copy_u8Command)
{
	u8 Local_u8Temp =0 ;
	/* set A0 pin to low */
	MGPIO_voidClearPin(TFT_A0_PIN) ;
	/* Send command over SPI */
	MSPI1_voidSendReceiveSynch(Copy_u8Command , &Local_u8Temp) ;
}


static void voidWriteData(u8 Copy_u8Data)
{
	u8 Local_u8Temp =0 ;
	/* set A0 pin to high */
	MGPIO_voidSetPin(TFT_A0_PIN) ;
	/* Send command over SPI */
	MSPI1_voidSendReceiveSynch(Copy_u8Data , &Local_u8Temp) ;
}
