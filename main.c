#ifndef _CANARY_C
#define	_CANARY_C

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include "cpuclock.h"     // #define X04MHz
#include <math.h>
//#include "delays.h"
#include "Load_detection.h"
#include "typedefs.h"
#include "software_uart.h"
#include "HEflash.h"


#pragma config	CONFIG1 = 0x880;
#pragma config	CONFIG2 = 0xE03;
#define _XTAL_FREQ	4000000L        //CLK Frequency


void main(void);
void SysInit(void);
void interrupt ISR (void);
void security_check_lenient(void);
char scheduler (void);
char task_manager(void);


char min_sleep_level=0;


void main()
{	
	/* Set up the OSC and its frequency */

/*
	#ifdef	X01MHz	
		OSCCON	= 0x5A;	// SELECT OSCILLATOR FREQUENCY at 1 MHz	
	#endif	
*/
        
    #ifdef	X04MHz	
	OSCCON	= 0x6A;	// SELECT OSCILLATOR FREQUENCY at 4 MHz 	
	#endif	
	
	OSCSTAT = 0xFF;
	
	OSCCONbits.SCS1 = 1;	// Internal OSC block
	INTCON = 0x00;			// Disable interrupts
	PIE1 = 0x00;  // 
	PIE2 = 0x00;
//	PIE3 = 0x00;
	PIR1 = 0x00;
	PIR2 = 0x00;
//	PIR3 = 0x00;  



	/* Initialize system */	
	SysInit();                    //IOC is configured during this Init and GIE
  
	INTCONbits.PEIE = 1;		 // RE-enable peripheral interrupts
	INTCONbits.GIE = 1;			 // RE-enable interrupts
    
    IOCAF = 0x00;			     // Clear all IOC flags
    IOCIF = 0x00;
    
 //  INITIALIZING   
    
     timer0_config();             // Timer 0 setup
     InitSoftUART();              // SW Uart
     resets();                    // Global Flags 
     
     
 // INITIAL PANEL STATE - POWER_ON
     
     flags.security_pass=1;         //Dummy security check to start us off
     if(flags.security_pass==1)
       {
         flags.security_pass=0;
         power_on();
       }
    
     // TESTING REPO
    
     
    Forver_loop
    {
    
     //erase_operation();
     //write_operation (0x0780, 0x55, 1);
     //write_operation (0x0780, 0x55, 0);
     //__delay_ms(20);
      /*
        if(flags.one_second ==1)          // For Debugging
          {
            
             state_machine(PULSE_GENERATOR);
          // display_string("test\0");
          
           // p = Read_operation();
           // display_digits(*p);
            flags.one_second=0;
          }
        */
    /////////////////////////////////////////test mode churn//////////////////////////////////////////////////////////////    
     /*
        //test Mode churn
        i = test_mode_operations(); //
        if ( i < min_sleep_level ) min_sleep_level = i;      //
     */
        
      //////////////////////////scheduler churn/////////////////////////////////////////////////////////////////
     
      i = scheduler(); //
      {
        if ( i < min_sleep_level ) min_sleep_level = i;  
      }
      /////////////////////////task manager churn///////////////////////////////////////////////////////////////
      i = task_manager(); //
      {
        if ( i < min_sleep_level ) min_sleep_level = i;  
      }
     
    } 
        
}  


/*-----------------------------------------------------------------------------------------*/
/*  @fn         SysInit()
*   @param      None. 
*   @return     None.
*   @brief      Configure I/O's and  Interrupt on Change 
*               INPUTS:- RA0 - PGD, RA1 - PGC
*               INPUT:-  RA2 - INT (IoC) Pin
*               INPUT:-  RA3:-  MCLR  
                OUTPUTS:- RA4, RA5:-Pulse Generate and Power Pin Respectively  
*------------------------------------------------------------------------------------------*/

void SysInit(void )
{

	LATA = 0x00;
    TRISA = 0b00001111;      
    LATA = 0x00; 
    ANSELAbits.ANSA0=1;      //PGD - Analog Pin enabled
    ANSELAbits.ANSA1=1;      //PGC - Analog Pin enabled
    ANSELAbits.ANSA2=0;      //Analog Pin disabled
    ANSELAbits.ANSA4=0;      //Analog Pin disabled
    
    
    IOCAPbits.IOCAP2 = 1;    // Rising edge IOC enabled for RA2
    IOCANbits.IOCAN3 = 1; 	 // IOC Falling edge  enabled for RA3(Test mode))
    //IOCAPbits.IOCAP3 = 1;  // IOC Rising edge  enabled for RA3(Test mode))
	INTCONbits.IOCIE = 1;    // IOC Master Switch ON
	
   }
   volatile int pulse_counter = 0,pulse_counter_test_mode=0,y=0, test_mode_count=0;
/*-----------------------------------------------------------------------------------------*/
/*  @fn         interrupt ISR()
*   @param      None. 
*   @return     None.
*   @brief      Interrupts Routine
*------------------------------------------------------------------------------------------*/

void interrupt ISR(void) 
{
        
if (INTCONbits.IOCIF==1)
 {
    INTCONbits.IOCIF=0;
    
    if(IOCAFbits.IOCAF2 == 1)
    {
       IOCAFbits.IOCAF2 = 0;
       pulse_counter++;  
        if(pulse_counter>2)
            {

                   flags.security_pass=1;
                   security_check();        //Critical mission
                //display_digits(pulse_counter);
                   pulse_counter=0; 
            }
    }
    
    if(IOCAFbits.IOCAF3 == 1)              //Test mode flag
    {
       IOCAFbits.IOCAF3 = 0;
    
       test_mode_count++;  
    
        if(test_mode_count>0)                   //Just one count to get us into test mode
            {
               test_mode_count=0; 
               flags.test_mode=1;
            }
    }
    
}

     if (INTCONbits.TMR0IF == 1)		         	// TMR0 interrupt? 
        {
		    INTCONbits.TMR0IF = 0;			            // CLR interrupt
		     y++; // increment timer variable
          
		       if (y ==15)                 
		          {
                    flags.one_second = 1;                      //One Sec Flag set flag
                    // display_string("oneSEC\r");
                   	y = 0;                                //clear variable
		          }
	    } 


}
 /*-----------------------------------------------------------------------------------------*/
/*  @fn         pulse_gen_mode()
*   @param      None. 
*   @return     None
*   @brief      Echo pulses received from production board
*------------------------------------------------------------------------------------------*/
void pulse_gen_mode(void)
{
        char x=0;
        x=pulse_counter_test_mode;
        pulse_counter_test_mode =0;       //Reset this counter
   		for (char a=0; a<=x; a++)     //
		      {
	         	PULSE_PIN = HIGH;
	           	__delay_ms(1);
	         	PULSE_PIN = LOW;
	          	__delay_ms(1);
	          }
}
/*-----------------------------------------------------------------------------------------*/ 
/*  @fn        test_mode_operations()
*   @param      None. 
*   @return     char SLEEP_LEVEL
*   @brief      cover routine for sequencer and state machine functions
*------------------------------------------------------------------------------------------*/

char test_mode_operations (void)
{
     if(flags.test_mode==1)
        {
         flags.test_mode=0;
         test_mode_sequencer();    
         test_mode_engine();
        }
    return SLEEP_LEVEL;  
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         test_mode_sequencer()
*   @param      None. 
*   @return     None
*   @brief      Run sequence of events during test mode
*------------------------------------------------------------------------------------------*/

void test_mode_sequencer (void)
{
    switch(test_state)
        {
            case POWER_OFF:                    //Production Brd is Reading PGC
                test_state = POWER_ON;         //Sequencer kicks off
                             
                break;
            case POWER_ON:                     //Production Brd is Reading PGC
                test_state = LOAD_DETECTION;
               // test_done = 1;
                break;
            case LOAD_DETECTION:                     //Do nothing - Production Brd is Reading PGC&PGD
                //power_on();
               test_state = PULSE_GENERATOR;
              // test_done = 1;
                break;
            case PULSE_GENERATOR:                     
                 test_state = POWER_OFF;
              //   test_done = 1;
                 break;
            default:
                test_state = POWER_OFF;
        }
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         test_mode_engine()
*   @param      None. 
*   @return     None
*   @brief      Implements what's to be done during each test sate
*------------------------------------------------------------------------------------------*/
void test_mode_engine (void)
{
   switch(test_state)
        {
            case TEST_START:
                test_state = POWER_OFF;
                break;
            case POWER_OFF:                    //Production Brd is Reading PGC
                power_off();
                    
                break;
            case POWER_ON:                     //Production Brd is Reading PGC
                power_on();
                
                break;
            case LOAD_DETECTION:                     //Do nothing - Production Brd is Reading PGC&PGD
                
                break;
            case PULSE_GENERATOR:                     //
                 pulse_gen_mode();                  //sends what has been received from Production brd
                // test_done = 0;
                // test_statedelay = 1;
                 break;
            default:
                test_done = 0;
        } 
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         scheduler()
*   @param      None. 
*   @return     char SLEEP_LEVEL;
*   @brief      Keeps track of programs time/timing
*               Keep retrying to establish communication in case of failure 
*------------------------------------------------------------------------------------------*/
char scheduler (void)
{
    //char tries;
    
       if(flags.one_second ==1)
            {
              flags.one_second =0;
                     //  state_machine(PULSE_GENERATOR);
              
                   if(flags.retries_timeout==1)
                      {                       
                        ioc_disable();
                        power_off();
                        state_machine(PULSE_GENERATOR);
                        ioc_enable();
                        delay_seconds(5);
                        state_machine(POWER_ON);
                       
                      }
                        if(++Count1Min==60)
                          {
                             Count1Min=0;
                  
                             flags.one_minute=1;
                
                // display_string("oneMin\r");
                               if(++Count9Min==9)
                                  {
                                    Count9Min=0;
                                    flags.nine_minute=1;
                                  }
                 
                               if(++Count3Min==3)
                                  {
                                    Count3Min=0;
                                    flags.three_minute=1;
                     
                                    if(++Count6Min==2)
                                       {
                                         Count6Min=0;
                                         flags.six_minute=1;  
                                       }
                                  }
                          }
              
                
            }
  
 return SLEEP_LEVEL;         
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         task_manager()
*   @param      None. 
*   @return     char SLEEP_LEVEL;
*   @brief      implements all system activities routinely
*               sets retries flag incase of communication failure every 1,3,6 or 9Minutes as desired
*------------------------------------------------------------------------------------------*/
char task_manager(void)
{
  if(flags.one_minute==1)
        {
    
        flags.one_minute=0;
       /*
        ioc_disable();
        power_off();
        state_machine(PULSE_GENERATOR);
        ioc_enable();
        flags.retries_timeout=1;                 //TRY SET THIS FLAG AFTER COMM FAILURE IN SECURITY CHECK())
       */
         
        }
    
    
       while(flags.three_minute==1)
           {      
           
        flags.three_minute=0;          //Do nothing
        /*
        ioc_disable();
        power_off();
        state_machine(PULSE_GENERATOR);
        ioc_enable();
        flags.retries_timeout=1;
      
      // break;
        */
          }   
         while(flags.six_minute==1)
    {
        flags.six_minute=0;           //Do nothing
        break;
    }  
   
       if(flags.nine_minute==1)
    {
        flags.nine_minute=0;
      
        ioc_disable();
        power_off();
        state_machine(PULSE_GENERATOR);
        ioc_enable();
        flags.retries_timeout=1;
        
    }   
      
   return SLEEP_LEVEL;     
       
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         security_check()
*   @param      None. 
*   @return     char SLEEP_LEVEL;
*   @brief      Checks for successful communication i.e Have we received enough pulses after 9 Minutes?
*               If the above is true, allow charging else remain in pulse generation state
*------------------------------------------------------------------------------------------*/

void security_check(void)
{
 if(flags.security_pass==1)                          ///*&& (flags.retries_timeout==1) /*|| (flags.load_detected==1)*/
    {
        flags.security_pass=0;
        flags.retries_timeout=0;
        flags.load_detected=0;
        power_on();
    }
 else
 {
     flags.retries_timeout=1;
 }
// change for first commit

}

#endif  