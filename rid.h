/*
 * rid.h
 *
 * Created: 18.01.2021 16:40:47
 *  Author: Dennis
 */ 


#ifndef RID_H_
#define RID_H_

#include <avr/io.h>
#include "avr-quantizer/quantizer.h"

//////////////////////////////////////////////////////////////////////////
// Defines & Enums
//////////////////////////////////////////////////////////////////////////

// all resistor values for the functions are shifted 5 bits to the right (= divided by 32) in order
// to store "large" values in a uint16_t
#define RID_LUT_SCALE	5

#define RID_REF_OHM		(22000>>RID_LUT_SCALE)

// a total of 32 IDs, starting at 0. Use the number or alternatively the name below
typedef enum
{
	RID_SHORT,
	RID_1K0,
	RID_1K5,
	RID_2K2,
	RID_3K3,
	RID_4K7,
	RID_5K6,
	RID_6K8,
	RID_8K2,
	RID_10K,
	RID_12K,
	RID_15K,
	RID_18K,
	RID_22K,
	RID_27K,
	RID_33K,
	RID_39K,
	RID_47K,
	RID_56K,
	RID_68K,
	RID_82K,
	RID_100K,
	RID_120K,
	RID_150K,
	RID_180K,
	RID_220K,
	RID_270K,
	RID_330K,
	RID_470K,
	RID_680K,
	RID_1000K,
	RID_OPEN,
} rid_e;


//////////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////////

extern const __flash uint8_t lut16_rid[42];


//////////////////////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////////////////////

// returns the low or high side resistor value of a voltage divider, given that the other resistor
// and the measured ADC values are known
// param:
//	- adc_10b		adc value, must fit into 10 bit to guarantee arithmetic correctness
//	- res_*			other resistance value, must fit into 14 bit
uint16_t rid_res_low (uint16_t adc_10b, uint16_t res_high_14b);
uint16_t rid_res_high (uint16_t adc_10b, uint16_t res_low_14b);

// converts a resistor value into the respective ID
// param:
//	- res_scaled	resistance value, scaled down by 'RID_LUT_SCALE'
inline rid_e rid_get (uint16_t res_scaled) __attribute__((always_inline));


//////////////////////////////////////////////////////////////////////////
// Inline Functions
//////////////////////////////////////////////////////////////////////////

inline rid_e rid_get (uint16_t res_scaled)
{
	return quantizer_uint16(lut16_rid, res_scaled);
}


#endif /* RID_H_ */