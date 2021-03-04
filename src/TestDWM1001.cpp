#ifndef ARDUINO

#include "TestDWM1001.h"
#include <cstdio>

void TestDWM1001::nop()
{

}

void TestDWM1001::write_tlv(uint8_t const type, uint8_t const length, uint8_t const* const value)
{
    printf("write_tlv: type = %02x, length = %u\n", type, length);
    printf("write_tlv: tlv = %02x %02x ", type, length);
    for (uint8_t i = 0; i < length; i++) {
        printf("%02x ", value[i]);
    }
    printf("\n");
}

DWM1001Error TestDWM1001::read_all_resp()
{
    printf("read_all_resp was called\n");
    return DWM1001Error::Ok;
}

#endif // ifndef ARDUINO
