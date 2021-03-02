#pragma once
#ifdef ARDUINO
#include "DWM1001.h"
#include <Arduino.h>
#include <SPI.h>
#define SPI_SETTINGS (SPISettings(8000000, MSBFIRST, SPI_MODE0))

class ArduinoSPIDWM1001 : public DWM1001 {
public:
    ArduinoSPIDWM1001(uint8_t pin_cs, uint8_t pin_drdy = NO_DRDY);

    void nop();
    void write_tlv(
        uint8_t const type, uint8_t const length, uint8_t const* const value);
    DWM1001Error read_all_resp();

private:
    uint8_t const pin_cs; // chip select (output)
    uint8_t const pin_drdy; // data ready (input)
    uint8_t constexpr static NO_DRDY = 0xff;
    uint16_t constexpr static MAX_RETRY = 10000;
};

#endif // ifdef ARDUINO
