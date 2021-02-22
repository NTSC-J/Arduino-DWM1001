#pragma once
#include "tlv.h"
#include <cstdint>

/*
 * @brief Position coordinates in millimeters, little endian
 */
struct Position {
    int32_t x;
    int32_t y;
    int32_t z;
    uint8_t qf; // quality factor ([0, 100])

    Position(int32_t x = 0, int32_t y = 0, int32_t z = 0, uint8_t qf = 100);

    void to_bytes(uint8_t *const bytes) const; // 13 bytes
    static Position from_bytes(uint8_t const *const bytes);
};

/*
 * @brief configuration of the tag
 */
struct TagCfg {
    bool stnry_en;
    bool low_power_en;
    uint8_t meas_mode;
    bool loc_engine_en;
    bool led_en;
    bool ble_en;
    uint8_t uwb_mode;
    bool fw_upd_en;

    void to_bytes(uint8_t *const bytes); // 2 bytes
};

/*
 * @brief interrupt configuration
 */
struct IntCfg {
    bool loc_ready;
    bool spi_data_ready;
    bool bh_status_changed;
    bool bh_data_ready;
    bool bh_initialized_changed;
    bool uwb_scan_ready;
    bool usr_data_ready;
    bool uwbmac_joined_changed;
    bool usr_data_sent;

    void to_bytes(uint8_t *const bytes); // 2 bytes
};

enum class DWM1001Error : int8_t {
    // returned from functions
    Ok = 0,
    Err = 1,
    Internal = 2,
    Param = 3,
    Busy = 4,
    Permit = 5
};

/* 
 * @brief DWM1001 driver base class
 */
class DWM1001 {
public:
    DWM1001Error pos_set(Position const& position);
    DWM1001Error pos_get(Position *const position);
    DWM1001Error upd_rate_set(uint16_t const ur, uint16_t const urs);
    DWM1001Error upd_rate_get(uint16_t *const ur, uint16_t *const urs);
    DWM1001Error cfg_tag_set(TagCfg const& cfg);
    DWM1001Error usr_data_read(uint8_t *const length, uint8_t *const data);
    DWM1001Error usr_data_write(uint8_t const length, uint8_t const *const data, bool overwrite);
    DWM1001Error panid_set(uint16_t const panid);
    DWM1001Error panid_get(uint16_t *const panid);
    DWM1001Error nodeid_get(uint64_t *const nodeid);
    DWM1001Error int_cfg_set(IntCfg const& cfg);
    DWM1001Error int_cfg_get(IntCfg *const cfg);
    DWM1001Error reset();

    /* transport-dependent features are declared virtual here */
    void virtual nop() = 0;
    DWM1001Error virtual write_tlv(
        uint8_t const type, uint8_t const length, uint8_t const* const value) = 0;
    DWM1001Error virtual read_tlv(
        uint8_t *const type, uint8_t *const length, uint8_t *const value) = 0;

    DWM1001Error read_err();
    Position read_pos_xyz();
};
