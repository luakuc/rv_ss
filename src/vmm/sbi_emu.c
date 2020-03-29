#include "sbi_emu.h"

//legacy sbi extension
#define SBI_EXT_ID_SBI_TIMER 0x00
#define SBI_EXT_ID_PUT_CHAR 0x01
#define SBI_EXT_ID_GET_CHAR 0x02
#define SBI_EXT_ID_CLEAR_IPI 0x03
#define SBI_EXT_ID_SEND_IPI 0x04
#define SBI_EXT_ID_REMOTE_FENCE_I 0x05
#define SBI_EXT_ID_REMOTE_SFENCE_VMA 0x06
#define SBI_EXT_ID_REMOTE_SFENCE_VMA_ASID 0x07
#define SBI_EXT_ID_SHUTDOWN 0x08
// base sbi
#define SBI_EXT_ID_BASE_FUNC 0x10

#define SBI_FUNC_ID_GET_SPEC 0x0
#define SBI_FUNC_ID_IMPL_ID 0x1
#define SBI_FUNC_ID_IMPL_VERSION 0x2
#define SBI_FUNC_ID_PROBE_EXT 0x3
#define SBI_FUNC_ID_MVENDORID 0x4
#define SBI_FUNC_ID_MARCHID 0x05
#define SBI_FUNC_ID_MIMPID 0x6

#define SBI_IMPLEMENTATION_ID 0x10 // rvss sbi

bool emulate_sbi_call(virtual_cpu_t* vcpu)
{
    uint64_t extension_id = vcpu->guest_context.a7;
    //uint64_t function_id = vcpu->guest_context.a6;

    switch(extension_id)
    {
        case SBI_EXT_ID_SBI_TIMER:
        {
            //TODO
            return true;
            break;
        }
        default:
        {
            // not yet implemented
            return false;
        }

    }

    return true;
}
