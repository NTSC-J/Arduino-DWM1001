#include "DWM1001.h"

FWVersion FWVersion::from_bytes(uint8_t const *const bytes)
{
    uint8_t maj = bytes[0];
    uint8_t min = bytes[1];
    uint8_t patch = bytes[2];
    uint8_t ver = bytes[3];
    
    return {maj, min, patch, ver};
}
