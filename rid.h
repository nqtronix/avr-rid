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
	RID_SHORT	= 0x00,		// 0
	RID_1K0		= 0x01,		// 1
	RID_1K5		= 0x02,		// 2
	RID_2K2		= 0x03,		// 3
	RID_3K3		= 0x04,		// 4
	RID_4K7		= 0x05,		// 5
	RID_5K6		= 0x06,		// 6
	RID_6K8		= 0x07,		// 7
	RID_8K2		= 0x08,		// 8
	RID_10K		= 0x09,		// 9
	RID_12K		= 0x0A,		// 10
	RID_15K		= 0x0B,		// 11
	RID_18K		= 0x0C,		// 12
	RID_22K		= 0x0D,		// 13
	RID_27K		= 0x0E,		// 14
	RID_33K		= 0x0F,		// 15
	RID_39K		= 0x10,		// 16
	RID_47K		= 0x11,		// 17
	RID_56K		= 0x12,		// 18
	RID_68K		= 0x13,		// 19
	RID_82K		= 0x14,		// 20
	RID_100K	= 0x15,		// 21
	RID_120K	= 0x16,		// 22
	RID_150K	= 0x17,		// 23
	RID_180K	= 0x18,		// 24
	RID_220K	= 0x19,		// 25
	RID_270K	= 0x1A,		// 26
	RID_330K	= 0x1B,		// 27
	RID_470K	= 0x1C,		// 28
	RID_680K	= 0x1D,		// 29
	RID_1000K	= 0x1E,		// 30
	RID_OPEN	= 0x1F,		// 31
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