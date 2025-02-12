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
    /** 
     * /brief begin AS5040 instance.
     * 
     * /param [in] *pSPI Pointer to an Arduino SPI SPIClass instance
     * /param [in] *SPIs copy an Arduino SPI SPISettings instance.
     * /param [in] otp AS5040_OTP copy.
     *
     * Pick your own flavor. begin(void) will use Arduino SPI and SPISettings by
     * default for AS5040. If you want to have control of the SPI instance and
     * the initialization call begin(*pSPI, SPIs).
     *
     * If you want the ic to be configured on the initialization invoque
     * begin(*pSPI, SPIs, otp). Configuration can be performed at any moment
     * after begin(). See nonPermanentProgram()
     *
     * /note OTP required special hardward. Check the datasheet. 
     *
     * /return AS5040_OK on success, AS5040_FAIL on failure.
     */
    enum AS5040_RC begin(void);
    enum AS5040_RC begin(SPIClass *pSPI, SPISettings SPIs);
    enum AS5040_RC begin(SPIClass *pSPI, SPISettings SPIs, struct AS5040_OTP otp);

    /**
     * /brief AS5040 SPI pins definition
     *
     * Tell me what pins are used for the AS5040 interface when instanciating
     * AS5040Class. 
     */
    AS5040Class(uint8_t CSpin, uint8_t CLKpin, uint8_t MISOpin, uint8_t MOSIpin);
#else
    /** 
     * /brief begin AS5040 instance.
     *
     * /note To be implemented!
     * 
     * /return AS5040_OK on success, AS5040_FAIL on failure.
     */
    enum AS5040_RC begin(void);
#endif

    /**
     * /brief read absolute position in [deg]
     *
     * retrieve the last magned position read. 
     *
     * In case the message fails chech the status message with getStatus().
     *
     * /return absolute position (0 - 360) [deg] 
     * /return 404.0 if the message parity fails. 
     * /return 505.0 if the message status fails. 
     */
    float readAbsolutePosition(void);

    /** 
     * /brief program non permanent memory
     *
     * /param [in] otp_val struct AS5040_OTP with the desired configuration.
     *
     * The AS5040 can operated without programming with the following
     * configuration:
     *      CCW:0 = Clockwise operation
     *      Z9 to Z0: 00 = No programmed zero position 
     *      Indx: 0 = Index bit width = 1LSB
     *      Div0, Div1: 00 = Incremental resolution = 10-bit 
     *      Md0, MD1: 00 = Incremental mode = quadrature
     *
     * /return AS5040_OK on success, AS5040_FAIL on failure.
     */
    enum AS5040_RC nonPermanentProgram(struct AS5040_OTP otp_val);

    /** 
     * /brief retrieve last status message
     *
     * Every time you read the absolute position the driver process the status
     * data from the message and saves it localy in temporary memory. You can
     * use this function to retrive that status data. You should call
     * readAbsolutePosition() every time. If not you will get the status of the
     * las read or a 0 if you never read an absolute position.
     *
     * /return uinon AS5040_AAPD with last status received.
     */
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
