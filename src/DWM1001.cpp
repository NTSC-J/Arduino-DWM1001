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

DWM1001Error DWM1001::cfg_get(NodeCfg *const cfg)
{
    write_tlv(DWM1001TLV::CFG_GET, 0, nullptr);
    auto err = read_err();

    uint8_t type, length, value[256];
    read_tlv(&type, &length, value);
    // assert(type == DWM1001TLV::CFG && length == 2);
    *cfg = NodeCfg::from_bytes(value);

    return err;
}

DWM1001Error DWM1001::sleep()
{
    write_tlv(DWM1001TLV::SLEEP, 0, nullptr);

    return read_err();
}

DWM1001Error DWM1001::stnry_cfg_set(StnrySensitivity const& sentitivity)
{
    write_tlv(DWM1001TLV::STNRY_CFG_SET, 1, (uint8_t *)&sentitivity);

    return read_err();
}

DWM1001Error DWM1001::stnry_cfg_get(StnrySensitivity *const sensitivity)
{
    write_tlv(DWM1001TLV::STNRY_CFG_GET, 0, nullptr);

    auto err = read_err();
    uint8_t type, length, buf[256];
    read_tlv(&type, &length, buf);
    // assert(type == DWM1001TLV::STNRY_CFG && length == 1);
    *sensitivity = (StnrySensitivity)buf[0];

    return err;
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

    return err;
}

DWM1001Error DWM1001::usr_data_write(uint8_t const *const data, uint8_t const length, bool overwrite)
{
    uint8_t length_ = length + 1, data_[DWM_USR_DATA_LEN_MAX + 1];
    data_[0] = (uint8_t)overwrite;
    memcpy(data_ + 1, data, length);
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
    auto err = static_cast<DWM1001Error>(buf[0]);

#ifdef DEBUG
#ifndef ARDUINO
    if (err != DWM1001Error::Ok) {
        printf("ERROR: %d\n", (uint8_t)err);
    }
#endif
#endif

    return err;
}

Position DWM1001::read_pos_xyz()
{
    uint8_t type, length, buf[255];
    read_tlv(&type, &length, buf);
    // assert(type == DWM1001TLV::POS_XYZ && length == 13);
    return Position::from_bytes(buf);
}
