#pragma once
#include "tlv.h"

#ifdef ARDUINO
#include <stdint.h>
#else
#include <cstdint>
#endif

void uint16_to_le_bytes(uint32_t in, uint8_t *const out);
void int32_to_le_bytes(int32_t in, uint8_t *const out);
uint16_t le_bytes_to_uint16(uint8_t const *const bytes);
int32_t le_bytes_to_int32(uint8_t const *const bytes);
uint64_t le_bytes_to_uint64(uint8_t const *const bytes);

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
 * @brief byte index of GPIO pins available to the user
 */
enum class GPIOIdx : uint8_t {
    GPIO2 = 2,
    GPIO8 = 8,
    GPIO9 = 9,
    GPIO10 = 10,
    GPIO12 = 12,
    GPIO13 = 13,
    GPIO14 = 14,
    GPIO15 = 15,
    GPIO22 = 22,
    GPIO23 = 23,
    GPIO27 = 27,
    GPIO30 = 30,
    GPIO31 = 31
};

/* gpio_value not implemented in favor of plain bool */

/*
 * @brief status of the GPIO pin as input
 */
enum class GPIOPull : uint8_t {
    NOPULL = 0,
    PULLDOWN = 1,
    PULLUP = 3
};

/*
 * @brief the version number of the firmware
 */
struct FWVersion {
    uint8_t maj;
    uint8_t min;
    uint8_t patch;
    uint8_t ver; // upper 4 bits = reserved; lower 4 bits = variant

    static FWVersion from_bytes(uint8_t const *const bytes); // 4 bytes
    uint8_t inline var() const {
        return ver & 0x0f;
    }
};

/*
 * @brief configuration of the tag
 */
struct TagCfg {
    bool stnry_en; // stationary detection enabled?
    bool low_power_en; // low-power mode enabled?
    uint8_t meas_mode; // measurement mode. 0 - TWR; 1, 2, 3 - reserved.
    bool loc_engine_en; // internal location engine enabled?
    bool enc_en; // encryption enabled?
    bool led_en; // general purpose LEDs enabled?
    bool ble_en; // Bluetooth enabled?
    uint8_t uwb_mode; // UWB operation mode. 0 - offline, 1 - passive, 2 - active.
    bool fw_upd_en; // firmware update enabled?
    
    void to_bytes(uint8_t *const bytes) const; // 2 bytes
};

/* TODO AnchorCfg */

/*
 * @brief interrupt configuration
 */
struct IntCfg {
    bool loc_ready; // location data are ready
    bool spi_data_ready; // new SPI data
    bool bh_status_changed; // UWBMAC status changed
    bool bh_data_ready; // UWBMAC backhaul data ready
    bool bh_initialized_changed; // UWBMAC route configured
    bool uwb_scan_ready; // UWB scan result is available
    bool usr_data_ready; // user data received over UWBMAC
    bool uwbmac_joined_changed; // UWBMAC joined
    bool usr_data_sent; // user data TX completed over UWBMAC

    void to_bytes(uint8_t *const bytes) const; // 2 bytes
    static IntCfg from_bytes(uint8_t const *const bytes);
};

/*
 * @brief stationary sensitivity
 */
enum class StnrySensitivity : uint8_t {
    SS_LOW = 0, // 512 mg ("LOW" conflicts with a macro in Arduino.h)
    SS_NORMAL = 1, // 2048 mg
    SS_HIGH = 2 // 4064 mg ("HIGH" conglicts with a macro in Arduino.h)
};

/* TODO evt_id_map */
/* TODO NodeCfg */

struct Status {
    bool loc_ready; // new location data are ready
    bool uwbmac_joined; // node is connected to UWB network
    bool bh_data_ready; // UWB MAC backhaul data ready
    bool bh_status_changed; // UWB MAC status has changed
    bool uwb_scan_ready; // UWB scan results are ready
    bool usr_data_ready; // User data over UWB received
    bool usr_data_sent; // User data over UWB sent
    bool fwup_in_progress; // Firmware update is in progress

    static Status from_bytes(uint8_t const *const bytes); // 2 bytes
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
    //DWM1001Error cfg_anchor_set(AnchorCfg const& cfg);
    //DWM1001Error cfg_get(NodeCfg *const cfg);
    DWM1001Error sleep();
    //DWM1001Error anchor_list_get(AnchorList *const list);
    //DWM1001Error loc_get(LocData *const loc);
    //DWM1001Error baddr_set(Baddr const& baddr);
    //DWM1001Error baddr_get(Baddr *const baddr);
    //DWM1001Error stnry_cfg_set(StnrySensitivity const& sensitivity);
    //DWM1001Error stnry_cfg_get(StnrySensitiviry *const sensitivity);
    DWM1001Error factory_reset();
    DWM1001Error reset();
    //DWM1001Error ver_get(Version *const ver);
    //DWM1001Error uwb_cfg_set(UWBCfg const& cfg);
    //DWM1001Error uwb_cfg_get(UWBCfg *const cfg);
    DWM1001Error usr_data_read(uint8_t *const data, uint8_t *const length);
    DWM1001Error usr_data_write(uint8_t const *const data, uint8_t const length, bool overwrite);
    //DWM1001Error label_read(uint8_t *const label, uint8_t *const length);
    //DWM1001Error label_write(uint8_t const *const label, uint8_t const length);
    DWM1001Error gpio_cfg_output(GPIOIdx const idx, bool const value);
    DWM1001Error gpio_cfg_input(GPIOIdx const idx, GPIOPull const pull_mode);
    DWM1001Error gpio_value_set(GPIOIdx const idx, bool const value);
    DWM1001Error gpio_value_get(GPIOIdx const idx, bool *const value);
    DWM1001Error gpio_value_toggle(GPIOIdx const idx);
    DWM1001Error panid_set(uint16_t const panid);
    DWM1001Error panid_get(uint16_t *const panid);
    DWM1001Error nodeid_get(uint64_t *const nodeid);
    DWM1001Error status_get(Status *const status);
    DWM1001Error int_cfg_set(IntCfg const& cfg);
    void int_cfg_get(IntCfg *const cfg);
    //DWM1001Error enc_key_set(EncKey const& key);
    //DWM1001Error enc_key_clear();
    //DWM1001Error bh_status_get(BHStatus *const status);
    //DWM1001Error backhaul_xfer(...);

    /* transport-dependent features are declared virtual here */
    DWM1001Error virtual write_tlv(
        uint8_t const type, uint8_t const length, uint8_t const* const value) = 0;
    DWM1001Error virtual read_tlv(
        uint8_t *const type, uint8_t *const length, uint8_t *const value) = 0;

    DWM1001Error read_err();
    Position read_pos_xyz();
};
