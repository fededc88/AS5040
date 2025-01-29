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

/**
 * D9:D0 absolute angular position data (MSB is clocked out first)
 *
 * OCF (Offset Compensation Finished), logic high indicates the finished Offset
 * Compensation Algorithm. For fast startup, this bit may be polled by the
 * external microcontroller. As soon as this bit is set, the AS5040 has
 * completed the startup and the data is valid.
 *
 * COF (CORDIC Overflow), logic high indicates an out of range error in the
 * CORDIC part. When this bit is set, the data at D9:D0 is invalid. 
 * Bring the magnet within the X-Y-Z tolerance limits.
 *
 * LIN (Linearity Alarm), logic high indicates that the input field generates a
 * critical output linearity. When this bit is set, the data at D9:D0 may still
 * be used, but can contain invalid data. Bringing the magnet within the X-Y-Z
 * tolerance limits.
 * 
 * Magnetic Magnitude Variation Indicator
 * MagINCn MagDECn |    Description
 *   0       0        No distance change; Magnetic input field OK (in range, 45mT to 75mT)
 *   0       1        Distance increase: Pull-function.  (*)
 *   1       0        Distance decrease: Push- function. (*) 
 *   1       1        Magnetic Input Field invalid – out of range: <45mT or >75mT (or missing magnet)
 *
 * (*) This state is dynamic, it is only active while the magnet is moving away
 * from the chip in Z-axis.
 */
struct AS5040_STATUS
{
    uint16_t OCF:1; /**> Ofcet Compensation Finished */
    uint16_t COF:1; /**> Cordic Overflow */
    uint16_t LIN:1; /**> Linear Alarm */
    uint16_t MagINCn:1; /**> Magnitud Increase */
    uint16_t MagDECn:1; /**> Magnitud Decrease */
    uint16_t parity:1; /**> bit for transmission error detection of bits 1 to 15 */
};


struct AS5040_AAPD
{
    uint16_t angle:10; /**> Absolute angular position data */
    struct AS5040_STATUS status;
};

class AS5040Class 
{
public:
#if defined (ARDUINO_AVR_UNO)
    enum AS5040_RC begin(SPIClass *pSPI, SPISettings SPIs, uint8_t CSpin);
    enum AS5040_RC begin(void);
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
