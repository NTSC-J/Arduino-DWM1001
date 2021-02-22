#include "DWM1001.h"

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

int16_t static le_bytes_to_int16(uint8_t const *const bytes)
{
    int16_t ret = 0;
    ret |= bytes[0];
    ret |= (bytes[1] << 8);
    return ret;
}

int32_t static le_bytes_to_int32(uint8_t const *const bytes)
{
    int32_t ret = 0;
    ret |= bytes[0];
    ret |= (bytes[1] << 8);
    ret |= (bytes[2] << 16);
    ret |= (bytes[3] << 24);
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
    *ur = le_bytes_to_int16(value);
    *urs = le_bytes_to_int16(value + 2);

    return err;
}

DWM1001Error DWM1001::cfg_tag_set(TagCfg const& cfg)
{
    return DWM1001Error::Ok;
}

DWM1001Error DWM1001::usr_data_read(uint8_t *const length, uint8_t *const data)
{
    return DWM1001Error::Ok;
}

DWM1001Error DWM1001::usr_data_write(uint8_t const length, uint8_t const *const data, bool overwrite)
{
    return DWM1001Error::Ok;
}

DWM1001Error DWM1001::panid_set(uint16_t const panid)
{
    return DWM1001Error::Ok;
}

DWM1001Error DWM1001::panid_get(uint16_t *const panid)
{
    return DWM1001Error::Ok;
}

DWM1001Error DWM1001::nodeid_get(uint64_t *const nodeid)
{
    return DWM1001Error::Ok;
}

DWM1001Error DWM1001::int_cfg_set(IntCfg const& cfg)
{
    return DWM1001Error::Ok;
}

DWM1001Error DWM1001::int_cfg_get(IntCfg *const cfg)
{
    return DWM1001Error::Ok;
}

/*
 * @brief reboot the device
 */
DWM1001Error DWM1001::reset()
{
    write_tlv(DWM1001TLV::RESET, 0, nullptr);
    return read_err();
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
