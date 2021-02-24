#pragma once

#ifndef ARDUINO

#include "DWM1001.h"
#include <libserialport.h>

class SerialDWM1001 : public DWM1001 {
public:
    SerialDWM1001(char const *const portname, int32_t timeout = 10000);
    ~SerialDWM1001();

    DWM1001Error write_tlv(
        uint8_t const type, uint8_t const length, uint8_t const* const value);
    DWM1001Error read_tlv(
        uint8_t *const type, uint8_t *const length, uint8_t *const value);

private:
    sp_port *port;
    int32_t timeout;
    sp_return check(sp_return result);
};

#endif // ifndef ARDUINO
