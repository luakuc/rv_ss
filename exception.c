#include "exception.h"

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
