
#include "AS5040.h"

#if defined (ARDUINO_AVR_UNO)
AS5040Class::AS5040Class(uint8_t CSpin, uint8_t CLKpin, uint8_t MISOpin, uint8_t MOSIpin)
{
     _CSpin = CSpin;
     _CLKpin = CLKpin;
     _DOpin = MISOpin;
     _PROGpin = MOSIpin;
}

enum AS5040_RC AS5040Class::begin(void)
{
    enum AS5040_RC rc; /* return command */

    AS5040Class::pSPI = &SPI;
    AS5040Class::SPIs = SPISettings(AS5040_CLKAREAD, MSBFIRST,SPI_MODE1);

    //TODO: to be implemented
    return rc;
}

enum AS5040_RC AS5040Class::begin(SPIClass *pSPI, SPISettings SPIs)
{
    enum AS5040_RC rc = AS5040_OK; /* return command */

    AS5040Class::pSPI = pSPI;
    AS5040Class::SPIs = SPIs;

    /* Use default AS5040 configuration */

    return rc;
}

enum AS5040_RC AS5040Class::begin(SPIClass *pSPI, SPISettings SPIs, struct AS5040_OTP otp)
{
    enum AS5040_RC rc = AS5040_OK; /* return command */

    AS5040Class::pSPI = pSPI;
    AS5040Class::SPIs = SPIs;

    if ( nonPermanentProgram(otp) )
    {
        rc = AS5040_FAIL;
    }

    return rc;
}

#else
enum AS5040_RC AS5040Class::begin()
{
    enum AS5040_RC rc; /* return command */

    #warning User should implement this! 

    //TODO: to be implemented
    return rc;
}
#endif

float AS5040Class::readAbsolutePosition(void)
{
    float angle;

    aapd.val.uint16 = _write_read(0x00);

    /* Check Parity */
    if( _check_parity(aapd) )
     {
         angle = 404;
     }
    else if ( _check_status( aapd ) )
    {
         angle = 405;
    }
    else
    {
        // TODO: counts to angle
        angle = ((float) aapd.bit.angle) / counts * 360;
    }

    return angle;
}

enum AS5040_RC AS5040Class::nonPermanentProgram(struct AS5040_OTP otp_val)
{
    enum AS5040_RC rc = AS5040_OK;

#if defined (ARDUINO_AVR_UNO)

    if ( _enable_prog() )
    {
        rc = AS5040_FAIL;
    }
    else
    {
        //ProgEn! Ready to programm

        pSPI->beginTransaction(SPISettings(AS5040_CLKPROG, MSBFIRST, SPI_MODE0));

        // Write OTP
        pSPI->transfer16(otp_val.val.uint16);

        pSPI->endTransaction();

        _disable_prog();
    }

#else
    /* User should implement a different SPI driver here */
    #warning Implement _nonPermanentProgram()
#endif
    return rc;
}

union AS5040_AAPD AS5040Class::getStatus(void)
{
    union AS5040_AAPD aapd_ghost = aapd;

    aapd_ghost.bit.angle = 0;
    aapd_ghost.bit.parity = 0;

    return aapd_ghost;
}

uint16_t AS5040Class::_write_read(uint16_t write_val)
{

    uint16_t read_val = 0;
    
#if defined (ARDUINO_AVR_UNO)
    // Before using SPI.transfer() or asserting chip select pins,
    // this function is used to gain exclusive access to the SPI bus
    // and configure the correct settings.
    pSPI->beginTransaction(SPIs);

    // take the SS pin low to select the chip:
    digitalWrite(_CSpin, LOW);

    delayMicroseconds (10); //The delay should be 500ns

    // send in the address and value via SPI:
    read_val = pSPI->transfer16(write_val);

    // take the SS pin high to de-select the chip:
    digitalWrite(_CSpin, HIGH);

    // After performing a group of transfers and releasing the chip select
    // signal, this function allows others to access the SPI bus
    pSPI->endTransaction();
#else
    /* User should implement a different SPI driver here */
    #warning Implement _write_read()
#endif

    return read_val;
}

enum AS5040_RC AS5040Class::_enable_prog(void)
{
    enum AS5040_RC rc = AS5040_OK;

#if defined (ARDUINO_AVR_UNO)
    
    /* AVR shares GPIOs with peripherals in a way where they don't need to be
     * reconfigured for DIO but they can overwrite the value. If you are
     * migrating this driver to another target don't forget to reconfigure GPIOs
     */

    /* Enable programming procedure */
    digitalWrite(_CSpin, LOW);
    delayMicroseconds(1);
    digitalWrite(_PROGpin, HIGH);
    delayMicroseconds (3); /* tprog-enable > 2us */

    if(digitalRead(_PROGpin) == LOW)
    {
        rc = AS5040_FAIL;
    }

    digitalWrite(_CSpin, HIGH);
    delayMicroseconds (3); /* tdata in > 2us */

    /* Ready for program! */

#else
    /* User should implement a different SPI driver here */
    #warning Implement _enable_prog()

#endif

    return rc;
}

enum AS5040_RC AS5040Class::_disable_prog(void)
{
    enum AS5040_RC rc = AS5040_OK;

#if defined (ARDUINO_AVR_UNO)

    /* I write this 0 to pull down the MOSI(PROG) line  to finish programming */
    _write_read(0);

    delayMicroseconds (5);
    digitalWrite(_CSpin, HIGH);

    /* Ready for reads! */
                                
#else
    /* User should implement a different SPI driver here */
    #warning Implement _disable_prog()

#endif
    return rc;
}

enum AS5040_RC AS5040Class::_check_parity(union AS5040_AAPD last_read)
{
    enum AS5040_RC rc = AS5040_OK;

    uint8_t parity = 0;
    uint16_t n;

    n = last_read.val.uint16 & 0xFFFE;

    while(n)
    {
        parity = !parity;
        n = n & (n - 1);
    }        

    if( parity != last_read.bit.parity)
        rc = AS5040_FAIL; 

    return rc;
}

enum AS5040_RC AS5040Class::_check_status(union AS5040_AAPD last_read)
{
    enum AS5040_RC rc = AS5040_OK;

    if(!last_read.bit.OCF)
        rc = AS5040_FAIL;
    else if( last_read.bit.COF )
        rc = AS5040_FAIL;
    else if ( last_read.bit.LIN )
        rc = AS5040_FAIL;
    else if ( last_read.bit.MagINCn && last_read.bit.MagDECn)
        /* Magnetic Input Field invalid – out of range: <45mT or >75mT
         * or missing magnet */
        rc = AS5040_FAIL;

    return rc;
}

// 
// End of file.
//
