#ifdef ARDUINO

#include "ArduinoSPIDWM1001.h"
#include <Arduino.h>
#include <SPI.h>

/*
 * @brief Initialize pins.
 */
ArduinoSPIDWM1001::ArduinoSPIDWM1001(uint8_t pin_cs, uint8_t pin_drdy) :
    pin_cs(pin_cs), pin_drdy(pin_drdy)
{
    pinMode(pin_cs, OUTPUT);
    if (pin_drdy != 0xff) {
        pinMode(pin_drdy, INPUT);
    }
    digitalWrite(pin_cs, HIGH); // active low
}

/*
 * @brief let DWM1001 SPI thread be IDLE state
 */
void ArduinoSPIDWM1001::nop()
{
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    digitalWrite(pin_cs, LOW);
    delayMicroseconds(50);
    for (int i = 0; i < 3; i++) {
        SPI.transfer(0xff); // dummy byte
    }
    digitalWrite(pin_cs, HIGH);
    SPI.endTransaction();
}

DWM1001Error ArduinoSPIDWM1001::write_tlv(
    uint8_t const type,
    uint8_t const length,
    uint8_t const* const value)
{
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    // at least 35 us wide pulse has to be generated on CS pin to wakeup device
    digitalWrite(pin_cs, LOW); // active low
    delayMicroseconds(50);

    SPI.transfer(type);
    SPI.transfer(length);
    for (uint8_t i = 0; i < length; i++) {
        SPI.transfer(value[i]);
    }

    digitalWrite(pin_cs, HIGH);
    SPI.endTransaction();

    return DWM1001Error::Ok;
}

DWM1001Error ArduinoSPIDWM1001::read_tlv(
    uint8_t *const type,
    uint8_t *const length,
    uint8_t *const value)
{
    if (pin_drdy != 0xff) {
        while (digitalRead(pin_drdy) == LOW)
            delay(1);
    }

    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    digitalWrite(pin_cs, LOW);
    delayMicroseconds(50);

    uint8_t size, num;
    while (true) {
        size = SPI.transfer(0xff);
        num = SPI.transfer(0xff);
        delay(1);
        if (size || num)
            break;
    }
    // assert(size == 1 && num == length + 2);
    // ↑this fails when using backhaul_xfer
    
    *type = SPI.transfer(0xff);
    *length = SPI.transfer(0xff);
    for (uint8_t i = 0; i < *length; i++) {
        value[i] = SPI.transfer(0xff);
    }

    digitalWrite(pin_cs, HIGH);
    SPI.endTransaction();

    return DWM1001Error::Ok;
}

#endif // ifdef ARDUINO
