#include "virtio_mmio.h"

// virtio mmio register offsets
// There are details in
// http://docs.oasis-open.org/virtio/virtio/v1.0/virtio-v1.0.html
#define MAGIC_VALUE (0x0000)
#define VERSION (0x0004)
#define DEVICE_ID (0x0008)
#define VENDOR_ID (0x000c)
#define DEVICE_FEATURES (0x0010)
#define DEVICE_FEATURES_SEL (0x0014)
#define DRIVER_FEATURES (0x0020)
#define DRIVER_FEATURES_SEL(0x0024)
#define QUEUE_SEL (0x0030)
#define QUEUE_NUM_MAX (0x34)
#define QUEUE_NUM (0x38)
#define QUEUE_READY (0x44)
#define QUEUE_NOTIFY (0x50)
#define INTERRUPT_STATUS (0x60)
#define INTERRUPT_ACK (0x64)
#define STATUS (0x70)
#define QUEUE_DESC_LOW (0x80)
#define QUEUE_DESC_HIGH (0x84)
// etc

enum device_type
{
    DEVICE_ID_RESERVED = 0,
    DEVICE_ID_NETWORK_CARD = 1,
    DEVICE_ID_BLOCK_DEVICE = 2,
    DEVICE_ID_CONSOLE = 3,
    //etc
};

#define MAGIC_VALUE 0x74726976

static bool init_virtio_block(const uintptr_t base)
{
    uint32_t status = *(uint32_t*)(base + STATUS);
    //TODO
}

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
        {
            bool result = init_virtio_block(base);
            if(!result)
            {
                return false;
            }
            break;
        }
        default:
            return false;
            break;
    }

    return true;
}
