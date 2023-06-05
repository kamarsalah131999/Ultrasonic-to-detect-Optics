/*
 * Ultrasonic sensor.c
 *
 * Created: 6/5/2023 1:56:10 AM
 * Author : kamar
 */ 

#include <avr/io.h>
#include "macros.h"
#include "LCD driver.h"
#define F_CPU 1000000UL
#include <util/delay.h>

int main(void)
{
    unsigned short a,b,c, high,distance;
	LCD_vinit ();
	DIO_vset_pin_dir('D', 7, 1);                                               //conect triger with PD7
	
	while (1) 
    {                         
	
	SET_BIT(TIFR,ICF1);                                                      // clear flag of ICU "we work pooling not interruput for this we clear flag by hand"
	DIO_vwrite_pin('D', 7, 1);                                              // transimit clock to trig pin
	_delay_us(50);
    DIO_vwrite_pin('D', 7, 0); 
	
	SET_BIT(TCCR1B,ICES1);                                                 //rising edge 
	SET_BIT(TCCR1B,ICNC1);                                                // noise clear
	SET_BIT(TCCR1B,CS10);                                                // no predcaler >> freauancy of microcontroller
	 
	while (READ_BIT(TIFR,ICF1)==0);                                     // mean capture action dont happen yet
	a=ICR1;                                                            // take value of capture register
	SET_BIT(TIFR,ICF1);                                               // clear flag of ICU 
	
	CLEAR_BIT(TCCR1B,ICES1);                                          //falling  edge 
	SET_BIT(TCCR1B,ICNC1);                         
	SET_BIT(TCCR1B,CS10);                          
    
	while (READ_BIT(TIFR,ICF1)==0);                
	b=ICR1;                                                        // take value of capture register
	SET_BIT(TIFR,ICF1);                            
	
	//stop timer
	CLEAR_BIT(TCCR1B,ICES1);                         
	CLEAR_BIT(TCCR1B,ICNC1);                         
	CLEAR_BIT(TCCR1B,CS10); 
	
	high=b-a;                                     
	distance= (high*34600)/(F_CPU*2);                          // time of tick = high/F_CPU
	
	if (distance>=80)
	{
		LCD_vclear_screen ();
		LCD_vsend_sring ("No Object");
		_delay_ms(500);
	}
	else
	{
		LCD_vclear_screen ();
		LCD_vsend_sring ("distance=");
		LCD_vsend_char ((distance/10)+48);
		LCD_vsend_char ((distance%10)+48);
		LCD_vsend_sring ("cm");
		_delay_ms(500);
	}
	
    
    }
}

