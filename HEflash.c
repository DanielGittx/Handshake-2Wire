/*
 * File:   HEflash.c
 * Author: Mkopa
 *
 * Created on July 20, 2016, 12:11 PM
 */
#ifndef _HEflash_C
#define _HEflash_C

#include "Load_detection.h"
#include "cpuclock.h"
#include "typedefs.h"

#include "HEflash.h"


/*-----------------------------------------------------------------------------------------*/
/*  @fn         unlock_sequence()
*   @param      None. 
*   @return     none 
*   @brief      flash unlock sequence      
*------------------------------------------------------------------------------------------*/

void unlock_sequence (void)
{
	#asm
	BANKSEL PMCON2
	MOVLW 0x55
	MOVWF PMCON2 & 0x7F
	MOVLW 0xAA
	MOVWF PMCON2 & 0x7F
	BSF PMCON1 & 0x7F,1 ; set WR bit
	NOP
	NOP
	#endasm
}


/*-----------------------------------------------------------------------------------------*/
/*  @fn         flashRead()
*   @param      None. 
*   @return     unsigned 
*   @brief      Read flash,      
*------------------------------------------------------------------------------------------*/

char *Read_operation(void)
{
    unsigned char data=0;
    char read_buffer[32];
    
    
    // Select Word Address
    PMADR = 0x0780;
    
    // Select Flash Memory
    PMCON1bits.CFGS = 0;

    //Initiate read operation
    PMCON1bits.RD = 1;
    // do nothing
    _nop();
    _nop();
    
    // read the data
    //data = ((PMDATH)<<8 | ((PMDATL)&0xFF));  //joins hi-lo and return a word
    for (char length=0; length<32; length++)
       {
         read_buffer[length] = PMDATL; 
       }
   return read_buffer;

}

/*-----------------------------------------------------------------------------------------*/
/*  @fn         write_operation()
*   @param      unsigned address:- Flash location to write 
*               char data:- data to be written
*               char latch:-  1 = latch, 0 = write row
*   @return     none 
*   @brief      copy to flash      
*------------------------------------------------------------------------------------------*/

void write_operation (unsigned address, char data, char latch)
{
	// 1. disable interrupts (remember setting)
	char temp = INTCONbits.GIE;
	INTCONbits.GIE = 0;
	// 2. load the address pointers
	PMADR = address;
	PMDAT = data;
	PMCON1bits.LWLO = latch; // 1 = latch, 0 = write row
	PMCON1bits.CFGS = 0; // select the Flash address space
	PMCON1bits.FREE = 0; // next operation will be a write
	PMCON1bits.WREN = 1; // enable Flash memory write/erase
	// 3. perform unlock sequence
	unlock_sequence();
	// 4. restore interrupts
	if (temp)
		INTCONbits.GIE = 1;
}
/*

void write_operation (unsigned address, char *data, char latch)
{
	// 1. disable interrupts (remember setting)
	char temp = INTCONbits.GIE;
	INTCONbits.GIE = 0;
	
	 for (char length = 0; length < 5; length++)        //15 Bytes Write 
	 {
	// 2. load the address pointers
	PMADR = address;
	PMDAT = data[length];
	PMCON1bits.LWLO = latch; // 1 = latch, 0 = write row
	PMCON1bits.CFGS = 0; // select the Flash address space
	PMCON1bits.FREE = 0; // next operation will be a write
	PMCON1bits.WREN = 1; // enable Flash memory write/erase
	// 3. perform unlock sequence
	unlock_sequence();
     }
	 
	 // 4. restore interrupts
	if (temp)
		INTCONbits.GIE = 1;

}
 */
/*-----------------------------------------------------------------------------------------*/
/*  @fn         erase_operation()
*   @param      none 
*   @return     none 
*   @brief      copy to flash      
*------------------------------------------------------------------------------------------*/

void erase_operation(void)
{
    //Disable Global Interrupts
    INTCONbits.GIE = 0; 
    //Select Flash memory space
    PMCON1bits.CFGS = 0;
    //Select Erase Address
    PMDAT = 0x0780;
    //Enable Erase operation
    PMCON1bits.FREE = 1;
    //Enable write erase/operation
    PMCON1bits.WREN=1;
    // Unlock Sequence
    unlock_sequence();
    //Disable write erase/operation
    PMCON1bits.WREN=0;
    //Enable Global Interrupts
    INTCONbits.GIE= 1; 
    
}
     

void flashWrite_Op (unsigned address, char data, char latch0,char latch1)
   {
    for(char x; x<=20; x++)
    {
     erase_operation();
     write_operation(address, data, latch1);
     write_operation(address, data, latch0);
      __delay_ms(20);
    }
   }




#endif