#ifndef __AS5040_H__
#define __AS5040_H__

#include <stdint.h>

enum AS5040_RC
{
    AS5040_FAIL = -1,
    AS5040_OK = 0
};

class AS5040 
{
public:
    enum AS5040_RC begin();
    uint16_t readAbsolutePosition(void);

private:
    uint16_t _write_read(uint16_t write_val);
};

#endif

//
// End of file.
//
