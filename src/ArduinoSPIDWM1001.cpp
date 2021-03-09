#ifdef ARDUINO

#include "ArduinoSPIDWM1001.h"

/*
 * @brief Initialize pins.
 */
ArduinoSPIDWM1001::ArduinoSPIDWM1001(uint8_t pin_cs, uint8_t pin_drdy) :
    pin_cs(pin_cs), pin_drdy(pin_drdy)
{
    pinMode(pin_cs, OUTPUT);
    if (pin_drdy != NO_DRDY) {
        pinMode(pin_drdy, INPUT);
    }
    digitalWrite(pin_cs, HIGH); // active low
    SPI.begin();
}

/*
 * @brief let DWM1001 SPI thread be IDLE state
 */
void ArduinoSPIDWM1001::nop()
{
    SPI.beginTransaction(SPI_SETTINGS);
    digitalWrite(pin_cs, LOW);
    delayMicroseconds(50);
    for (int i = 0; i < 3; i++) {
        SPI.transfer(0xff); // dummy byte
    }
    digitalWrite(pin_cs, HIGH);
    SPI.endTransaction();
}

void ArduinoSPIDWM1001::write_tlv(
    uint8_t const type,
    uint8_t const length,
    uint8_t const* const value)
{
    SPI.beginTransaction(SPI_SETTINGS);
    // at least 35 us wide pulse has to be generated on CS pin to wakeup device
    digitalWrite(pin_cs, LOW); // active low
    delayMicroseconds(35);

    SPI.transfer(type);
    SPI.transfer(length);
    for (uint8_t i = 0; i < length; i++) {
        SPI.transfer(value[i]);
    }

    digitalWrite(pin_cs, HIGH);
    //SPI.endTransaction();
}

DWM1001Error ArduinoSPIDWM1001::read_all_resp()
{
    clear_tlv_data();

    if (pin_drdy != NO_DRDY) {
        while (digitalRead(pin_drdy) == LOW) {
            delay(1);
        }
    }

    //SPI.beginTransaction(SPI_SETTINGS);

    uint8_t size = 0, read = 0, page = 0;
    while (!size) {
        digitalWrite(pin_cs, LOW);
        size = SPI.transfer(0xff);
        digitalWrite(pin_cs, HIGH);
        delay(1);
    }

    digitalWrite(pin_cs, LOW);
    while (read < size) {
        auto data = last_tlv_data[page];
        data[0] = SPI.transfer(0xff); // type
        data[1] = SPI.transfer(0xff); // length
        read += 2;
        for (int i = 0; i < data[1]; i++) {
            data[i + 2] = SPI.transfer(0xff);
            read++;
        }
        page++;
    }
    digitalWrite(pin_cs, HIGH);

    SPI.endTransaction();
}

#endif // ifdef ARDUINO
