#pragma once
#ifdef ARDUINO
#include "DWM1001.h"

class ArduinoSPIDWM1001 : public DWM1001 {
public:
    ArduinoSPIDWM1001(uint8_t pin_cs);
    ArduinoSPIDWM1001(uint8_t pin_cs, uint8_t pin_drdy);

    void nop();
    DWM1001Error write_tlv(
        uint8_t const type, uint8_t const length, uint8_t const* const value);
    DWM1001Error read_tlv(
        uint8_t *const type, uint8_t *const length, uint8_t *const value);

private:
    uint8_t const pin_cs; // chip select (output)
    uint8_t const pin_drdy; // data ready (input)
};

#endif // ifdef ARDUINO
