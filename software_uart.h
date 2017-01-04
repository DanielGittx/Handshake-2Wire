/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  


#ifndef _SOFTWARE_UART_H
#define _SOFTWARE_UART_H

#include "cpuclock.h"     // #define X04MHz
#include "typedefs.h"
#include "Load_detection.h"
#include "flash.h"


#define Baudrate              1200                      //bps
#define OneBitDelay           (1000000/Baudrate)
#define DataBitCount          8                         // no parity, no flow control
#define UART_RX               //PORTAbits.RA4						// UART RX pin
#define UART_TX               LATAbits.LATA0						// UART TX pin
#define UART_RX_DIR			  //TRISAbits.TRISA2					// UART RX pin direction register
#define UART_TX_DIR			  TRISAbits.TRISA0					// UART TX pin direction register
unsigned char digit1,digit2,digit3,digit4;
unsigned int mod1,mod2,mod3;
//Function Declarations
void InitSoftUART(void);
unsigned char UART_Receive(void);
void display_string(char* name);
//void UART_Transmit(const char);
void display_digits(unsigned int digits);
void UART_Transmit( unsigned char DataValue);
char scratchpad[32];
char len;//, adc_reading;
void debug_write(char* buff_ptr, char len);



#endif	/* XC_HEADER_TEMPLATE_H */


