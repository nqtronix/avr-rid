/*
 * rid_config.h
 *
 * Created: 22.03.2021 22:57:36
 *  Author: Dennis (keybase.io/nqtronix)
 */ 


#ifndef RID_CONFIG_H_
#define RID_CONFIG_H_


// Do not change this line.
// If a newer version is incompatible with your custom config file
#define RID_VERSION_CONFIG		0


//////////////////////////////////////////////////////////////////////////
// User Config
//////////////////////////////////////////////////////////////////////////

// define the external dull-down reference resistor value, scaled to match the LUT
#define RID_REF_OHM			(24200)

// define the parasitic internal series resistor between ADC input and device output. Set to 0 if
// an external voltage divider is used (internal pullup disabled).
// This parasitic resistance is not specified or even mentioned in the datasheet, but can be measured indirectly by enabeling the
// internal pullup, shorting the pin externally to ground, reading the ADC value and calculating
// the apparent resistance value. On the device I used (attiny13a) it was 128-352 Ohms, chosen
// calibration value delivered consistent results in experiments.
// Do not confuse this with the ADC offset error, if an external 0V is applied without the internal
// pullup the reading is correct.
#define RID_PARASITIC_OHM	(320)


#endif /* RID_CONFIG_H_ */