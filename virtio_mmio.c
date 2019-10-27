#include "virtio_mmio.h"

// virtio mmio register offsets
// There are details in
// http://docs.oasis-open.org/virtio/virtio/v1.0/virtio-v1.0.html
#define MAGIC_VALUE (0x0000)
#define VERSION (0x0004)
#define DEVICE_ID (0x0008)
#define VENDOR_ID (0x000c)
#define HOST_FEATURES (0x0010)
#define HOST_FEATURES_SEL (0x0014)
#define GUEST_FEATURES (0x0020)
#define GUEST_FEATURES_SEL (0x0024)
#define GUEST_PAGE_SIZE (0x0028)
//#define VIRTIO_MMIO_REGS_
//#define 
//#define 

enum device_type
{
    DEVICE_ID_RESERVED = 0,
    DEVICE_ID_NETWORK_CARD = 1,
    DEVICE_ID_BLOCK_DEVICE = 2,
    DEVICE_ID_CONSOLE = 3,
    //etc
};

#define MAGIC_VALUE 0x74726976

bool init_virtio_mmio(const struct memory_map_entry *memory_map_entry)
{
    uintptr_t base = memory_map_entry->base;

    uint32_t magic_value =
        *(uint32_t *)(base + MAGIC_VALUE);

    if(magic_value != MAGIC_VALUE)
    {
        return false;
    }

    uint32_t version =
        *(uint32_t *)(base + VERSION);
    if(version != 2)
    {
        return false;
    }

    uint32_t device_id =
        *(uint32_t *)(base + DEVICE_ID);

    uint32_t vendor_id =
        *(uint32_t *)(base + VENDOR_ID);

    switch(device_id)
    {
        case DEVICE_ID_BLOCK_DEVICE:
            //TODO
            break;
        default:
            return false;
            break;
    }

    return true;
}
