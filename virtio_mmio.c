#include "virtio_mmio.h"
#include "io_interface.h"
#include "string.h"

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
#define DRIVER_FEATURES_SEL (0x0024)
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
    // etc
};

#define QEMU_MAGIC_VALUE 0x74726976

#define STATUS_RESET 0x0
#define STATUS_ACK 0x1
#define STATUS_DRIVER 0x2
#define STATUS_DRIVER_OK 0x4
#define STATUS_FEATURES_OK 0x8

// features bit
#define VIRTIO_BLK_F_BARRIER 0 // legacy interface
#define VIRTIO_BLK_F_SIZE_MAX 1
#define VIRTIO_BLK_F_SEG_MAX 2
#define VIRTIO_BLK_F_GEOMETRY 4
#define VIRTIO_BLK_F_RO 5
#define VIRTIO_BLK_F_BLK_SIZE 6
#define VIRTIO_BLK_F_SCSI 7 // legacy interface
#define VIRTIO_BLK_F_FLUSH 9
#define VIRTIO_BLK_F_TOPOLOGY 10
#define VIRTIO_BLK_F_CONFIG_WCE 11

static bool init_virtio_block(const uintptr_t base)
{
    // 4.1.1 Device Initialization & 3.1.1
    // 1. reset the device
    uint32_t *status = (uint32_t *)(base + STATUS);
    *status = STATUS_RESET;

    // 2. set the ack status bit
    *status |= STATUS_ACK;

    // 3. set the driver status bit
    *status |= STATUS_DRIVER;

    // 4. read device feature bits and write the subset of the feature bits
    uint32_t *device_features = (uint32_t *)(base + DEVICE_FEATURES);

    char features_str[10];
    int_to_str(*device_features, features_str);
    put_string("virtio,block: device features 0x");
    put_string(features_str);
    put_string("\n");
    //TODO write the features bit

    // 5. set the features_ok status bit
    *status |= STATUS_FEATURES_OK;

    // 6. read features_ok bit
    if(!(*status & STATUS_FEATURES_OK))
    {
        // setup failed
        put_string("features ok status bit is zero");
        return false;
    }

    // 7. device specific setup

    // 8. setup driver_ok status bit
    *status |= STATUS_DRIVER_OK;

    // 4.2.3.2 Virtqueue Configuration
    // 1. select the queue writing its indext to QueueSel
    uint32_t *queue_sel = (uint32_t*)(base + QUEUE_SEL);

    return false;
}

bool init_virtio_mmio(const struct memory_map_entry *memory_map_entry)
{
    uintptr_t base = memory_map_entry->base;

    uint32_t magic_value = *(uint32_t *)(base + MAGIC_VALUE);

    if (magic_value != QEMU_MAGIC_VALUE)
    {
        return false;
    }

    uint32_t version = *(uint32_t *)(base + VERSION);

    if (version == 2)
    {
        put_string("this driver is not non legacy virtio devices.\n");
        return false;
    }

    if (version != 1)
    {
        put_string("unknown version");
        return false;
    }

    uint32_t device_id = *(uint32_t *)(base + DEVICE_ID);

    // uint32_t vendor_id = *(uint32_t *)(base + VENDOR_ID);

    switch (device_id)
    {
        case DEVICE_ID_BLOCK_DEVICE:
        {
            bool result = init_virtio_block(base);
            if (!result)
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
