#include "guest_address_translation.h"

static inline uint16_t extract_vpn_sv39_x4(const virtual_address_t v_address,
                                   const int level)
{
    const size_t offset = 12;
    const uint64_t mask = level == 2 ? 0x3ff : 0x1ff;

    size_t shift_amount = offset + 9 * level;

    uint64_t vpn = (v_address >> shift_amount) & mask;
    return (uint16_t)vpn;
}

