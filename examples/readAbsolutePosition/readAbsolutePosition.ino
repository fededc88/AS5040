
#include "AS5040.h"


AS5040Class AS5040;



void setup() {

    SPI.begin();
    AS5040.begin(&SPI, SPISettings(AS5040_CLKAREAD, MSBFIRST,SPI_MODE1));

}

void loop() {

}


// 
// End of file.
//

