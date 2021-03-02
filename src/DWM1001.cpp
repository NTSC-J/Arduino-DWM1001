#include "DWM1001.h"

#ifdef ARDUINO
#include <string.h>
#else
#include <cstring>
#ifdef DEBUG
#include <cstdio>
#endif
#endif

DWM1001Error DWM1001::pos_set(Position const& position)
{
    uint8_t arg[13];
    position.to_bytes(arg);
    write_tlv(DWM1001TLV::POS_SET, 13, arg);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::pos_get(Position *const position)
{
    write_tlv(DWM1001TLV::POS_GET, 0, nullptr);

    read_all_resp();
    *position = parse_pos_xyz();

    return parse_err();
}

DWM1001Error DWM1001::upd_rate_set(uint16_t const ur, uint16_t const urs)
{
    uint8_t arg[4];
    uint16_to_le_bytes(ur, arg);
    uint16_to_le_bytes(urs, arg);
    
    write_tlv(DWM1001TLV::UR_SET, 4, arg);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::upd_rate_get(uint16_t *const ur, uint16_t *const urs)
{
    write_tlv(DWM1001TLV::UR_GET, 0, nullptr);

    read_all_resp();

    auto data = last_tlv_data[1];
    auto type = data[0], length = data[1], *value = data + 2;

    // assert(type == DWM1001TLV::UR && length == 4);
    *ur = le_bytes_to_uint16(value);
    *urs = le_bytes_to_uint16(value + 2);

    return parse_err();
}

DWM1001Error DWM1001::cfg_tag_set(TagCfg const& cfg)
{
    uint8_t arg[2];
    cfg.to_bytes(arg);
    write_tlv(DWM1001TLV::CFG_TN_SET, 2, arg);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::cfg_anchor_set(AnchorCfg const& cfg)
{
    uint8_t arg[2];
    cfg.to_bytes(arg);
    write_tlv(DWM1001TLV::CFG_AN_SET, 2, arg);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::cfg_get(NodeCfg *const cfg)
{
    write_tlv(DWM1001TLV::CFG_GET, 0, nullptr);

    read_all_resp();

    auto data = last_tlv_data[1];
    auto type = data[0], length = data[1], *value = data + 2;
    // assert(type == DWM1001TLV::CFG && length == 2);
    *cfg = NodeCfg::from_bytes(value);

    return parse_err();
}

DWM1001Error DWM1001::sleep()
{
    write_tlv(DWM1001TLV::SLEEP, 0, nullptr);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::stnry_cfg_set(StnrySensitivity const& sentitivity)
{
    write_tlv(DWM1001TLV::STNRY_CFG_SET, 1, (uint8_t *)&sentitivity);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::stnry_cfg_get(StnrySensitivity *const sensitivity)
{
    write_tlv(DWM1001TLV::STNRY_CFG_GET, 0, nullptr);

    read_all_resp();

    auto data = last_tlv_data[1];
    auto type = data[0], length = data[1], *value = data + 2;

    // assert(type == DWM1001TLV::STNRY_CFG && length == 1);
    *sensitivity = (StnrySensitivity)value[0];

    return parse_err();
}

DWM1001Error DWM1001::factory_reset()
{
    write_tlv(DWM1001TLV::FAC_RESET, 0, nullptr);

    read_all_resp();

    return parse_err();
}

/*
 * @brief reboot the device
 */
DWM1001Error DWM1001::reset()
{
    write_tlv(DWM1001TLV::RESET, 0, nullptr);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::usr_data_read(uint8_t *const data, uint8_t *const length)
{
    write_tlv(DWM1001TLV::USR_DATA_READ, 0, nullptr);

    read_all_resp();

    auto d = last_tlv_data[1];
    auto type = d[0], *value = d + 2;
    *length = d[1];
    // assert(type == DWM1001TLV::USR_DATA);
    memcpy(data, d, *length);

    return parse_err();
}

DWM1001Error DWM1001::usr_data_write(uint8_t const *const data, uint8_t const length, bool overwrite)
{
    uint8_t length_ = length + 1, data_[DWM_USR_DATA_LEN_MAX + 1];
    data_[0] = (uint8_t)overwrite;
    memcpy(data_ + 1, data, length);
    write_tlv(DWM1001TLV::USR_DATA_WRITE, length_, data_);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::gpio_cfg_output(GPIOIdx const idx, bool const value)
{
    uint8_t arg[2] = {(uint8_t)idx, (uint8_t)value};
    write_tlv(DWM1001TLV::GPIO_CFG_OUTPUT, 2, arg);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::gpio_cfg_input(GPIOIdx const idx, GPIOPull const pull_mode)
{
    uint8_t arg[2] = {(uint8_t)idx, (uint8_t)pull_mode};
    write_tlv(DWM1001TLV::GPIO_CFG_INPUT, 2, arg);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::gpio_value_set(GPIOIdx const idx, bool const value)
{
    uint8_t arg[2] = {(uint8_t)idx, (uint8_t)value};
    write_tlv(DWM1001TLV::GPIO_VAL_SET, 2, arg);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::gpio_value_get(GPIOIdx const idx, bool *const value)
{
    write_tlv(DWM1001TLV::GPIO_VAL_GET, 1, (uint8_t *)&idx);

    auto data = last_tlv_data[1];
    auto type = data[0], length = data[1];
    // assert(type == DWM1001TLV::GPIO_VALUE && length == 1);
    *value = data[2];
    return parse_err();
}

DWM1001Error DWM1001::gpio_value_toggle(GPIOIdx const idx)
{
    write_tlv(DWM1001TLV::GPIO_VAL_TOGGLE, 1, (uint8_t *)&idx);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::panid_set(uint16_t const panid)
{
    uint8_t arg[2];
    uint16_to_le_bytes(panid, arg);
    write_tlv(DWM1001TLV::PANID_SET, 2, arg);

    read_all_resp();

    return parse_err();
}

DWM1001Error DWM1001::panid_get(uint16_t *const panid)
{
    write_tlv(DWM1001TLV::PANID_GET, 0, nullptr);

    read_all_resp();
    
    auto data = last_tlv_data[1];
    auto type = data[0], length = data[1], *value = data + 2;
    // assert(type == DWM1001TLV::PANID && LENGTH == 2);
    *panid = le_bytes_to_uint16(value);

    return parse_err();
}

DWM1001Error DWM1001::nodeid_get(uint64_t *const nodeid)
{
    write_tlv(DWM1001TLV::NODE_ID_GET, 0, nullptr);

    read_all_resp();

    auto data = last_tlv_data[1];
    auto type = data[0], length = data[1], *value = data + 2;
    // assert(type == DWM1001TLV::NODE_ID && length == 8);

    *nodeid = le_bytes_to_uint64(value);

    return parse_err();
}

DWM1001Error DWM1001::status_get(Status *const status)
{
    write_tlv(DWM1001TLV::STATUS_GET, 0, nullptr);

    read_all_resp();

    auto data = last_tlv_data[1];
    auto type = data[0], length = data[1], *value = data + 2;
    // assert(type == DWM1001TLV::STATUS && length == 2);

    *status = Status::from_bytes(value);

    return parse_err();
}

DWM1001Error DWM1001::int_cfg_set(IntCfg const& cfg)
{
    uint8_t arg[2];
    cfg.to_bytes(arg);
    write_tlv(DWM1001TLV::INT_CFG_SET, 2, arg);

    read_all_resp();

    return parse_err();
}

void DWM1001::int_cfg_get(IntCfg *const cfg)
{
    write_tlv(DWM1001TLV::INT_CFG_GET, 0, nullptr);

    read_all_resp();

    auto data = last_tlv_data[1];
    auto type = data[0], length = data[1], *value = data + 2;
    // assert(type == DWM1001TLV::INT_CFG && length == 2);

    *cfg = IntCfg::from_bytes(value);
}

DWM1001Error DWM1001::parse_err(uint8_t page)
{
    auto data = last_tlv_data[page];
    auto type = data[0], length = data[1], *value = data + 2;
    // assert(type == DWM1001TLV::RET_VAL && length == 1);
    auto err = static_cast<DWM1001Error>(value[0]);

#ifdef DEBUG
#ifndef ARDUINO
    if (err != DWM1001Error::Ok) {
        printf("ERROR: %d\n", (uint8_t)err);
    }
#endif
#endif

    return err;
}

Position DWM1001::parse_pos_xyz(uint8_t page)
{
    auto data = last_tlv_data[page];
    auto type = data[0], length = data[1], *value = data + 2;
    // assert(type == DWM1001TLV::POS_XYZ && length == 13);
    return Position::from_bytes(value);
}
