#include "inst_emu.h"
#include "instruction.h"
#include "mmu.h"
#include "plic_emu.h"
#include "uart_emu.h"
#include "virtual_memory.h"

static uint64_t *get_register_address(virtual_cpu_t *vcpu, size_t offset)
{
    return (uint64_t *)vcpu + offset;
}

static bool store_emulation(virtual_cpu_t *vcpu, uint64_t target_address,
                            uint64_t value, uint8_t width)
{
    // plic range
    const uint64_t plic_base = 0x0c000000;
    const uint64_t plic_end = 0x0c000000 + 0x04000000;
    if (target_address >= plic_base && target_address < plic_end)
    {
        // uint64_t offset = target_address - plic_base;
        // bool result = plic_emulate_store(vcpu, target_address, value, width);
        bool result = plic_emulate_store(vcpu, target_address, value, width);
        if (!result)
        {
            return false;
        }

        return true;
    }

    const uint64_t uart0_base = 0x10000000;
    const uint64_t uart0_end = uart0_base + 0x100;
    if (uart0_base <= target_address && target_address < uart0_end)
    {
        // TODO
        uint64_t offset = target_address - uart0_base;
        bool result = uart_emulation_store(vcpu, offset, value, width);
        if (!result)
        {
            return false;
        }

        return true;
    }

    return false;
}

static bool load_emulation(virtual_cpu_t *vcpu, uint64_t target_address,
                           uint64_t *value, uint8_t width)
{
    // const uint64_t plic_base = 0x0c000000;
    // const uint64_t plic_end = 0x0c000000 + 0x04000000;
    // if (target_address >= plic_base && target_address < plic_end)
    //{
    // uint64_t offset = target_address - plic_base;
    // bool result = plic_emulate_load(vcpu, offset, value, width);
    //    bool result = plic_emulate_load(vcpu, target_address, value, width);
    //    if (!result)
    //    {
    //        return false;
    //    }

    //    return true;
    //}

    // uart register range
    const uint64_t uart0_base = 0x10000000;
    const uint64_t uart0_end = uart0_base + 0x100;
    if (uart0_base <= target_address && target_address < uart0_end)
    {
        uint64_t offset = target_address - uart0_base;
        bool result = uart_emulation_load(vcpu, offset, value, width);
        if (!result)
        {
            return false;
        }

        return true;
    }

    return false;
}

bool read_guest_instuction(virtual_cpu_t* vcpu, uint64_t address, uint32_t* instruction)
{
    uint64_t inst_page_base = (uint64_t)address & -0x1000;
    uint64_t inst_offset = (uint64_t)address & (0x1000 - 1);
    page_table_entry_t *entry =
        page_walk_sv39x4(vcpu->gp_hp_page_table, inst_page_base);
    if (entry == NULL)
    {
        return false;
    }

    uintptr_t inst_address = convert_into_page_base(*entry) + inst_offset;

    *instruction = *(uint32_t *)inst_address;
    return true;
}

bool instruction_emulation(virtual_cpu_t *vcpu, uint64_t instruction_address)
{
    uint32_t instruction;
    bool result = read_guest_instuction(vcpu, instruction_address, &instruction);
    if(!result)
    {
        return false;
    }

    uint32_t opcode = instruction & RV64_OPCODE_MASK;

    uint16_t width = (instruction & RV64_LOAD_STORE_WIDTH_MASK) >>
                     RV64_LOAD_STORE_WIDTH_SHIFT_AMOUNT;

    uint16_t base = (instruction & RV64_LOAD_STORE_BASE_MASK) >>
                    RV64_LOAD_STORE_BASE_SHIFT_AMOUNT;

    uint64_t *base_reg_ptr = get_register_address(vcpu, base);

    switch (opcode)
    {
        case RV64_OPCODE_STORE:
        {
            uint16_t src = (instruction & RV64_STORE_SRC_MASK) >>
                           RV64_STORE_SRC_SHIFT_AMOUNT;
            uint64_t *src_reg_ptr = get_register_address(vcpu, src);

            uint16_t offset0 = (instruction & RV64_LOAD_STORE_DEST_MASK) >>
                               RV64_STORE_OFFSET0_SHIFT_AMOUNT;
            uint16_t offset5 = (instruction & RV64_STORE_OFFSET_MASK) >>
                               RV64_STORE_OFFSET5_SHIFT_AMOUNT;

            uint16_t offset = offset5 << 5 | offset0;
            int16_t sign_extended_offset = offset | -(offset & 0x800);

            uint64_t target = (uint64_t)*base_reg_ptr + sign_extended_offset;

            return store_emulation(vcpu, target, *src_reg_ptr, width);
        }
        case RV64_OPCODE_LOAD:
        {
            uint16_t dest = (instruction & RV64_LOAD_STORE_DEST_MASK) >>
                            RV64_LOAD_DEST_SHIFT_AMOUNT;

            uint16_t offset = (instruction & RV64_LOAD_OFFSET_MASK) >>
                              RV64_LOAD_OFFSET_SHIFT_AMOUNT;

            int32_t sign_extended_offset = offset | -(offset & 0x800);
            uint64_t target = *base_reg_ptr + sign_extended_offset; // TODO

            uint64_t read_from_device;
            bool result =
                load_emulation(vcpu, target, &read_from_device, width);
            if (!result)
            {
                return false;
            }

            // TODO 64bit sign exntend the loaded value;

            uint64_t *dest_reg = get_register_address(vcpu, dest);
            *dest_reg = read_from_device;

            return true;
        }
        default:
        {
            // TODO
            // what is happen?
        }
    }

    return false;
}
