/*
 * File:   flash.c
 * Author: danial
 *
 * Created on May 24, 2016, 9:38 AM
 */

#ifndef _FLASH_H
#define	_FLASH_H
//#include <xc.h>
//#include "cpuclock.h"     // #define X04MHz
#include "typedefs.h"
#include "Load_detection.h"
//#include <math.h>
#include "software_uart.h"
#include "flash.h"
//unsigned char flash_read(void );

unsigned char flash_read(void )
{
    // variable to read the data
   unsigned char buf[]=0;
    // Select COnfiguration Memory
    PMCON1bits.CFGS = 0;

    for(char i=0;i<=3;i++){
    // Select Word Address
    PMADRH = 0x80;
    PMADRL = 0x95+i;

    //Initiate read operation
    PMCON1bits.RD = 1;

    // do nothing
    _nop();
    _nop();
    // read the data
    
    buf[i] = ((PMDATH)<<8 | ((PMDATL)&0xFF));  //joins bytes & returns the value stored
    }
    return buf;
}
#endif