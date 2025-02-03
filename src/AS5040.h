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
    AS5040Class(uint8_t CSpin, uint8_t CLKpin, uint8_t MISOpin, uint8_t MOSIpin);

    enum AS5040_RC begin(SPIClass *pSPI, SPISettings SPIs);
    enum AS5040_RC begin(void);
#else
    enum AS5040_RC begin(void);
#endif

    float readAbsolutePosition(void);
    enum AS5040_RC nonPermanentProgram(struct AS5040_OTP otp_val);
    union AS5040_AAPD getStatus(void);


private:
#if defined (ARDUINO_AVR_UNO)
    uint8_t _CSpin;
    uint8_t _CLKpin;
    uint8_t _DOpin;
    uint8_t _PROGpin;

    SPIClass *pSPI;
    SPISettings SPIs;
#endif
    uint16_t counts = 1024;
    union AS5040_AAPD aapd;

    uint16_t _write_read(uint16_t write_val);
    enum AS5040_RC _enable_prog(void);
    enum AS5040_RC _disable_prog(void);
    enum AS5040_RC _check_parity(union AS5040_AAPD val);
    enum AS5040_RC _check_status(union AS5040_AAPD last_read);
};

#endif

//
// End of file.
//
