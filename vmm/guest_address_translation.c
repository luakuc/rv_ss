#include "guest_address_translation.h"
#include "csr_func.h"
#include "memory_manager.h"

static uint16_t extract_vpn_sv39_x4(const virtual_address_t v_address,
                                    const int level)
{
    const size_t offset = 12;
    const uint64_t mask = level == 2 ? 0x3ff : 0x1ff;

    size_t shift_amount = offset + 9 * level;

    uint64_t vpn = (v_address >> shift_amount) & mask;
    return (uint16_t)vpn;
}

page_table_entry_t *page_walk_sv39x4(page_table_t root,
                                     const virtual_address_t address)
{
    page_table_t current_table = root;
    for (int i = 2; i > 0; --i)
    {
        uint16_t current_level_vpn = extract_vpn_sv39_x4(address, i);
        page_table_entry_t *entry = &current_table[current_level_vpn];

        if (!entry->fields.access_control.valid)
        {
            page_table_t new_page_table = kalloc_4k();
            if (new_page_table == NULL)
            {
                return NULL;
            }
            *entry = convert_into_table_entry(new_page_table);
        }

        current_table = convert_into_page_table(*entry);
    }

    uint16_t vpn = extract_vpn_sv39_x4(address, 0);
    return &current_table[vpn];
}

bool guest_memory_map(page_table_t root, uint64_t guest_physical,
                      uint64_t host_phisical, size_t size, uint16_t permission)
{
    if (guest_physical & 0xfff)
    {
        return false;
    }

    if (host_phisical & 0xfff)
    {
        return false;
    }

    if (size & 0xfff)
    {
        return false;
    }

    while (size > 0)
    {
        page_table_entry_t *entry = page_walk_sv39x4(root, guest_physical);
        if (entry == NULL)
        {
            return false;
        }

        *entry = convert_into_table_entry((page_table_t)host_phisical);

        entry->fields.access_control.valid = 1;
        entry->value |= permission;

        guest_physical += 0x1000;
        host_phisical += 0x1000;
        size -= 0x1000;
    }

    return true;
}

void update_hgatp(page_table_t table)
{
    hgatp_t hgatp;
    hgatp.mode = HGATP_MODE_SV39X4;
    hgatp.vmid = 0;
    hgatp.ppn = (uint64_t)table >> 12;

    csr_write_hgatp(hgatp.value);
}
