#ifdef ARDUINO

#include "ArduinoSPIDWM1001.h"
#include <Arduino.h>

/*
 * @brief For use without DRDY
 */
ArduinoSPIDWM1001::ArduinoSPIDWM1001(uint8_t pin_cs) :
    pin_cs(pin_cs), pin_drdy(0xff)
{
    pinMode(pin_cs, OUTPUT);
    digitalWrite(pin_cs, HIGH);
}

/*
 * @brief Use with DRDY
 */
ArduinoSPIDWM1001::ArduinoSPIDWM1001(uint8_t pin_cs, uint8_t pin_drdy) :
    pin_cs(pin_cs), pin_drdy(pin_drdy)
{
    pinMode(pin_cs, OUTPUT);
    pinMode(pin_drdy, INPUT);
    digitalWrite(pin_cs, HIGH); // active low
}

/*
 * @brief let DWM1001 SPI thread be IDLE state
 */
void ArduinoSPIDWM1001::nop()
{
    digitalWrite(pin_cs, LOW);
    delayMicroseconds(50);
    for (int i = 0; i < 3; i++) {
        SPI.transfer(0xff); // dummy byte
    }
    digitalWrite(pin_cs, HIGH);
}

DWM1001Error ArduinoSPIDWM1001::write_tlv(
    uint8_t const type,
    uint8_t const length,
    uint8_t const* const value)
{
    // at least 35 us wide pulse has to be generated on CS pin to wakeup device
    digitalWrite(pin_cs, LOW); // active low
    delayMicroseconds(50);
    SPI.transfer(type);
    SPI.transfer(length);
    for (uint8_t i = 0; i < length; i++) {
        SPI.transfer(value[i]);
    }
    digitalWrite(pin_cs, HIGH);
}

DWM1001Error ArduinoSPIDWM1001::read_tlv(
    uint8_t *const type,
    uint8_t *const length,
    uint8_t *const value)
{
    if (pin_drdy != 0xff) {
        while (digitalRead(pin_drdy) == LOW)
            ;
    }
    do {
        *type = SPI.transfer(0xff);
        *length = SPI.transfer(0xff);
    } while (!type);
    for (uint8_t i = 0; i < length; i++) {
        value[i] = SPI.transfer(0xff);
    }
}

#endif // ifdef ARDUINO
