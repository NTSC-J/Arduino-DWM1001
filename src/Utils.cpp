#include "DWM1001.h"

#ifdef ARDUINO
#include <stdint.h>
#else
#include <cstdint>
#endif

void uint16_to_le_bytes(uint32_t in, uint8_t *const out)
{
    out[0] = in & 0xff;
    out[1] = (in >> 8) & 0xff;
}

void int32_to_le_bytes(int32_t in, uint8_t *const out)
{
    out[0] = in & 0xff;
    out[1] = (in >> 8) & 0xff;
    out[2] = (in >> 16) & 0xff;
    out[3] = (in >> 24) & 0xff;
}

uint16_t le_bytes_to_uint16(uint8_t const *const bytes)
{
    uint16_t ret = 0;
    ret |= bytes[0];
    ret |= ((uint16_t)bytes[1]) << 8;
    return ret;
}

int32_t le_bytes_to_int32(uint8_t const *const bytes)
{
    int32_t ret = 0;
    ret |= bytes[0];
    ret |= ((int32_t)bytes[1]) << 8;
    ret |= ((int32_t)bytes[2]) << 16;
    ret |= ((int32_t)bytes[3]) << 24;
    return ret;
}

uint64_t le_bytes_to_uint64(uint8_t const *const bytes)
{
    uint64_t ret = 0;
    ret |= bytes[0];
    ret |= ((uint64_t)bytes[1]) << 8;
    ret |= ((uint64_t)bytes[2]) << 16;
    ret |= ((uint64_t)bytes[3]) << 24;
    ret |= ((uint64_t)bytes[4]) << 32;
    ret |= ((uint64_t)bytes[5]) << 40;
    ret |= ((uint64_t)bytes[6]) << 48;
    ret |= ((uint64_t)bytes[7]) << 56;
    return ret;
}
