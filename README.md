<h1 align="center" style="font-weight: bold; margin-top: 20px; margin-bottom: 20px;">avr-rid</h1>

<h4 align="center">Identify plug-in modules or user settings with a single resistor.</h4>

<p align="center">
	<a href="#changelog"><img src="https://img.shields.io/github/release-pre/nqtronix/avr-rid.svg" alt="release: NA"></a>
    <a href="#about"><img src="https://img.shields.io/badge/language-C_(GCC_5.4.0)-blue.svg" alt="language: C GCC (5.4.0)"></a>
    <a href="#about"><img src="https://img.shields.io/badge/platform-AVR8-blue.svg" alt="platform: AVR8"></a>
	<a href="#about"><img src="https://img.shields.io/badge/status-maintained-green.svg" alt="status: maintained"></a>
	<a href="https://github.com/nqtronix/avr-rid/issues"><img src="https://img.shields.io/github/issues/nqtronix/avr-rid.svg" alt="issues: NA"></a>
	<a href="#license"><img src="https://img.shields.io/github/license/nqtronix/avr-rid.svg" alt="license: NA"></a>
</p>

<p align="center">
  <a href="#getting-started">Getting Started</a> •
  <a href="#documentation">Documentation</a> •
  <a href="#under-the-hood">Under the Hood</a> •
  <a href="#support">Need Help?</a> •
  <a href="#about">About</a> •
  <a href="#credits-and-references">Credits</a>
</p>



## Introduction
Despite the great flexibility that MCUs offer, configuration changes typically require a programmer and software support, which is often unnecessary complicated. For some settings solder jumpers or dip switches are used, practical applications are limited by the number of IO available.

**avr-rid** uses an external resistor to encode up to 5 bit of information per IO pin. The build-in ADC is used in conjunction with internal pull-up resistor and only one additional external resistor is required for self-calibration. The code works independently from the supply voltage and over a large temperature range. Possible applications include detecting various plug-in modules, setting the device address for ie. I2C, setting a software controlled voltage/current/power limit and much more.

<br>

## Key Features

 - **simple:** just one resistor and few lines of code - that's it!
 - **precise:** algorithm takes all major error sources into account
 - **lightweight:** minimal demo code is <0.5kB incl. software multiply & divide

<br>

## Requirements

- per resistor: one IO with internal pull-up resistor (20k-50k) and an ADC channel 
- as reference: one IO with internal pull-up resistor (20k-50k), an ADC channel and an external *known* pull-down resistor (10k-100k). After calibration, this IO can be used as a digital output driving a CMOS input.
- ADC with 10 bit resolution and 4 LSB of absolute error or less
- a regulated supply voltage, the absolute value does not matter

<br>

## Limitations

 - Calibration *must* be done at room temperature. Therefore it's best to calibrate once after flashing the device and store the value in EEPROM.
 - The large operational temperature range is only possible by taking temperature related error sources into account. These value were derived from the graphs in ATTINY13A datasheet and are typical values only. I assume the production tolerances for the same part and for other AVR8 MCUs are small enough to not cause any errors, but *I can not guarantee error-free operation.*
 - Small external resistor values may be detected incorrectly due to internal parasitic resistance that only shows when using the ADC together with the internal pull-up resistor. You may need to adjust the calibration value `RID_PARASITIC_OHM`.
 - Large external resistor values may be detected incorrectly due to noise that couples into the line. To minimize the risk, keep high speed signals away from this line and provide a good ground return path.

<br>

## Usage Example
Because the ADC hardware varies quite a bit between parts, **avr-rid** only contains the code for the calculations. A simple ADC example is provided in the demo file.

```c
int main(void)
{
	// enable pull-up resistor and get 10 bit adc value of refrence
	uint16_t adc_ref  = adc_get(0b11<<MUX0, 1<<3);
	
	// calculate internal pull-up resistor value as a reference
	uint16_t res_high = rid_res_high(adc_ref, RID_REF);
	
	// enable pull-up resistor and get 10 bit adc value of rid resistor
	uint16_t adc_val = adc_get(0b10<<MUX0, 1<<4);
	
	// calculate the external pull-down resistor value
	int16_t res_low = rid_res_low(adc_val, res_high);
	
	// map resistor value to it's rid
	rid_e rid = rid_get(res_low);
	
	/* use rid */
}
```

## Getting Started
This section is written especially for everyone who is **not familiar** with the used tools. If you run into problems, please [ask for clarification](#get-help).<br>

### Step 0: Software
 - [**Atmel Studio 7.0**][tool-atmel-studio-7-0] (Build 1931) [free]<br>
   The installer contains all tools you need to open, edit, compile, simulate and flash this code. If you favor another development tool, feel free to use it instead. (But please understand that I can not provide any support).
 - **An AVR8 ISP/JTAG programmer [optional]**<br>
   To program AVR8 MCUs I use the [AVR Dragon][tool-avr-dragon]. It can be also used as a debugger and is available within Atmel Studio by default.

### Step 1: Download avr-rid
 - Clone this repository or hit [Download][git-download] and extract the .zip file.

### Step 2: Browse the project
 - **Open the project in Atmel Studio:**<br>
   Either double click `rid.atsln` or open Atmel Studio and select "File -> Open -> Project/Solution..."

### Step 3: Setup the hardware
 - Connect an ATTINY13A to an ISP programmer
 - Connect PB3 through a 22k 1% resistor to GND
 - Connect PB1 to a logic analyzer or the RX pin USB-UART-adapter (115200 Baud) and connect the ground pins

### Step 4: Run the demo
 - **Select your MCU & Programming tool:**<br>
   Press `F7` to flash the the demo code to the ATTINY13A. You may be prompted to select your programmer first.
   
### Step 5: Test the hardware
 - **Connect a valid resistor:**
	Connect a resistor value from the table below between PB4 and GND. Every 500ms the value is measured and the respective ID reported through UART.
	
<br>

## Documentation

Table of all rid to reistor value assignments:

| rid | resistor 1% |
| --- | ------- |
|   0 |   SHORT |
|   1 |    1000 |
|   2 |    1500 |
|   3 |    2200 |
|   4 |    3300 |
|   5 |    4700 |
|   6 |    5600 |
|   7 |    6800 |
|   8 |    8200 |
|   9 |   10000 |
|  10 |   12000 |
|  11 |   15000 |
|  12 |   18000 |
|  13 |   22000 |
|  14 |   27000 |
|  15 |   33000 |
|  16 |   39000 |
|  17 |   47000 |
|  18 |   56000 |
|  19 |   68000 |
|  20 |   82000 |
|  21 |  100000 |
|  22 |  120000 |
|  23 |  150000 |
|  24 |  180000 |
|  25 |  220000 |
|  26 |  270000 |
|  27 |  330000 |
|  28 |  470000 |
|  29 |  680000 |
|  30 | 1000000 |
|  31 |    OPEN |

The matching threshold values are generated with the included excel file. Note that the error over temperature is unsymetric, the internal resistance value increases if the chip gets hotter or colder. The second excel files proves that the value bins have enough margin between them to account for the ADC error.

<br>

## Under the Hood

There is not much esciting code in this project, the main  challange was to come up with some a light-weight algorithm which does not add much error to the crude measurements. Because calibration is required, a simple LUT to convert ADC values into rids is not possible and more run-time math must be used instead.

This calculation includes a division with a result of ≈1, so floating point calculation would be suited best. However this is not a viable option for the targeted devices, a custom floating point implementation had to be used to save space. Further improvements were made by replacing bulky shift instructions with accessing the next byte in memory directly. See comment in the source code for more details.

<br>

## Support

### Get Help

**Something doesn't work as expected?** No worries! Just open up a new issue in the [GitHub issue tracker][git-issues]. Please provide all information to reproduce your problem. If you don't have a GitHub account (and can't be bothered to create one,) you can [contact](#contact) me directly.

<br>

### Contribute

**Spotted an error?** [Open an issue][git-issues] or submit a pull request.

There is no CONTRIBUTING.md yet, sorry. Contributions will inherit the [license](#license) of this project. If you have any questions, just ask.

<br>

## About
### Status
**This project is currently classified as** <a href="https://github.com/nqtronix/git-template/blob/master/badges.md#project-status"><img src="https://img.shields.io/badge/status-maintained-green.svg" alt="status: maintained"></a><br>
_The developers intend to keep the code in working condition by updating dependencies, fixing bugs and solving issues._

<br>

### Changelog
This project uses [**Semantic Versioning 2.0.0**][semver.org]. During initial development (0.x.x versions) any _major_ increase is substituted with a _minor_ increase (0.1.0->0.2.0 instead of 0.1.0->1.0.0).

The message of each commit contains detailed information about the changes made. The list below is a summary about all significant improvements.

 - **0.1.0** <br>
	- initial release

<br>

### Contact

If you haven't done so already, please check out [Get Help](#get-help) for the fastest possible help on your issue. Alternatively you can find my public email address on my [profile][git-profile].

<br>

## Credits and References

### Projects Used
- [**git-template**][git-repo-git-template] - *A simple and clean git repository template.*<br>
- [**avr-tinyuart**][git-repo-avr-tinyuart] - *A software uart designed for tiny AVR MCUs.*<br>
- [**avr-quantizer**][git-repo-avr-quantizer] - *A quantizer with support for arbitrary bins designed for AVR MCUs.*<br>

<br>


## License
This project is proudly licensed under the [MIT license][git-license].

The MIT license was chosen to give you the freedom to use this project in any way you want, while protecting all contributors from legal claims. Good code works, great code works for everyone. If this code has become a part of one of your projects, a link back to us would be highly appreciated. Thanks!

<!-- Links -->

[git-readme]:README.md
[git-license]:LICENSE.md
[git-profile]:https://github.com/nqtronix
[git-issues]:https://github.com/nqtronix/avr-rid/issues
[git-download]:https://github.com/nqtronix/avr-rid/archive/master.zip

[git-repo-git-template]:https://github.com/nqtronix/git-template
[git-repo-avr-tinyuart]:https://github.com/nqtronix/avr-tinyuart
[git-repo-avr-quantizer]:https://github.com/nqtronix/avr-quantizer

[semver.org]:semver.org

[tool-atmel-studio-7-0]:https://www.microchip.com/mplab/avr-support/atmel-studio-7
[tool-avr-dragon]:https://www.microchip.com/Developmenttools/ProductDetails/ATAVRDRAGON

