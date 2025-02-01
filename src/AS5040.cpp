
#include "AS5040.h"

#if defined (ARDUINO_AVR_UNO)
AS5040Class::AS5040Class(uint8_t CSpin, uint8_t CLKpin, uint8_t MISOpin, uint8_t MOSIpin)
{
     _CSpin = CSpin;
     _CLKpin = CLKpin;
     _DOpin = MISOpin;
     _PROGpin = MOSIpin;
}

enum AS5040_RC AS5040Class::begin(SPIClass *pSPI, SPISettings SPIs)
{
    enum AS5040_RC rc = AS5040_OK; /* return command */

    struct AS5040_OTP otp;

    AS5040Class::pSPI = pSPI;
    AS5040Class::SPIs = SPIs;

    otp.val.uint16 = 0;
    if( nonPermanentProgram(otp) )
    {
        rc = AS5040_FAIL;
    }

    return rc;
}

enum AS5040_RC AS5040Class::begin(void)
{
    enum AS5040_RC rc; /* return command */

    AS5040Class::pSPI = &SPI;
    AS5040Class::SPIs = SPISettings(AS5040_CLKAREAD, MSBFIRST,SPI_MODE1);

    //TODO: to be implemented
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
    union AS5040_AAPD aapd;
    float angle;

    aapd.val.uint16 = _write_read(0x00);

    // TODO: Check Status

    // TODO: counts to angle

    return angle;
}

enum AS5040_RC AS5040Class::nonPermanentProgram(struct AS5040_OTP otp_val)
{
    enum AS5040_RC rc = AS5040_OK;

#if defined (ARDUINO_AVR_UNO)

    // Enable programming mode:
    digitalWrite(_CSpin, LOW);
    delayMicroseconds (1);
    digitalWrite(_CLKpin, LOW);
    digitalWrite(_PROGpin, HIGH);
    delayMicroseconds (3); /* tprog-enable > 2us */
    digitalWrite(_CSpin, HIGH);
    //ProgEn! Ready to programm
    
    pSPI->beginTransaction(SPISettings(AS5040_CLKAREAD, MSBFIRST, SPI_MODE0));

    // Write OTP
    pSPI->transfer16(otp_val.val.uint16);

    // After performing a group of transfers and releasing the chip select
    // signal, this function allows others to access the SPI bus
    pSPI->endTransaction();

    digitalWrite(_CSpin, LOW);
    digitalWrite(_PROGpin, LOW);
    digitalWrite(_CLKpin, HIGH);
    delayMicroseconds (5);
    digitalWrite(_CSpin, HIGH); // Ready for reads!

#else
    /* User should implement a different SPI driver here */
    #warning Implement _write_read()
#endif
    return rc;
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

    delayMicroseconds (1); //The delay should be 500ns

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

// 
// End of file.
//
