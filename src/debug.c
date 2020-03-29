#include "debug.h"
#include "csr_func.h"
#include "string.h"
#include "io_interface.h"

void dump_csrs(void)
{
    uint64_t scause = csr_read_scause();
    uint64_t stval = csr_read_stval();
    uint64_t sepc = csr_read_sepc();
    uint64_t sscratch = csr_read_sscratch();
    uint64_t sstatus = csr_read_sstatus();

    char text[9];

    int_to_str(scause, text);
    put_string("scause\n0x");
    put_string(text);
    put_char('\n');

    int_to_str(stval, text);
    put_string("stval\n0x");
    put_string(text);
    put_char('\n');

    int_to_str(sepc, text);
    put_string("sepc\n0x");
    put_string(text);
    put_char('\n');

    int_to_str(sscratch, text);
    put_string("sscratch\n0x");
    put_string(text);
    put_char('\n');

    int_to_str(sstatus, text);
    put_string("sstatus\n0x");
    put_string(text);
    put_char('\n');
}
