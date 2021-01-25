/*
 * rid_demo.c
 *
 * Created: 18.01.2021 16:36:29
 * Author : Dennis
 */ 

#include <avr/io.h>

#include "rid.h"

int main(void)
{
	// clear cpu prescaler
	CLKPR	= (1<<CLKPCE);
	CLKPR	= (0b0000<<CLKPS0);

	PORTB	= (1<<4);	// PB4/ADC2 is reference input; enable internal pullup and connect external 22k
						// PB3/ADC3 is continuous sampled input
	
	ADMUX	= (0b10<<MUX0);
	ADCSRA	= (1<<ADEN)|(1<<ADSC)|(0b110<<ADPS0);		// div 64 -> 150kHz
	while (ADCSRA & (1<<ADSC));							// wait for completion
	
	uint16_t adc_ref = ADC;
	uint16_t res_high = rid_res_high(adc_ref, RID_REF_OHM);
	
	
	ADMUX	= (0b11<<MUX0);	// PB3/ADC3 is continuous sampled input
    while (1) 
    {
		ADCSRA	= (1<<ADEN)|(1<<ADSC)|(0b110<<ADPS0);		// div 64 -> 150kHz
		while (ADCSRA & (1<<ADSC));
		
		uint16_t adc_val = ADC;
		uint16_t res_low = rid_res_low(adc_val, res_high);
		
		rid_e rid = rid_get(res_low);
    }
}

