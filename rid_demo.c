/*
 * rid_demo.c
 *
 * Created: 18.01.2021 16:36:29
 * Author : Dennis
 */ 

#define F_CPU	9600000

#include <avr/io.h>
#include <util/delay.h>

#include "rid.h"
#include "repos/avr-tinyuart/tinyuart.h"


typedef union
{
	uint16_t u;
	uint16_t u16[1];
	uint8_t u8[2];
} uint16_u;

int main(void)
{
	// clear cpu prescaler
	CLKPR	= (1<<CLKPCE);
	CLKPR	= (0b0000<<CLKPS0);

	PORTB	= (1<<3)|(1<<4);	// PB3/ADC3 is reference input; enable internal pullup and connect external 22k
								// PB4/ADC2 is continuous sampled input
	
	ADMUX	= (0b11<<MUX0);
	for (uint8_t cnt=64; cnt != 0; cnt--)
	{
		ADCSRA	= (1<<ADEN)|(1<<ADSC)|(0b110<<ADPS0);		// div 64 -> 150kHz
		while (ADCSRA & (1<<ADSC));							// wait for completion
	}
	
	uint16_t adc_ref = ADC;
	uint16_t res_high = rid_res_high(adc_ref, RID_REF_OHM);
	
	
	ADMUX	= (0b10<<MUX0);	// PB4/ADC2 is continuous sampled input
    while (1) 
    {
		ADCSRA	= (1<<ADEN)|(1<<ADSC)|(0b110<<ADPS0);		// div 64 -> 150kHz
		while (ADCSRA & (1<<ADSC));
		
		uint16_t adc_val = ADC;
		uint16_t res_low = rid_res_low(adc_val, res_high);
		
		rid_e rid = rid_get(res_low);
		
		// demo code: output intermediary values
// 		uint16_u adc = (uint16_u)adc_val;
// 		uint16_u out = (uint16_u)res_low;
// 		
// 		tinyuart_send_uint8(adc.u8[1]);
// 		_delay_us(30);
// 		tinyuart_send_uint8(adc.u8[0]);
// 		_delay_us(100);
// 		tinyuart_send_uint8(out.u8[1]);
// 		_delay_us(30);
// 		tinyuart_send_uint8(out.u8[0]);
// 		_delay_us(100);
		
		// demo code: output result
		tinyuart_send_uint8(rid);
		_delay_ms(500);
    }
}

