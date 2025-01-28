
#include "AS5040.h"

#if defined (ARDUINO_AVR_UNO)
enum AS5040_RC AS5040::begin(SPIClass *pspic, SPISettings spis)
{
    enum AS5040_RC rc; /* return command */

    AS5040::pSPI = pspic;
    AS5040::SPIsc = spis;

    return rc;
}
#else
enum AS5040_RC AS5040::begin()
{
    enum AS5040_RC rc; /* return command */

    //TODO: to be implemented
    return rc;
}
#endif

uint16_t AS5040::readAbsolutePosition(void)
{
    uint16_t val;
    //TODO:

    return val;
}

uint16_t AS5040::_write_read(uint16_t write_val)
{

    uint16_t read_val = 0;
    
    // Before using SPI.transfer() or asserting chip select pins,
    // this function is used to gain exclusive access to the SPI bus
    // and configure the correct settings.
    AS5040::pSPI->beginTransaction(AS5040::SPIsc);

    // take the SS pin low to select the chip:
    digitalWrite(10, LOW);

    delay(100);

    // send in the address and value via SPI:
    read_val = AS5040::pSPI->transfer16(write_val);

    delay(100);

    // take the SS pin high to de-select the chip:
    digitalWrite(10, HIGH);

    // After performing a group of transfers and releasing the chip select
    // signal, this function allows others to access the SPI bus
    AS5040::pSPI->endTransaction();

    return read_val;
}

// 
// End of file.
//
