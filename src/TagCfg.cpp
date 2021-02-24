#include "DWM1001.h"

void TagCfg::to_bytes(uint8_t *const bytes) const
{
    bytes[0] =
        ((uint8_t)low_power_en) << 7 |
        ((uint8_t)loc_engine_en) << 6 |
        ((uint8_t)enc_en) << 5 |
        ((uint8_t)led_en) << 4 |
        ((uint8_t)ble_en) << 3 |
        ((uint8_t)fw_upd_en) << 2 |
        (uwb_mode & 0x03);
    bytes[1] =
        ((uint8_t)stnry_en) << 2 |
        (meas_mode & 0x03);
}
