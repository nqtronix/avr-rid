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
} uint32_u;


//////////////////////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////////////////////

uint16_t rid_res (uint16_t dividend_10b, uint16_t divisor_10b, uint16_t res_low_12b);


//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

uint16_t rid_res_low (uint16_t adc_val, uint16_t res_high_12b)
{
	if (adc_val >= 1000)
		return UINT16_MAX;
	
	//return ((((uint32_t)adc_val<<10)/(ADC_MAX_VALUE-adc_val))*res_high_12b)>>10;
	
	return rid_res(adc_val, ADC_MAX_VALUE-adc_val, res_high_12b);
}

uint16_t rid_res_high (uint16_t adc_val, uint16_t res_low_12b)
{
	// prevent division through 0 edge case
	if (adc_val <= 11)
		return UINT16_MAX;

	//return ((((ADC_MAX_VALUE-(uint32_t)adc_val)<<10)/adc_val)*res_low_12b)>>10;
	
	return rid_res(ADC_MAX_VALUE-adc_val, adc_val, res_low_12b);
}

uint16_t rid_res (uint16_t dividend_10b, uint16_t divisor_10b, uint16_t res_12b)
{
	return (((((uint32_t)dividend_10b)<<10)/divisor_10b)*res_12b)>>10;
}