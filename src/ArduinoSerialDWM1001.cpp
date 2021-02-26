#ifdef ARDUINO
#include "ArduinoSerialDWM1001.h"
#include <Arduino.h>

ArduinoSerialDWM1001::ArduinoSerialDWM1001(HardwareSerial& serial) : serial(serial)
{
    serial.begin(115200);
}

DWM1001Error ArduinoSerialDWM1001::write_tlv(uint8_t const type, uint8_t const length, uint8_t const *const value)
{
    serial.write(&type, 1);
    serial.write(&length, 1);
    serial.write(value, (size_t)length);
    serial.flush();

    return DWM1001Error::Ok; // FIXME
}

DWM1001Error ArduinoSerialDWM1001::read_tlv(uint8_t *const type, uint8_t *const length, uint8_t *const value)
{
    serial.readBytes(type, 1);
    serial.readBytes(length, 1);
    serial.readBytes(value, (size_t)length);

    return DWM1001Error::Ok; // FIXME
}

#endif // ifdef ARDUINO
