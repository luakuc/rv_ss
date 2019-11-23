#include "fdt.h"
#include "endian.h"
#include "fdt_core.h"
#include "memory_manager.h"
#include "string.h"

static device_tree_t *device_tree;

static fdt_reserve_entry_t *memory_block;
static fdt_struct_entry_t *struct_block;
static char *string_block;

static uint64_t fdt_base_address = 0;

char *get_next_name(int *index)
{
    struct list
    {
        uint32_t value;
        struct list *next;
    };

    int list_length = 1;
    char *name;
    struct list *head = (struct list *)kalloc(sizeof(struct list));
    head->value = struct_block[(*index)++];
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
        next->value = struct_block[(*index)++];
        next->next = NULL;
        current->next = next;
        current = next;

        list_length++;
    } while (true);

reach_to_end:

    name = (char *)kalloc(sizeof(uint32_t) * list_length);
    memory_set(name, 0x00, sizeof(uint32_t));

    current = head;

    int offset = 0;
    while (current != NULL)
    {
        // memory_copy(name + offset, &(current->value), sizeof(uint32_t));
        *(uint32_t *)(name + offset) = current->value;
        offset += sizeof(uint32_t);
        current = current->next;
    }

    return name;
}

char *get_string(int offset)
{
    // int base = offset;
    char *string;

    int offset_diff_from_u32 = offset % sizeof(uint32_t);
    int start = offset - offset_diff_from_u32;
    int length = -offset_diff_from_u32;

    int index = 0;
    while (true)
    {
        uint32_t tmp = *(uint32_t *)&string_block[start + index];
        index += 4;
        uint32_t str_32 = big2little_32(tmp);
        for (int i = 0; i < sizeof(uint32_t); ++i)
        {
            length++;
            if (length <= 0)
            {
                continue;
            }

            char c = (str_32 << (i * 8)) >> 24;
            if (c == '\0')
            {
                goto got_length;
            }
        }
    }
got_length:

    string = (char *)kalloc(sizeof(char) * length);
    memory_set(string, 0x00, sizeof(char) * length);

    int dest_index = 0;
    index = 0;
    length = -offset_diff_from_u32;
    while (true)
    {
        uint32_t tmp = *(uint32_t *)&string_block[start + index];
        index += 4;
        uint32_t str_32 = big2little_32(tmp);
        for (int i = 0; i < sizeof(uint32_t); ++i)
        {
            length++;
            if (length <= 0)
            {
                continue;
            }
            char c = (str_32 << (i * 8)) >> 24;
            string[dest_index++] = c;
            if (c == '\0')
            {
                goto reach_to_end;
            }
        }
    }
reach_to_end:

    return string;
}

property_t *parse_property(int *index)
{
    uint32_t be_len = struct_block[(*index)++];
    uint32_t be_nameoff = struct_block[(*index)++];

    property_info_t info = {
        .len = big2little_32(be_len),
        .nameoff = big2little_32(be_nameoff),
    };

    int number_of_words = (info.len + sizeof(uint32_t) - 1) / sizeof(uint32_t);

    uint32_t *value = (uint32_t *)kalloc(sizeof(uint32_t) * number_of_words);
    if (!value)
    {
        return NULL;
    }

    memory_set(value, 0x00, sizeof(uint32_t) * number_of_words);

    for (int i = 0; i < number_of_words; ++i)
    {
        uint32_t tmp = struct_block[(*index)++];
        value[i] = big2little_32(tmp);
    }

    property_t *prop = (property_t *)kalloc(sizeof(property_t));
    if (!prop)
    {
        return NULL;
    }
    prop->next = NULL;
    prop->value = value;
    prop->name = get_string(info.nameoff);
    prop->len = number_of_words;

    return prop;
}

static void append_subnode(device_tree_t *dt, device_tree_t *subnode)
{
    if (dt->subnodes == NULL)
    {
        dt->subnodes = subnode;
        subnode->next = NULL;
        return;
    }

    device_tree_t *current = dt->subnodes;

    while (current->next)
    {
        current = current->next;
    }

    current->next = subnode;
    subnode->next = NULL;
}

static void append_property(device_tree_t *dt, property_t *prop)
{
    if (dt->properties == NULL)
    {
        dt->properties = prop;
        prop->next = NULL;
        return;
    }

    property_t *current = dt->properties;
    while (current->next)
    {
        current = current->next;
    }
    current->next = prop;
}

device_tree_t *parse_node(int *index)
{
    device_tree_t *dt = kalloc(sizeof(device_tree_t));
    if (!dt)
    {
        return NULL;
    }
    memory_set(dt, 0x00, sizeof(device_tree_t));

    char *name = get_next_name(index);
    if (!name)
    {
        return NULL;
    }
    dt->name = name;

    while (true)
    {
        uint32_t tmp = struct_block[(*index)++];
        fdt_struct_entry_t entry = big2little_32(tmp);
        switch (entry)
        {
            case FDT_BEGIN_NODE:
            {
                device_tree_t *subnode = parse_node(index);
                if (!subnode)
                {
                    return NULL;
                }
                append_subnode(dt, subnode);
                break;
            }
            case FDT_END_NODE:
            {
                goto close_node;
            }
            case FDT_PROP:
            {
                property_t *prop = parse_property(index);
                if (!prop)
                {
                    return NULL;
                }
                append_property(dt, prop);
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

device_tree_t *search_node(device_tree_t *dt, const char *node_path)
{
    if (node_path[0] != '/')
    {
        return NULL;
    }

    // e.g. "/cpus/cpu@0"
    bool is_continue = false;
    int length = 0;
    for (; node_path[length]; ++length)
    {
        char c = node_path[length + 1]; // 1 means a "/"
        if (c == '/')
        {
            is_continue = true;
            break;
        }
        // if(c == '\0')
        //{
        //    break;
        //}
    }

    device_tree_t *current = dt->subnodes;
    while (current)
    {
        bool result = string_compare(&node_path[1], current->name, length);
        if (result)
        {
            if (is_continue)
            {
                return search_node(current, &node_path[length + 1]);
            }
            else
            {
                return current;
            }
        }
        current = current->next;
    }

    return NULL;
}

property_t *get_property(const char *node_path, const char *prop_name)

{
    // e.g. node_path: "/cpus/cpu@0", prop_name: "riscv,isa"
    device_tree_t *dt = search_node(device_tree, node_path);
    if (dt == NULL)
    {
        return NULL;
    }

    property_t *current = dt->properties;
    size_t prop_len = string_length(prop_name) + 1;
    while (current)
    {
        size_t name_len = string_length(current->name) + 1;
        size_t length = prop_len > name_len ? prop_len : name_len;

        bool result = string_compare(prop_name, current->name, length);
        if (result)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

static device_tree_t* parse_fdt(const fdt_header_t* header)
{
    uint32_t magic = big2little_32(header->magic);
    if (magic != FDT_HEADER_MAGIC)
    {
        return NULL;
    }

    uint32_t offset_to_struct = big2little_32(header->off_dt_struct);
    uint32_t offset_to_string = big2little_32(header->off_dt_strings);
    uint32_t offset_to_memory = big2little_32(header->off_mem_rsvmap);

    memory_block = (fdt_reserve_entry_t *)((uint64_t)header + offset_to_memory);

    struct_block = (fdt_struct_entry_t *)((uint64_t)header + offset_to_struct);

    string_block = (char *)((uint64_t)header + offset_to_string);

    int index = 0;

    uint32_t tmp = struct_block[index++];
    fdt_struct_entry_t entry = big2little_32(tmp);

    if (entry != FDT_BEGIN_NODE)
    {
        return NULL;
    }

    return parse_node(&index);
}

uint64_t get_fdt_base(void)
{
    return fdt_base_address;
}

bool init_fdt(const uint64_t fdt_base)
{
    fdt_header_t *header = (fdt_header_t *)fdt_base;

    uint32_t magic = big2little_32(header->magic);
    if (magic != FDT_HEADER_MAGIC)
    {
        return false;
    }

    uint32_t totalsize = big2little_32(header->totalsize);

    fdt_header_t* moved_header = (fdt_header_t*)kalloc(totalsize);

    fdt_base_address = (uint64_t)moved_header;

    // move the fdt to kernel managed area.
    memory_copy(moved_header, header, totalsize);

    device_tree = parse_fdt(moved_header);
    if(device_tree == NULL)
    {
        return false;
    }

    return true;
}
