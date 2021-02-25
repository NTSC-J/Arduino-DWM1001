#include "DWM1001.h"

NodeCfg NodeCfg::from_bytes(uint8_t const *const bytes)
{
    return {
        .mode = (bool)(bytes[1] & 0x20),
        .initiator = (bool)(bytes[1] & 0x10),
        .bridge = (bool)(bytes[1] & 0x08),
        .stnry_en = (bool)(bytes[1] & 0x04),
        .meas_mode = (uint8_t)(bytes[1] & 0x03), // 2 bits
        .low_power_en = (bool)(bytes[0] & 0x80),
        .loc_engine_en = (bool)(bytes[0] & 0x40),
        .enc_en = (bool)(bytes[0] & 0x20),
        .led_en = (bool)(bytes[0] & 0x10),
        .ble_en = (bool)(bytes[0] & 0x08),
        .fw_upd_en = (bool)(bytes[0] & 0x04),
        .uwb_mode = (uint8_t)(bytes[0] & 0x03) // 2 bits
    };
}