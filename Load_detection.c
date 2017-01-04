#ifndef _LOAD_DETECTION_C
#define _LOAD_DETECTION_C

#include <xc.h>
#include <math.h>
#include "Load_detection.h"
#include "cpuclock.h"
#include "typedefs.h"



/*-----------------------------------------------------------------------------------------*/
/*  @fn         delay_x_us()
*   @param      unsigned char delay. 
*   @return     None
*   @brief      Create microsecond delay
*------------------------------------------------------------------------------------------*/


void delay_x_us(unsigned char delay)
{
	unsigned char x = delay;
	while (x--)
	{
		__delay_us(1);
	}
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         delay_x_ms()
*   @param      unsigned char delay. 
*   @return     None
*   @brief      Create millisecond delay
*------------------------------------------------------------------------------------------*/

void delay_x_ms(unsigned char delay)
{
	unsigned char x = delay;
	while (x--)
	{
		__delay_ms(1);
	}	
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         delay_seconds()
*   @param      unsigned int seconds. 
*   @return     None
*   @brief      Create 1 Second delay
*------------------------------------------------------------------------------------------*/
void delay_seconds(unsigned int seconds)
	{
		while(seconds)
			{
				seconds--;
				__delay_ms(250);
				__delay_ms(250);
				__delay_ms(250);
				__delay_ms(250);		
			}
	}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         resets()
*   @param      None. 
*   @return     None
*   @brief      Reset all flags
*------------------------------------------------------------------------------------------*/
void resets (void)
{
flags.security_pass=0;
flags.load_detected=0;
flags.one_second=0;
flags.retries_timeout=0;
flags.test_mode=0;
flags.one_minute=0;
flags.ten_msecs=0;

}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         adc_setup()
*   @param      char adc_channel. 
*   @return     None
*   @brief      Configure ADC
*------------------------------------------------------------------------------------------*/

void adc_setup(char adc_channel)
{
	/** Port Configuration - TRIS and ANSEL; done in SysInit routine **/
	/** ADC conversion clock source	**/

	#ifdef	X01MHz
	ADCON1 = 0x00; 		// Fosc/2
	#endif
	
	#ifdef	X02MHz
	#error No appropriate ADC clock period forr 2 MHz Fosc
	#endif
	
	#ifdef	X04MHz
	ADCON1 = 0x40;		// Fosc/4
	#endif
	
	#ifdef	X08MHz
	ADCON1 = 0x10;		// Fosc/8
	#endif
	
	#ifdef	X16MHz
	ADCON1 = 0x50;	 	// Fosc/16
	#endif
	
	/** ADC (+ve) voltage reference selection	**/
	ADCON1bits.ADPREF1 = 0;	// Vref+ = VDD
	ADCON1bits.ADPREF0 = 0;	
	
	ADCON1bits.ADFM = 0;	// Result Left Justified;Six Least Significant bits of ADRESL are set to ‘0’
	/** Channel Selection **/
	__delay_ms(10);	
   	ADCON0bits.CHS = adc_channel;			// Set Channel
	__delay_ms(30);
	/** ADC module enable **/
	ADCON0bits.ADON = 1; 	// ADC module enabled
	/**	Interrupt Control **/
	PIR1bits.ADIF = 0;		// CLR ADC interrupt flag
	__delay_ms(20);
	/** Start AD conversion cycle **/
	ADCON0bits.GO_nDONE = 1; // Start AD conversion cycle

}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         adc_read_10bits()
*   @param      char adc_channel. 
*   @return     word  adc result 
*   @brief      ADC conversion and return result
*------------------------------------------------------------------------------------------*/
word adc_read_10bits (void)
{

	adc_setup(0x00);  //adc channel 0
		
	while (ADCON0bits.GO_nDONE);  	// Wait till AD convERSION IS complete
	
	if (ADCON1bits.ADFM == 0)
	{

		adc_reading_PGD =  ADRESH;
		adc_reading_PGD <<= 2;
		adc_reading_PGD |=ADRESL >> 6;

            PIR1bits.ADIF = 0;		// CLR ADC interrupt flag
			ADCON0bits.ADON = 0; 	// ADC module disabled
    }
			return adc_reading_PGD;
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         adc_read2_10bits()
*   @param      char adc_channel. 
*   @return     word  adc result 
*   @brief      ADC conversion and return result
*------------------------------------------------------------------------------------------*/
word adc_read2_10bits (void)
{

	adc_setup(0x01);  //adc channel 1
		
	while (ADCON0bits.GO_nDONE);  	// Wait till AD convERSION IS complete
	
	if (ADCON1bits.ADFM == 0)
	{

		adc_reading_PGC =  ADRESH;
		adc_reading_PGC <<= 2;
		adc_reading_PGC |=ADRESL >> 6;

            PIR1bits.ADIF = 0;		// CLR ADC interrupt flag
			ADCON0bits.ADON = 0; 	// ADC module disabled
    }
			return adc_reading_PGC;
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         load_detection()
*   @param      None. 
*   @return     None 
*   @brief      load connection status
*------------------------------------------------------------------------------------------*/
void load_detection (void)
{
	
	//PGD = adc_read_10bits();
	//PGC = adc_read2_10bits();
	
	if ((PGC>=600)&&(PGC<=800))  //load detected?           
	{
        //panel_state = POWER_ON;
		flags.load_detected=1;
	 	
	}
}	
/*-----------------------------------------------------------------------------------------*/
/*  @fn         state_machine()
*   @param      char state. 
*   @return     None 
*   @brief      Implement operation mode states
*------------------------------------------------------------------------------------------*/
void state_machine(char state)
{
panel_state = state;
switch (panel_state)
{
case POWER_OFF:
power_off();
break;

case POWER_ON:
power_on();
break;

case PULSE_GENERATOR:

pulse_generator();
    
break;

case SECURITY_CHECK:
security_check();
break;
        
default:
power_on();

}

}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         power_on()
*   @param      None. 
*   @return     None 
*   @brief      Switch on circuit (through RA4) to allow charging 
*------------------------------------------------------------------------------------------*/
void power_on(void)
{
    
	PULSE_PIN = LOW;             //Pulse line LOW during Power On 
    POWER_PIN = 1;
    delay_seconds(1);
    
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         power_off()
*   @param      None. 
*   @return     None 
*   @brief      Switch off circuit (through RA4) to stop charging 
*------------------------------------------------------------------------------------------*/
void power_off(void)
{
    panel_state = POWER_OFF;
	POWER_PIN = 0;
   
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         pulse_generator()
*   @param      None. 
*   @return     None 
*   @brief      create 5 pulses 10x
*               50% duty cycle Pulses 
 *              10mS delay to allow reception of pulses from Ibis-2      
*------------------------------------------------------------------------------------------*/
void pulse_generator (void) 
{
      unsigned char a, tries=0;  
     // for(tries=0;tries<10;tries++)    //Looping 10 times
      // {
        
       //  __delay_ms(1);              //Takes care of synchronization with the other communication device
          //delay_seconds(1);             //Every 1 Sec
         
		  for (a=0; a<5; a++)           //
		      {
		        PULSE_PIN = HIGH;
	         	__delay_ms(1);
		        PULSE_PIN = LOW;
		        __delay_ms(1);
                
              }
         //delay_seconds(1);
         // __delay_ms(10);
       
	  // }

}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         ioc_disable()
*   @param      None. 
*   @return     None 
*   @brief      Disable Interrupt on change     
*------------------------------------------------------------------------------------------*/
void ioc_disable(void)
{
   IOCAPbits.IOCAP2 = 0;
   //IOCANbits.IOCAN2 = 0;
   INTCONbits.IOCIE = 0;
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         ioc_enable()
*   @param      None. 
*   @return     None 
*   @brief      Enable Interrupt on change     
*------------------------------------------------------------------------------------------*/
void  ioc_enable(void)
{
   IOCANbits.IOCAN = 1;
   IOCAPbits.IOCAP2 = 1;
   INTCONbits.IOCIE = 1;
}
/*-----------------------------------------------------------------------------------------*/
/*  @fn         timer0_config()
*   @param      None. 
*   @return     None 
*   @brief      Driver for Timer 0     
*------------------------------------------------------------------------------------------*/
void timer0_config(void)
{
	/* */
	INTCONbits.TMR0IE = 1; 		// Enable TMR0 interrupt
	OPTION_REGbits.TMR0CS = 0; 	// TMR0 uses internal CLK cycle
	OPTION_REGbits.PSA = 0; 	// Prescaler assigned to TMR0
	OPTION_REGbits.PS2 = 1;		// 1:256 prescaler
	OPTION_REGbits.PS1 = 1;
	OPTION_REGbits.PS0 = 1;
	INTCONbits.TMR0IF = 0;		// CLR flag
	TMR0 = 0x00;				// Reload value
	
}


#endif



