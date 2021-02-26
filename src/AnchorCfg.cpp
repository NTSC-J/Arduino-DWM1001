#include "DWM1001.h"

void AnchorCfg::to_bytes(uint8_t *const bytes) const
{
    bytes[0] =
        ((uint8_t)initiator) << 7 |
        ((uint8_t)bridge) << 6 |
        ((uint8_t)enc_en) << 5 |
        ((uint8_t)led_en) << 4 |
        ((uint8_t)ble_en) << 3 |
        ((uint8_t)fw_upd_en) << 2 |
        (uwb_mode & 0x03);
    bytes[1] = 0; // unused
}
