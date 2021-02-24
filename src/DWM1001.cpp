#include "DWM1001.h"

#ifdef ARDUINO
#include <string.h>
#else
#include <cstring>
#endif

#pragma region utility_functions
void static uint16_to_le_bytes(uint32_t in, uint8_t *const out)
{
    out[0] = in & 0xff;
    out[1] = (in >> 8) & 0xff;
}

void static int32_to_le_bytes(int32_t in, uint8_t *const out)
{
    out[0] = in & 0xff;
    out[1] = (in >> 8) & 0xff;
    out[2] = (in >> 16) & 0xff;
    out[3] = (in >> 24) & 0xff;
}

uint16_t static le_bytes_to_uint16(uint8_t const *const bytes)
{
    uint16_t ret = 0;
    ret |= bytes[0];
    ret |= ((uint16_t)bytes[1]) << 8;
    return ret;
}

int32_t static le_bytes_to_int32(uint8_t const *const bytes)
{
    int32_t ret = 0;
    ret |= bytes[0];
    ret |= ((int32_t)bytes[1]) << 8;
    ret |= ((int32_t)bytes[2]) << 16;
    ret |= ((int32_t)bytes[3]) << 24;
    return ret;
}

uint64_t static le_bytes_to_uint64(uint8_t const *const bytes)
{
    uint64_t ret = 0;
    ret |= bytes[0];
    ret |= ((uint64_t)bytes[1]) << 8;
    ret |= ((uint64_t)bytes[2]) << 16;
    ret |= ((uint64_t)bytes[3]) << 24;
    ret |= ((uint64_t)bytes[4]) << 32;
    ret |= ((uint64_t)bytes[5]) << 40;
    ret |= ((uint64_t)bytes[6]) << 48;
    ret |= ((uint64_t)bytes[7]) << 56;
    return ret;
}
#pragma endregion

#pragma region Position
Position::Position(int32_t x, int32_t y, int32_t z, uint8_t qf) :
    x(x), y(y), z(z), qf(qf)
{
}

void Position::to_bytes(uint8_t *const bytes) const
{
    int32_to_le_bytes(x, bytes);
    int32_to_le_bytes(y, bytes + 4);
    int32_to_le_bytes(z, bytes + 8);
    bytes[12] = qf;
}

Position Position::from_bytes(uint8_t const *const bytes)
{
    return {
        le_bytes_to_int32(bytes),
        le_bytes_to_int32(bytes + 4),
        le_bytes_to_int32(bytes + 8),
        bytes[12]
    };
}
#pragma endregion

#pragma region FWVersion
FWVersion FWVersion::from_bytes(uint8_t const *const bytes)
{
    uint8_t maj = bytes[0];
    uint8_t min = bytes[1];
    uint8_t patch = bytes[2];
    uint8_t ver = bytes[3];
    
    return {maj, min, patch, ver};
}
#pragma endregion

#pragma region TagCfg
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
#pragma endregion

#pragma region IntCfg
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
#pragma endregion

#pragma region Status
Status Status::from_bytes(uint8_t const *const bytes)
{
    return {
        (bool)(bytes[0] & 0x01),
        (bool)(bytes[0] & 0x02),
        (bool)(bytes[0] & 0x04),
        (bool)(bytes[0] & 0x08),
        (bool)(bytes[0] & 0x20),
        (bool)(bytes[0] & 0x40),
        (bool)(bytes[0] & 0x80),
        (bool)(bytes[1] & 0x01)
    };
}
#pragma endregion

#pragma region DWM1001
DWM1001Error DWM1001::pos_set(Position const& position)
{
    uint8_t arg[13];
    position.to_bytes(arg);
    write_tlv(DWM1001TLV::POS_SET, 13, arg);

    return read_err();
}

DWM1001Error DWM1001::pos_get(Position *const position)
{
    write_tlv(DWM1001TLV::POS_GET, 0, nullptr);

    DWM1001Error err = read_err();    
    *position = read_pos_xyz();

    return err;
}

DWM1001Error DWM1001::upd_rate_set(uint16_t const ur, uint16_t const urs)
{
    uint8_t arg[4];
    uint16_to_le_bytes(ur, arg);
    uint16_to_le_bytes(urs, arg);
    
    write_tlv(DWM1001TLV::UR_SET, 4, arg);
    return read_err();
}

DWM1001Error DWM1001::upd_rate_get(uint16_t *const ur, uint16_t *const urs)
{
    write_tlv(DWM1001TLV::UR_GET, 0, nullptr);
    auto err = read_err();

    uint8_t type, length, value[256];
    read_tlv(&type, &length, value);
    // assert(type == DWM1001TLV::UR && length == 4);
    *ur = le_bytes_to_uint16(value);
    *urs = le_bytes_to_uint16(value + 2);

    return err;
}

DWM1001Error DWM1001::cfg_tag_set(TagCfg const& cfg)
{
    uint8_t arg[2];
    cfg.to_bytes(arg);
    write_tlv(DWM1001TLV::CFG_TN_SET, 2, arg);

    return read_err();
}

DWM1001Error DWM1001::sleep()
{
    write_tlv(DWM1001TLV::SLEEP, 0, nullptr);

    return read_err();
}

DWM1001Error DWM1001::factory_reset()
{
    write_tlv(DWM1001TLV::FAC_RESET, 0, nullptr);

    return read_err();
}

/*
 * @brief reboot the device
 */
DWM1001Error DWM1001::reset()
{
    write_tlv(DWM1001TLV::RESET, 0, nullptr);
    return read_err();
}

DWM1001Error DWM1001::usr_data_read(uint8_t *const data, uint8_t *const length)
{
    write_tlv(DWM1001TLV::USR_DATA_READ, 0, nullptr);
    auto err = read_err();

    uint8_t type;
    read_tlv(&type, length, data);
    // assert(type == DWM1001TLV::USR_DATA);

    return DWM1001Error::Ok;
}

DWM1001Error DWM1001::usr_data_write(uint8_t const *const data, uint8_t const length, bool overwrite)
{
    uint8_t length_ = length + 1, data_[35];
    memcpy(data_, data, length);
    data_[length] = (uint8_t)overwrite;
    write_tlv(DWM1001TLV::USR_DATA_WRITE, length_, data_);

    return read_err();
}

DWM1001Error DWM1001::gpio_cfg_output(GPIOIdx const idx, bool const value)
{
    uint8_t arg[2] = {(uint8_t)idx, (uint8_t)value};
    write_tlv(DWM1001TLV::GPIO_CFG_OUTPUT, 2, arg);

    return read_err();
}

DWM1001Error DWM1001::gpio_cfg_input(GPIOIdx const idx, GPIOPull const pull_mode)
{
    uint8_t arg[2] = {(uint8_t)idx, (uint8_t)pull_mode};
    write_tlv(DWM1001TLV::GPIO_CFG_INPUT, 2, arg);

    return read_err();
}

DWM1001Error DWM1001::gpio_value_set(GPIOIdx const idx, bool const value)
{
    uint8_t arg[2] = {(uint8_t)idx, (uint8_t)value};
    write_tlv(DWM1001TLV::GPIO_VAL_SET, 2, arg);

    return read_err();
}

DWM1001Error DWM1001::gpio_value_get(GPIOIdx const idx, bool *const value)
{
    write_tlv(DWM1001TLV::GPIO_VAL_GET, 1, (uint8_t *)&idx);
    auto err = read_err();
    uint8_t type, length, data[256];
    read_tlv(&type, &length, data);
    // assert(type == DWM1001TLV::GPIO_VALUE && length == 1);
    *value = data[0];
    return err;
}

DWM1001Error DWM1001::gpio_value_toggle(GPIOIdx const idx)
{
    write_tlv(DWM1001TLV::GPIO_VAL_TOGGLE, 1, (uint8_t *)&idx);
    return read_err();
}

DWM1001Error DWM1001::panid_set(uint16_t const panid)
{
    uint8_t arg[2];
    uint16_to_le_bytes(panid, arg);
    write_tlv(DWM1001TLV::PANID_SET, 2, arg);
    return read_err();
}

DWM1001Error DWM1001::panid_get(uint16_t *const panid)
{
    write_tlv(DWM1001TLV::PANID_GET, 0, nullptr);

    auto err = read_err();

    uint8_t type, length, buf[256];
    read_tlv(&type, &length, buf);
    // assert(type == DWM1001TLV::PANID && LENGTH == 2);
    *panid = le_bytes_to_uint16(buf);

    return err;
}

DWM1001Error DWM1001::nodeid_get(uint64_t *const nodeid)
{
    write_tlv(DWM1001TLV::NODE_ID_GET, 0, nullptr);

    auto err = read_err();

    uint8_t type, length, buf[256];
    read_tlv(&type, &length, buf);
    // assert(type == DWM1001TLV::NODE_ID && length == 8);

    *nodeid = le_bytes_to_uint64(buf);

    return err;
}

DWM1001Error DWM1001::status_get(Status *const status)
{
    write_tlv(DWM1001TLV::STATUS_GET, 0, nullptr);

    auto err = read_err();

    uint8_t type, length, buf[256];
    read_tlv(&type, &length, buf);
    // assert(type == DWM1001TLV::STATUS && length == 2);

    *status = Status::from_bytes(buf);

    return err;
}

DWM1001Error DWM1001::int_cfg_set(IntCfg const& cfg)
{
    uint8_t arg[2];
    cfg.to_bytes(arg);
    write_tlv(DWM1001TLV::INT_CFG_SET, 2, arg);

    return read_err();
}

void DWM1001::int_cfg_get(IntCfg *const cfg)
{
    write_tlv(DWM1001TLV::INT_CFG_GET, 0, nullptr);

    uint8_t type, length, buf[256];
    read_tlv(&type, &length, buf);
    // assert(type == DWM1001TLV::INT_CFG && length == 2);

    *cfg = IntCfg::from_bytes(buf);
}

DWM1001Error DWM1001::read_err()
{
    uint8_t type, length, buf[255];
    read_tlv(&type, &length, buf);
    // assert(type == DWM1001TLV::RET_VAL && length == 1);
    return static_cast<DWM1001Error>(buf[0]);
}

Position DWM1001::read_pos_xyz()
{
    uint8_t type, length, buf[255];
    read_tlv(&type, &length, buf);
    // assert(type == DWM1001TLV::POS_XYZ && length == 13);
    return Position::from_bytes(buf);
}
#pragma endregion
