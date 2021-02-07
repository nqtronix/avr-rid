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

//////////////////////////////////////////////////////////////////////////
// User Config
//////////////////////////////////////////////////////////////////////////

// enable raw data output of some internal values for debug
//#define RID_OPT_DBG_RAW_OUTPUT


//////////////////////////////////////////////////////////////////////////
// Device Setup
//////////////////////////////////////////////////////////////////////////

__fuse_t __fuse __attribute__((section (".fuse"))) =
{
	.low	= FUSE_SPIEN & /*FUSE_CKDIV8 &*/ FUSE_SUT0 & FUSE_CKSEL0,	// default w/ out 8x divider
	.high	= HFUSE_DEFAULT,
};


//////////////////////////////////////////////////////////////////////////
// Struct & Typedef
//////////////////////////////////////////////////////////////////////////

typedef union
{
	uint16_t u;
	uint16_t u16[1];
	uint8_t u8[2];
} uint16_u;


//////////////////////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////////////////////

// returns right adjusted 10b value from the adc
static uint16_t adc_get(uint8_t admux, uint8_t pinb);


//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

int main(void)
{
	// PB3/ADC3 is reference input; enable internal pullup and connect external 22k
	uint16_t adc_ref = adc_get(0b11<<MUX0, 1<<3);
	uint16_t res_high = rid_res_high(adc_ref, RID_REF);
	
    while (1) 
    {
		// PB4/ADC2 is continuous sampled input
		// PB4/ADC2 is continuous sampled input
		uint16_t adc_val = adc_get(0b10<<MUX0, 1<<4);
		uint16_t res_low = rid_res_low(adc_val, res_high);
		
		rid_e rid = rid_get(res_low);
		
		// demo code: output intermediary values; use this to measure 
		#ifdef RID_OPT_DBG_RAW_OUTPUT
			uint16_u adc = (uint16_u)adc_val;
			uint16_u out = (uint16_u)res_low;
			
			tinyuart_send_uint8(adc.u8[1]);
			_delay_us(30);
			tinyuart_send_uint8(adc.u8[0]);
			_delay_us(100);
			tinyuart_send_uint8(out.u8[1]);
			_delay_us(30);
			tinyuart_send_uint8(out.u8[0]);
			_delay_us(100);
		#endif
		
		// demo code: output result
		tinyuart_send_uint8(rid);
		_delay_ms(500);
    }
}

static uint16_t adc_get(uint8_t admux, uint8_t pinb)
{
	PINB	= pinb;
	ADMUX	= admux;
	ADCSRA	= (1<<ADEN)|(1<<ADSC)|(0b110<<ADPS0);		// div 64 -> 150kHz
	while (ADCSRA & (1<<ADSC));							// wait for completion
	PINB	= pinb;
			
	return ADC;
}