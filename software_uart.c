/*
 * File:   software_uart.c
 * Author: Mkopa
 *
 * Created on May 11, 2016, 3:49 PM
 */
#ifndef _SOFTWARE_UART_C
#define _SOFTWARE_UART_C

//#include <xc.h>
#include "cpuclock.h"     // #define X04MHz
#include "typedefs.h"
#include "Load_detection.h"
//#include <math.h>
#include "software_uart.h"

/*-----------------------------------------------------------------------------------------*/
/*  @fn        InitSoftUART()
*   @param      None. 
*   @return     None
*   @brief      Initialize Port for software Uart
*------------------------------------------------------------------------------------------*/

void InitSoftUART(void)		// Initialize UART pins to proper values
{
	UART_TX = 1;			// TX pin is high in idle state
	
	//UART_RX_DIR = 1;		// Input
	UART_TX_DIR = 0;		// Output
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         UART_Transmit()
*   @param      const char DataValue. 
*   @return     None
*   @brief       1 Byte transmission
*------------------------------------------------------------------------------------------*/
void UART_Transmit( const char DataValue)
{
	/* Basic Logic
	   
	   TX pin is usually high. A high to low bit is the starting bit and 
	   a low to high bit is the ending bit. No parity bit. No flow control.
	   BitCount is the number of bits to transmit. Data is transmitted LSB first.

	*/

	// Send Start Bit
    
	UART_TX = 0;
	__delay_us(OneBitDelay);

    unsigned char i;
	for (i = 0; i < DataBitCount; i++ )
	{
		//Set Data pin according to the DataValue
		if( ((DataValue>>i)&0x1) == 0x1 )   //if Bit is high
		{
			UART_TX = 1;
		}
		else      //if Bit is low
		{
			UART_TX = 0;
		}

	    __delay_us(OneBitDelay);
	}

	//Send Stop Bit
	UART_TX = 1;
	__delay_us(OneBitDelay);
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         display_digits()
*   @param      unsigned int digits. 
*   @return     None
*   @brief      4 Bytes transmission
 *              build to transmit 10bit ADC VALUE
*------------------------------------------------------------------------------------------*/

void display_digits(unsigned int digits)
	{
  //char str[4];

    //sprintf(str,"%d",digits);
		unsigned int millivolts = digits;

		digit1 = millivolts/1000;
		mod1 = millivolts%1000;
		digit2 = mod1/100;
		mod2 = mod1%100;
		digit3 = mod2/10;
		digit4 = mod2%10;
		//digit1 = str[0];
		//digit2 = str[1];
		//digit3 = str[2];
		//digit4 = str[3];

				UART_Transmit(digit1);
				UART_Transmit(digit2);
				UART_Transmit(digit3);
				UART_Transmit(digit4);
				//delay_seconds(4);*/
	}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         display_string()
*   @param      char* name 
*   @return     None
*   @brief      Transmits strings
 *              
*------------------------------------------------------------------------------------------*/
void display_string(char* name)
{
  /*
	for (int i=0; i<8; i++)
	{
		UART_Transmit(name[i]);       
		if (name[i]=='\r')
		{
			UART_Transmit('\n');     send String to BT. Terminate all sent data with a \n
		}
	}
   */
     
     char length;
     for(length=0; name[length]!='\0'; length++)        //Terminating character \0
       {
          if(length=='\0')
             {
                UART_Transmit('\n'); 
             }
          else
             {
                UART_Transmit(name[length]); 
             }
       }
}
     

#endif