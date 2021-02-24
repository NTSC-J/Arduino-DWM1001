#include "DWM1001.h"

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
