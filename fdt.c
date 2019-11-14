#include "fdt.h"
#include "memory_manager.h"
#include "string.h"
#include "utils.h"

// FDT spec is included in device tree specification.
// https://www.devicetree.org/specifications/

#define FDT_HEADER_MAGIC 0xd00dfeed

#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE 0x00000002
#define FDT_PROP 0x00000003
#define FDT_NOP 0x00000004
#define FDT_END 0x00000009

typedef struct fdt_header
{
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

typedef struct memory_reserved_entry
{
    uint64_t addres;
    uint64_t size;
} fdt_reserve_entry_t;

typedef uint32_t fdt_struct_entry_t;

typedef struct property
{
    struct property *next;
    char *value;
    char *name;
} property_t;

typedef struct devicetree
{
    char *name;
    property_t *properties;
    struct devicetree *subnodes;
} device_tree_t;

typedef struct property_info
{
    uint32_t len;
    uint32_t nameoff;
} property_info_t;

static device_tree_t *device_tree;

static fdt_struct_entry_t *memory_block;
static int struct_index = 0;
static fdt_struct_entry_t *struct_block;
static char *string_block;

static char *get_next_name(int *index)
{

    struct list
    {
        uint32_t value;
        struct list *next;
    };

    int list_length = 1;
    char* name;
    struct list *head = (struct list *)kalloc(sizeof(struct list));
    head->value = big2little_32(struct_block[(*index)++]);
    head->next = NULL;

    struct list *current = head;

    do
    {
        for (int i = 0; i < sizeof(uint32_t); ++i)
        {
            char c = (current->value << (i * 8)) >> 24;
            if (c == '\0')
            {
                goto reach_to_end;
            }
        }

        struct list *next = (struct list *)kalloc(sizeof(struct list));
        next->value = big2little_32(struct_block[(*index)++]);
        next->next = NULL;
        current->next = next;

        list_length++;
    } while (true);

reach_to_end:

    name = (char *)kalloc(sizeof(uint32_t) * list_length);
    memory_set(name, 0x00, sizeof(uint32_t));

    current = head;

    while (current->next != NULL)
    {
        memory_copy(name, &(current->value), sizeof(uint32_t));
        current = current->next;
    }

    return name;
}

static property_info_t* parse_property(int *index)
{
    //TODO
}

static device_tree_t *parse_node(int index)
{
    uint32_t tmp = struct_block[index++];
    fdt_struct_entry_t entry = big2little_32(tmp);

    if (entry != FDT_BEGIN_NODE)
    {
        return NULL;
    }

    device_tree_t *dt = kalloc(sizeof(device_tree_t));
    if (!dt)
    {
        return NULL;
    }
    memory_set(dt, 0x00, sizeof(device_tree_t));

    char* name = get_next_name(&index);
    if(!name)
    {
        return NULL;
    }
    dt->name = name;

    while(true)
    {
        uint32_t tmp = struct_block[index++];
        fdt_struct_entry_t entry = big2little_32(tmp);
        switch(entry)
        {
            case FDT_BEGIN_NODE:
            {
                //TODO
                break;
            }
            case FDT_END_NODE:
            {
                goto close_node;
            }
            case FDT_PROP:
            {
                property_info_t* prop = parse_property(&index);
                break;
            }
            case FDT_NOP:
            {
                // do nothing
                break;
            }
            default:
                return NULL;
        }
    }

close_node:
    return dt;
}

bool init_fdt(uint64_t fdt_base)
{
    fdt_header_t *header = (fdt_header_t *)fdt_base;

    uint32_t magic = big2little_32(header->magic);
    if (magic != FDT_HEADER_MAGIC)
    {
        return false;
    }

    uint32_t totalsize = big2little_32(header->totalsize);
    uint32_t offset_to_struct = big2little_32(header->off_dt_struct);
    uint32_t offset_to_string = big2little_32(header->off_dt_strings);
    uint32_t offset_to_memory = big2little_32(header->off_mem_rsvmap);

    memory_block = (fdt_reserve_entry_t *)(fdt_base + offset_to_memory);

    struct_block = (fdt_struct_entry_t *)(fdt_base + offset_to_struct);

    string_block = (char *)(fdt_base + offset_to_string);

    device_tree = parse_node(0);

    return false;
}
