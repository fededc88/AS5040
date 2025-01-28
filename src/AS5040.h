#ifndef __AS5040_H__
#define __AS5040_H__

#if defined (ARDUINO_AVR_UNO)
#include <Arduino.h>
#include <SPI.h>
#endif

#include <stdint.h>

#define AS5040_TCLKFE 500 /* Time between falling edge of CSn and first falling edge of CLK [nS] */
#define AS5040_CLKPROG 250000 /* Write Data programming CLK max frequency [Hz] */
#define AS5040_CLKAREAD 100000 /* Analog read CLK max frequency [Hz] */

enum AS5040_RC
{
    AS5040_FAIL = -1,
    AS5040_OK = 0
};

class AS5040Class 
{
public:
#if defined (ARDUINO_AVR_UNO)
    enum AS5040_RC begin(SPIClass *pSPI, SPISettings SPIs, uint8_t CSpin);
#else
    enum AS5040_RC begin();
#endif

    uint16_t readAbsolutePosition(void);

private:
#if defined (ARDUINO_AVR_UNO)
    SPIClass *pSPI;
    SPISettings SPIs;
    uint8_t CSpin;
#endif

    uint16_t _write_read(uint16_t write_val);
};

#endif

//
// End of file.
//
