#include "DWM1001.h"

Status Status::from_bytes(uint8_t const *const bytes)
{
    return {
        (bool)(bytes[0] & 0x01),
        (bool)(bytes[0] & 0x02),
        (bool)(bytes[0] & 0x04),
        (bool)(bytes[0] & 0x08),
        (bool)(bytes[0] & 0x20),
        (bool)(bytes[0] & 0x40),
        (bool)(bytes[0] & 0x80),
        (bool)(bytes[1] & 0x01)
    };
}
