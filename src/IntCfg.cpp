#include "DWM1001.h"

void IntCfg::to_bytes(uint8_t *const bytes) const
{
    bytes[0] =
        ((uint8_t)uwbmac_joined_changed) << 7 |
        ((uint8_t)usr_data_ready) << 6 |
        ((uint8_t)uwb_scan_ready) << 5 |
        ((uint8_t)bh_initialized_changed) << 4 |
        ((uint8_t)bh_data_ready) << 3 |
        ((uint8_t)bh_status_changed) << 2 |
        ((uint8_t)spi_data_ready) << 1 |
        ((uint8_t)loc_ready);
    bytes[1] =
        (uint8_t)usr_data_sent;
}

IntCfg IntCfg::from_bytes(uint8_t const *const bytes)
{
    return {
        (bool)(bytes[0] & 0x01),
        (bool)(bytes[0] & 0x02),
        (bool)(bytes[0] & 0x04),
        (bool)(bytes[0] & 0x08),
        (bool)(bytes[0] & 0x10),
        (bool)(bytes[0] & 0x20),
        (bool)(bytes[0] & 0x40),
        (bool)(bytes[0] & 0x80),
        (bool)(bytes[1] & 0x01)
    };
}
