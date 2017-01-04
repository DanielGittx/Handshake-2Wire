#ifndef _LOAD_DETECTION_H
#define _LOAD_DETECTION_H

//#include <delays.h>
#include "cpuclock.h"
#include "typedefs.h"
#include "software_uart.h"
//#include "flash.h"
           
/*
#ifdef	X01MHz
#define _XTAL_FREQ	1000000L
#endif

#ifdef	X02MHz
#define _XTAL_FREQ	2000000L
#endif
*/
#ifdef	X04MHz
#define _XTAL_FREQ	4000000L
#endif

/*      
#ifdef	X08MHz
#define _XTAL_FREQ	8000000L
#endif
 * */


#define POWER_OFF   0x04
#define POWER_ON    0x06
#define PULSE_GENERATOR   0x08
#define SECURITY_CHECK   0x10
#define LOAD_DETECTION 0x12
#define TEST_START 0x14
 unsigned char num_retries = 0;
 unsigned char start_retry_indicator =0;
// char z=0;
void delay_x_us(char delay);
void delay_x_ms(char delay);
void delay_seconds(unsigned int seconds);

//unsigned char flash_read(void);
void delay_seconds();
void adc_setup(char adc_channel);

void load_detection (void);
void state_machine(char state);
void power_on(void);
void power_off(void);
void pulse_generator(void);
void timer0_config(void);
void  ioc_enable(void);
void  ioc_disable(void);

//void security_check(void);
#endif

