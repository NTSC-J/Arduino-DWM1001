#pragma once

#ifndef ARDUINO

#include "DWM1001.h"

/* TODO unit test */
class TestDWM1001 : public DWM1001 {
public:
    void nop();
    DWM1001Error write_tlv(
        uint8_t const type, uint8_t const length, uint8_t const* const value);
    DWM1001Error read_tlv(
        uint8_t *const type, uint8_t *const length, uint8_t *const value);
};

#endif // ifndef ARDUINO
