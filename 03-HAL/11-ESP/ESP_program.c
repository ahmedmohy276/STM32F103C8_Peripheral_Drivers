/*****************************************/
/* Author  :  Ahmed Mohy                 */
/* Date    :  20 January 2022             */
/* Version :  V01                        */
/*****************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "USART_interface.h"

#include "ESP_interface.h"
#include "ESP_private.h"
#include "ESP_config.h"


void HESP_voidSendCommand(u8 *Copy_pu8Command) 
{
	u8 Local_u8Response = 0;
	while(Local_u8Response == 0)
	{
		/* Disable Echo */
		MUSART1_voidTransmitSynch(Copy_pu8Command) ;
		Local_u8Response = ValidateResponse() ;
	}
}


void HESP_voidInit(void) 
{
	/* Disable Echo */
	HESP_voidSendCommand((u8*)"ATE0\r\n") ;
	
	/* Set ESP To Station Mode */
	HESP_voidSendCommand((u8*)"AT+CWMODE=1\r\n") ;
}


u16 HESP_u8SendReceiveData(u8 *Local_pu8RecData) 
{
	HESP_voidSendCommand((u8*)"AT+CIPSEND=40") ;
	u8 Local_u8Response = 0;
	u8 Local_u8Length = 0;
	u8 ReceiveResponse [1000] = {0} ;
	u8 Local_u8Validation = 0;
	u8 Local_u8Index = 0;
	u8 Dummy = 0;
	while(Local_u8Response == 0)
	{
		MUSART1_voidTransmitSynch((u8*)"GET http://mohy.freevar.com/status.txt") ;
		while(Dummy !=255)
		{
			Dummy = MUSART1_u8ReceiveSynch() ;
			ReceiveResponse[Local_u8Index] = Dummy ;
			Local_u8Index++ ;
		}
		
		if ( (ReceiveResponse[20] == 'O') && (ReceiveResponse[21] == 'K') )
		{
			Local_u8Validation = 1 ;
		}
	}
	
	Local_u8Length = ReceiveResponse[29] ;
	for (Local_u8Index=0 ; Local_u8Index < Local_u8Length ; Local_u8Index++)
	{
		Local_pu8RecData[Local_u8Index] = ReceiveResponse[Local_u8Index + 31] ;
	}
	
	return Local_u8Length ;
}


void HESP_voidWiFiConnect(void) 
{
	HESP_voidSendCommand((u8*)"AT+CWJAP_CUR=\"Honor_8x\",\"01065121188Mohy#\"") ;
}


void HESP_voidServerConnect(void) 
{
	HESP_voidSendCommand((u8*)"AT+CIPSTART=\"TCP\",\"162.253.155.226\",80") ;
}


static u8 ValidateResponse(void)
{
	u8 ReceiveResponse [100] = {0} ;
	u8 Local_u8Validation = 0;
	u8 Local_u8Index = 0;
	u8 Dummy = 0;
	while(Dummy !=255)
	{
		Dummy = MUSART1_u8ReceiveSynch() ;
		ReceiveResponse[Local_u8Index] = Dummy ;
		Local_u8Index++ ;
	}
	
	if ( (ReceiveResponse[0] == 'O') && (ReceiveResponse[1] == 'K') )
	{
		Local_u8Validation = 1 ;
	}
	
	else if ( (ReceiveResponse[28] == 'O') && (ReceiveResponse[29] == 'K') )
	{
		Local_u8Validation = 1 ;
	}
	
	else if ( (ReceiveResponse[9] == 'O') && (ReceiveResponse[10] == 'K') )
	{
		Local_u8Validation = 1 ;
	}
	else 
	{
		Local_u8Validation = 0 ;
	}
	return Local_u8Validation ;
}

