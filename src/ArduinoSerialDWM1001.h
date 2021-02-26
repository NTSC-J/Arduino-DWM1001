#pragma once
#ifdef ARDUINO
#include "DWM1001.h"
#include <Arduino.h>

class ArduinoSerialDWM1001 : public DWM1001 {
public:
    ArduinoSerialDWM1001(HardwareSerial& serial);

    DWM1001Error write_tlv(
        uint8_t const type, uint8_t const length, uint8_t const* const value);
    DWM1001Error read_tlv(
        uint8_t *const type, uint8_t *const length, uint8_t *const value);

private:
    HardwareSerial& serial;
};

#endif // ifdef ARDUINO
