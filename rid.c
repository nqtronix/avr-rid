/*
 * rid.c
 *
 * Created: 18.01.2021 16:40:35
 *  Author: Dennis
 */ 

#include "rid.h"

//////////////////////////////////////////////////////////////////////////
// Defines & Enums
//////////////////////////////////////////////////////////////////////////

#define ADC_MAX_VALUE 1023


//////////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////////

const __flash uint8_t lut16_rid[42] =
{
	// header
	10, 18, 22, 25, 29, 32, 36, 38, 40, 41,

	// data
	20, 38, 57, 87, 126, 155, 189, 228,
	139, 167, 208, 251,
	153, 188, 230,
	136, 164, 196, 237,
	143, 174, 209,
	129, 156, 190, 233,
	142, 195,
	139, 203,
	169,
	255
};


//////////////////////////////////////////////////////////////////////////
// Struct & Typedef
//////////////////////////////////////////////////////////////////////////

typedef union
{
	uint32_t u;
	uint32_t u32[1];
	uint16_t u16[2];
	uint8_t u8[4];
	struct  
	{
		uint32_t		: 8;
		uint32_t u16m	:16;
		uint32_t		: 8;
	};
} uint32_u;


//////////////////////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////////////////////

uint16_t rid_res (uint16_t dividend_10b, uint16_t divisor_10b, uint16_t res_low_12b);


//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

uint16_t rid_res_low (uint16_t adc_val, uint16_t res_high_14b)
{
	return rid_res(adc_val, ADC_MAX_VALUE-adc_val, res_high_14b);
}

uint16_t rid_res_high (uint16_t adc_val, uint16_t res_low_14b)
{
	return rid_res(ADC_MAX_VALUE-adc_val, adc_val, res_low_14b);
}

uint16_t rid_res (uint16_t dividend_10b, uint16_t divisor_10b, uint16_t res_14b)
{
	// catch division by 0. This is possible, as the divisor is the adc value
	if (divisor_10b == 0)
		return UINT16_MAX;	
	
	// The initial formula is quite simple, but not efficiently calculated on a 8bit MCU without FPU
	// 1) result = res_14b * (dividend_10b / divisor_10b)
	
	// A normal integer division would not work here because the quotient is close to "1" in most
	// cases. To solve this, the resistance can be first multiplied with the dividend:
	// 2) result = (res_14b * dividend_10b) / divisor_10b
	
	// However, if the resistance value is small (less than 10 bits) the division will add additional
	// error. To solve this the dividend can be shifted left (= multiplied by 2^n) and after the
	// division shifted right (= divided by 2^n).
	// 3) result = ( ((res_14b * dividend_10b)<<n) / divisor_10b) >>n
	
	// On AVR8 MCUs shifting by multiple bits is not possible with a single instruction, so the
	// compiler is forced to insert a software implementation which will increase code size. Instead
	// of shifting the value, it is much more effective to to access the 2 center bits of a uint32_t
	// directly.
	
	uint32_u tmp;
	tmp.u16m	= res_14b;
	tmp.u		*= dividend_10b;
	tmp.u		/= divisor_10b;
	
	// check overflow
	if (tmp.u8[3] != 0)
		return UINT16_MAX;
		
	return tmp.u16m;
}