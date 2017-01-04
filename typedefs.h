#ifndef _DEFINES_H
#define _DEFINES_H

#include <xc.h>
#include "cpuclock.h"     // #define X01MHz
#include <math.h>
//#include "delays.h"
#include "Load_detection.h"
#include "software_uart.h"
#include "flash.h"


#define POWER_PIN  LATAbits.LATA4
#define PULSE_PIN LATAbits.LATA5
#define DEBUG_LED LATAbits.LATA1
#define DEBUG_OUTPUT
//#define SLEEP_MODE   0x02

/*
 
 #ifdef DEBUG_OUTPUT
	
#endif
 */


#define Forver_loop while(1)

//typedef unsigned long int D_word; //32 bit
typedef unsigned int word;          // 16-bit
//typedef unsigned char byte;          // 8 -bits

word adc_read_10bits(void);
word adc_read2_10bits(void);
void resets (void);
char test_mode_operations (void);
void test_mode_sequencer (void);
void test_mode_engine (void);
void security_check(void);
void pulse_gen_mode(void); //test

unsigned char THREE_MINUTE_COUNTER=180;  ///should be 180
unsigned int SIX_MINUTE_COUNTER= 360;
unsigned int NINE_MINUTE_COUNTER =540;    //Should be 540

unsigned int adc_reading_PGD, PGD;     
unsigned int adc_reading_PGC, PGC;


volatile unsigned int Count1Min = 0;
volatile unsigned int Count2Min = 0;
volatile unsigned int Count6Min = 0;
volatile unsigned int Count3Min = 0;
volatile unsigned int Count9Min = 0;
volatile unsigned int Count10mSec = 0;




char test[],test_done,test_statedelay, i=0;
char test_state=0x00;
char dat=0x0000;
 

//unsigned char var1[31] @ 0x0780 = {1,2,3,4};
//const unsigned int table[] @ 0x07 = { 7,7,7,7 };

//rom char var;

 unsigned int x=0, z=0, a=0, b=0, c=0; 
//volatile char pulsecounter=0, y=0;
 

char retries=0;
char panel_state=0x00; //
char current_state = 0x00;

char *p;

	enum _POWER_PIN_STATUS             //POWER STATUS
	{
		LOW=0,HIGH

	}power_status;

	enum _POWER_STATUS        // POWER STATUS
	{
		OFF=0, ON, SLEEP_LEVEL

	};

   unsigned char read_buf[40]; //
   unsigned char write_buf[40];
    
   
   /*
#ifdef DEBUG_OUTPUT
	len = sprintf((char*)&scratchpad[0], (char*)"PGC ADC: %d", adc_reading);
	debug_write(scratchpad, len); // Write Debug
#endif
   */

 struct _FLAGS
{
   	unsigned security_pass : 1;
	unsigned load_detected : 1;
	unsigned one_second : 1;
    unsigned retries_timeout:1;
    unsigned test_mode:1;
    unsigned ten_msecs:1;
    unsigned one_minute:1;
    unsigned three_minute:1;
    unsigned six_minute:1;
    unsigned nine_minute:1;
    
	
} flags;

#endif
//