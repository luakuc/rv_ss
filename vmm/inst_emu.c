#include "inst_emu.h"
#include "instruction.h"
#include "mmu.h"
#include "virtual_memory.h"

bool instruction_emu_store(const page_table_t page_table,
                           uint64_t instruction_address)
{
    uint64_t inst_page_base = (uint64_t)instruction_address & -0x1000;
    uint64_t offset = (uint64_t)instruction_address & (0x1000 - 1);
    page_table_entry_t *entry = page_walk_sv39x4(page_table, inst_page_base);
    if (entry == NULL)
    {
        return false;
    }

    uintptr_t inst_address = convert_into_page_base(*entry) + offset;

    uint32_t instruction = *(uint32_t *)inst_address;
    uint32_t opcode = instruction & RV64_OPCODE_MASK;

    switch (opcode)
    {
        case RV64_OPCODE_LOAD:
        {
            break;
        }
        case RV64_OPCODE_STORE:
        {
            break;
        }
        default:
        {
            // unexpected opecode
            return false;
        }
    }

    return false;
}
