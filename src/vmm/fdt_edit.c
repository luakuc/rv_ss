#include "fdt_edit.h"
#include "endian.h"

#include "io_interface.h"

static bool check_string(uint32_t *base, size_t offset, const char *name)
{
    uint32_t base_index = offset / 4;
    uint32_t diff = offset % 4;

    size_t name_index = 0;
    while (true)
    {
        uint32_t word = base[base_index++];
        for (int i = 0; i < 4; ++i)
        {
            if (diff != 0)
            {
                diff--;
                continue;
            }

            char c = (word >> (8 * i)) & 0xff;
            if (name[name_index++] == c)
            {
                if (c == '\0')
                {
                    return true;
                }
                continue;
            }

            return false;
        }
    }

    return false;
}

static bool check_name(uint32_t *base, size_t *index, const char *name)
{
    size_t name_index = 0;
    while (true)
    {
        // uint32_t word = big2little_32(base[(*index)++]);
        uint32_t word = base[(*index)++];
        for (int i = 0; i < 4; ++i, ++name_index)
        {
            char c = (word >> (8 * i)) & 0xff;
            if (name[name_index] == c)
            {
                if (c == '\0')
                {
                    return true;
                }

                continue;
            }

            return false;
        }
    }

    return false;
}

static uint32_t *search_node(uint32_t *base, const char *name, size_t *index)
{
    while (true)
    {
        uint32_t token = big2little_32(base[(*index)++]);
        // TODO we have to parse each node and skip it.
        if (token == FDT_END)
        {
            return NULL;
        }

        if (token == FDT_BEGIN_NODE)
        {
            break;
        }
    }

    bool result = check_name(base, index, name);
    if (result)
    {
        return &base[*index];
    }

    while (true)
    {
        uint32_t token = big2little_32(base[(*index)++]);
        // TODO we have to parse each node and skip it.
        if (token == FDT_END_NODE)
        {
            break;
        }
    }

    return search_node(base, name, index);
}

static uint32_t *search_prop(uint32_t *struct_base, uint32_t *string_base,
                             const char *name)
{
    size_t index = 0;

    while (true)
    {
        size_t current_prop_index = index;
        uint32_t token = big2little_32(struct_base[index++]);
        if (token == FDT_PROP)
        {
            //uint32_t len = big2little_32(struct_base[index++]);
            size_t nameoff = (size_t)big2little_32(struct_base[index++]);

            bool result = check_string(string_base, nameoff, name);
            if (result)
            {
                return &struct_base[current_prop_index];
            }
        }
        if (token == FDT_END)
        {
            break;
        }
    }

    return NULL;
}

bool conceal_h_extension(fdt_header_t *header, size_t hart_id)
{
    uint32_t struct_offset = big2little_32(header->off_dt_struct);
    uint32_t string_offset = big2little_32(header->off_dt_strings);

    uint32_t *struct_base = (uint32_t *)((uintptr_t)header + struct_offset);
    uint32_t *string_base = (uint32_t *)((uintptr_t)header + string_offset);

    size_t index = 0;
    uint32_t *root_node = search_node(struct_base, "", &index);
    if (root_node == NULL)
    {
        return false;
    }

    index = 2;
    uint32_t *cpus_node = search_node(root_node, "cpus", &index);
    if (cpus_node == NULL)
    {
        return false;
    }

    index = 2;
    uint32_t *cpu_node = search_node(cpus_node, "cpu@0", &index);
    if (cpu_node == NULL)
    {
        return false;
    }

    uint32_t *prop = search_prop(cpu_node + 2, string_base, "riscv,isa");
    if (prop == NULL)
    {
        return false;
    }

    //const int token_index = 0;
    const int len_index = 1;
    //const int nameoff_index = 2;
    const int value_index = 3;

    uint32_t len = big2little_32(prop[len_index]);
    put_string((const char*)&prop[value_index]);
    for(int i=0; i< len; ++i)
    {
        char* c_ptr = (char *)((uintptr_t)&prop[value_index] + i);
        char c = *c_ptr;
        if(c == 'h')
        {
            for(int j = 0; j < len - i; ++j)
            {
                *(c_ptr + j) = *(c_ptr + j + 1);
            }
        }
    }
    put_string("\n");
    put_string((const char*)&prop[value_index]);
    put_string("\n");

    return true;
}
