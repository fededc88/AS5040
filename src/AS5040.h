#ifndef __AS5040_H__
#define __AS5040_H__

#if defined (ARDUINO_AVR_UNO)
#include <Arduino.h>
#include <SPI.h>
#endif

#include <stdint.h>
#include "AS5040_defs.h"

class AS5040Class 
{
public:
#if defined (ARDUINO_AVR_UNO)
    enum AS5040_RC begin(SPIClass *pSPI, SPISettings SPIs, uint8_t CSpin);
    enum AS5040_RC begin(void);
#else
    enum AS5040_RC begin();
#endif

    float readAbsolutePosition(void);

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
