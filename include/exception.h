#pragma once

enum interrupt_code
{
    user_software_interrupt = 0,
    supervisor_software_interrupt = 1,
    // 2 and 3 are reserved

    // timer interrupts
    user_timer_interrupt = 4,
    supervisor_timer_interrupt = 5,
    // 6 and 7 are reserved

    // external interrupt
    user_external_interrupt = 8,
    supervisor_external_interrupt = 9,
    // over 10 are reserved for platform use.
};

enum exception_code
{
    instruction_addres_misaligned = 0,
    instruction_access_fault = 1,
    illegal_instruction = 2,
    breakpoint = 3,
    load_address_misaligned = 4,
    load_access_fault = 5,
    store_amo_address_misaligned = 6,
    store_amo_access_fault = 7,
    environment_call_from_u = 8,
    //environment_call_from_s = 9,
    environment_call_from_hs = 9,
    environment_call_from_vs = 10,
    environment_call_from_m = 11,

    // 10 and 11 are reserved
    instruction_page_fault = 12,
    load_page_fault = 13,
    // 14 is reserved for future standard use
    store_amo_page_fault = 15,
    // over 17 are reserved
};

char *convert_interrupt_code_to_string(enum interrupt_code code)
{
    switch (code)
    {
        case user_software_interrupt:
            return "user_timer_interrupt";
        case supervisor_software_interrupt:
            return "supervisor_software_interrupt";
        case user_timer_interrupt:
            return "user_timer_interrupt";
        case supervisor_timer_interrupt:
            return "supervisor_timer_interrupt";
        case user_external_interrupt:
            return "user_external_interrupt";
        case supervisor_external_interrupt:
            return "supervisor_external_interrupt";
        default:
            return "unknown";
    }
}

char *convert_exception_code_to_string(enum exception_code code)
{
    switch (code)
    {
        case instruction_addres_misaligned:
            return "instruction_addres_misaligned";
        case instruction_access_fault:
            return "instruction_access_fault";
        case illegal_instruction:
            return "illegal_instruction";
        case breakpoint:
            return "breakpoint";
        case load_address_misaligned:
            return "load_address_misaligned";
        case load_access_fault:
            return "load_access_fault";
        case store_amo_address_misaligned:
            return "store_amo_address_misaligned";
        case store_amo_access_fault:
            return "store_amo_access_fault";
        case environment_call_from_u:
            return "environment_call_from_u";
        case environment_call_from_hs:
            return "environment_call_from_hs";
        case environment_call_from_vs:
            return "environment_call_from_vs";
        case environment_call_from_m:
            return "environment_call_from_m";
        case instruction_page_fault:
            return "instruction_page_fault";
        case load_page_fault:
            return "load_page_fault";
        case store_amo_page_fault:
            return "store_amo_page_fault";
        default:
            return "unknown";
    }
}
