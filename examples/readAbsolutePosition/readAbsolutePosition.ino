
#include "AS5040.h"


AS5040Class AS5040(10, 13, 12, 11);



void setup() {

    SPI.begin();
    AS5040.begin(&SPI, SPISettings(AS5040_CLKAREAD, MSBFIRST,SPI_MODE1));

}

void loop() {

}


// 
// End of file.
//

