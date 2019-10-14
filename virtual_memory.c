#include "virtual_memory.h"
#include "csr_func.h"
#include "memory_manager.h"
#include "string.h"

page_table_t kernel_root_page_table;

static inline uint16_t extract_vpn(const virtual_address_t v_address,
                                   const int level)
{
    const size_t offset = 12;
    const uint64_t mask = 0x1ff;
    size_t shift_amount = offset + 9 * level;
    uint64_t vpn = (v_address >> shift_amount) & mask;
    return (uint16_t)vpn;
}

static inline page_table_t
convert_into_page_table(const page_table_entry_t entry)
{
    page_table_t result;
    result = (page_table_t)(((uint64_t)entry.value >> 10) << 12);
    return result;
}

static inline page_table_entry_t
convert_into_table_entry(const page_table_t table)
{
    page_table_entry_t result;
    result.value = ((uint64_t)table >> 12) << 10;
    result.fields.access_control.valid = 1;
    return result;
}

page_table_entry_t *page_walk(const page_table_t root,
                              const virtual_address_t address)
{
    page_table_t current_table = root;
    for (int i = 2; i > 0; --i)
    {
        uint16_t current_level_vpn = extract_vpn(address, i);
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

    uint16_t vpn = extract_vpn(address, 0);
    return &current_table[vpn];
}

bool virtual_memory_map(page_table_t page_table, physical_address_t p_address,
                        virtual_address_t v_address, const size_t size,
                        const uint16_t permission)
{
    if (v_address & 0xfff)
    {
        // the address should be aligned
        return false;
    }
    if (p_address & 0xfff)
    {
        // the address should be aligned
        return false;
    }
    if (size & 0xfff)
    {
        return false;
    }

    uint64_t last_page_base = v_address + size;
    if (size == 0)
    {
        return true;
    }

    while (true)
    {
        page_table_entry_t *entry =
            page_walk(page_table, v_address);
        if (entry == NULL)
        {
            return false;
        }

        *entry = convert_into_table_entry((page_table_t)p_address);
        entry->fields.access_control.valid = 1;
        entry->value |= permission;

        v_address += 0x1000;
        p_address += 0x1000;
        if (v_address == last_page_base)
        {
            break;
        }
    }

    return true;
}

bool map_kernel_virtual_memory(physical_address_t p_address,
                               virtual_address_t v_address, const size_t size,
                               const uint16_t permission)
{
    if (v_address & 0xfff)
    {
        // the address should be aligned
        return false;
    }
    if (p_address & 0xfff)
    {
        // the address should be aligned
        return false;
    }
    if (size & 0xfff)
    {
        return false;
    }

    uint64_t last_page_base = v_address + size;
    if (size == 0)
    {
        return true;
    }

    while (true)
    {
        page_table_entry_t *entry =
            page_walk(kernel_root_page_table, v_address);
        if (entry == NULL)
        {
            return false;
        }

        *entry = convert_into_table_entry((page_table_t)p_address);
        entry->fields.access_control.valid = 1;
        entry->value |= permission;

        v_address += 0x1000;
        p_address += 0x1000;
        if (v_address == last_page_base)
        {
            break;
        }
    }

    return true;
}

void write_page_table(const page_table_t page_table)
{
    satp_t satp;

    uint64_t ppn = ((uint64_t)page_table) >> 12;
    satp.ppn = ppn;
    satp.mode = SATP_MODE_SV39;
    satp.asid = 0;

    csr_write_satp(satp.value);

    // tlb flush
    __asm__ volatile("sfence.vma x0, x0");
}

bool init_virtual_memory(void)
{
    kernel_root_page_table = (page_table_t)kalloc_4k();
    if (kernel_root_page_table == NULL)
    {
        return false;
    }

    memory_set(kernel_root_page_table, 0x00, sizeof(page_table_t) * 512);

    // DRAM
    map_kernel_virtual_memory(0x80000000, 0x80000000, 0x800000, 0b1111);

    // PLIC
    map_kernel_virtual_memory(0xc000000, 0xc000000, 0x4000000, 0b0110);

    // UART
    map_kernel_virtual_memory(0x10000000, 0x10000000, (0x100 + 0xfff) & -0x1000,
                              0b0110);

    write_page_table(kernel_root_page_table);

    return true;
}
