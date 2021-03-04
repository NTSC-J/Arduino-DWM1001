#pragma once

#ifndef ARDUINO

#include "DWM1001.h"
#include <libserialport.h>

class SerialDWM1001 : public DWM1001 {
public:
    SerialDWM1001(char const *const portname, int32_t timeout = 10000);
    ~SerialDWM1001();

    void write_tlv(
        uint8_t const type, uint8_t const length, uint8_t const* const value) override;
    DWM1001Error read_all_resp() override;

private:
    sp_port *port;
    int32_t timeout;
    sp_return check_(sp_return result, char const* file, int line);
};

#define check(r) (check_(r, __FILE__, __LINE__))

#endif // ifndef ARDUINO
