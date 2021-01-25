/*
 * rid.c
 *
 * Created: 18.01.2021 16:40:35
 *  Author: Dennis
 */ 

#include "rid.h"
#include "avr-quantizer/quantizer.h"

//////////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////////

#define ADC_MAX_VALUE 1023

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
// Functions
//////////////////////////////////////////////////////////////////////////

uint16_t rid_res_low (uint16_t adc_val, uint16_t res_high_12b)
{
	if (adc_val >= 1000)
		return UINT16_MAX;
	
	uint32_t val = ((adc_val<<10)/(ADC_MAX_VALUE-adc_val))*res_high_12b;
	
	return val>>10;
}

uint16_t rid_res_high (uint16_t adc_val, uint16_t res_low_12b)
{
	// prevent division through 0 edge case
	if (adc_val <= 11)
		return UINT16_MAX;
	
	uint32_t val = (((ADC_MAX_VALUE-adc_val)<<10)/adc_val)*res_low_12b;
	
	return val>>10;
}

// converts a resistor value into the respective ID
rid_e rid_get (uint16_t res_scaled)
{
	return quantizer_uint16(lut16_rid, res_scaled);
}