#pragma once

#ifndef ARDUINO

#include "DWM1001.h"

/* TODO unit test */
class TestDWM1001 : public DWM1001 {
public:
    void nop();
    void write_tlv(
        uint8_t const type, uint8_t const length, uint8_t const* const value) override;
    DWM1001Error read_all_resp() override;
};

#endif // ifndef ARDUINO
