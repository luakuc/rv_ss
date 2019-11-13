#include "fdt.h"
#include "utils.h"

//FDT spec is included in device tree specification.
//https://www.devicetree.org/specifications/

#define FDT_HEADER_MAGIC    0xd00dfeed

#define FDT_BEGIN_NODE  0x00000001
#define FDT_END_NODE    0x00000002
#define FDT_PROP        0x00000003
#define FDT_NOP         0x00000004
#define FDT_END         0x00000009

typedef struct fdt_header {
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
} fdt_header_t;

typedef struct memory_reserved_entry {
    uint64_t addres;
    uint64_t size;
} fdt_reserve_entry_t;

typedef uint32_t fdt_struct_entry_t;

bool init_fdt(uint64_t fdt_base)
{
    fdt_header_t *header = (fdt_header_t*)fdt_base;

    uint32_t magic = big2little_32(header->magic);
    if(magic != FDT_HEADER_MAGIC)
    {
        return false;
    }

    uint32_t totalsize = big2little_32(header->totalsize);
    uint32_t offset_to_struct = big2little_32(header->off_dt_struct);
    uint32_t offset_to_string = big2little_32(header->off_dt_strings);
    uint32_t offset_to_memory = big2little_32(header->off_mem_rsvmap);

    fdt_reserve_entry_t* memory_blocks = (fdt_reserve_entry_t*)fdt_base + offset_to_memory;

    fdt_struct_entry_t* struct_blocks = (fdt_struct_entry_t*)fdt_base + offset_to_struct;


    return false;
}
