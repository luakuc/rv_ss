#include "virtio_mmio.h"
#include "io_interface.h"
#include "memory_manager.h"
#include "string.h"
#include "fdt.h"
#include "endian.h"

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
#define QUEUE_AVAIL_LOW (0x90)
#define QUEUE_AVAIL_HIGH (0x94)
#define QUEUE_USED_LOW (0xa0)
#define QUEUE_USED_HIGH (0xa4)
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

#define VIRTIO_QUEUE_SIZE 3

typedef struct virtq_used
{
    uint16_t flags;
    uint16_t index;
    struct virtq_used_elem
    {
        uint32_t id;
        uint32_t len;
    } ring[VIRTIO_QUEUE_SIZE];
    uint16_t avail_event;
} virtq_used_t;

typedef struct virtqueue
{
    struct virtq_desc
    {
        uint64_t address; // physical address
        uint32_t len;
        uint16_t flags;
        uint16_t next;
    } desc[VIRTIO_QUEUE_SIZE];

    struct virtq_avail
    {
        uint16_t flags;
        uint16_t index;
        uint16_t ring[VIRTIO_QUEUE_SIZE];
        uint16_t used_event;
    } avail;
} virtqueue_pre_t;

typedef struct virtio_block
{
    virtqueue_pre_t *desc_avail;
    virtq_used_t *used;
    uint8_t request_status[VIRTIO_QUEUE_SIZE];
    uint32_t* mmio_base;
} virtio_block_t;

static virtio_block_t virtio_block;

#define VIRTIO_BLK_T_IN (0)
#define VIRTIO_BLK_T_OUT (1)
#define VIRTIO_BLK_T_FLUSH (4)

typedef struct virtio_blk_req
{
    uint32_t type;
    uint32_t reserved;
    uint64_t sector;
} virtio_blk_req_t;

#define VRING_DESC_F_NEXT 1
#define VRING_DESC_F_WRITE 2

void virtio_block_request(bool write, uint64_t* base_ptr)
{
    struct virtq_desc *desc0, *desc1, *desc2;
    desc0 = &virtio_block.desc_avail->desc[0];
    desc1 = &virtio_block.desc_avail->desc[1];
    desc2 = &virtio_block.desc_avail->desc[2];

    virtio_blk_req_t request = {
        .type = write ? VIRTIO_BLK_T_OUT : VIRTIO_BLK_T_IN,
        .reserved = 0,
        .sector = 0, //TODO
    };

    desc0->address = (uintptr_t)&request;
    desc0->len = sizeof(request);
    desc0->flags = VRING_DESC_F_NEXT;
    desc0->next = 1;

    desc1->address = (uintptr_t)base_ptr;
    desc1->len = 1;
    if(write)
    {
        desc1->flags = 0;
    }
    else
    {
        desc1->flags = VRING_DESC_F_WRITE;
    }

    desc1->flags |= VRING_DESC_F_NEXT;
    desc1->next = 2;

    desc2->address = (uintptr_t)&virtio_block.request_status[0];
    desc2->len = 1;
    desc2->flags = VRING_DESC_F_WRITE;
    desc2->next = 0;

    *(virtio_block.mmio_base + QUEUE_NOTIFY) = 0;
    //TODO
    //sleep this thread
}

static bool init_virtio_block(const uintptr_t base)
{
    virtio_block.mmio_base = base;
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

    char features_str[10] = {0};
    memory_set(features_str, 0x00, 10);
    int_to_str(*device_features, features_str);
    put_string("virtio,block: device features 0x");
    put_string(features_str);
    put_string("\n");
    // TODO write the features bit
    *device_features &= ~(1 << VIRTIO_BLK_F_RO);
    *device_features &= ~(1 << VIRTIO_BLK_F_SCSI);
    *device_features &= ~(1 << VIRTIO_BLK_F_CONFIG_WCE);

    // 5. set the features_ok status bit
    *status |= STATUS_FEATURES_OK;

    // 6. read features_ok bit
    if (!(*status & STATUS_FEATURES_OK))
    {
        // setup failed
        put_string("features ok status bit is zero\n");
        return false;
    }

    // 7. device specific setup

    // 8. setup driver_ok status bit
    *status |= STATUS_DRIVER_OK;

    // 4.2.3.2 Virtqueue Configuration
    // 1. select the queue writing its indext to QueueSel
    uint32_t *queue_sel = (uint32_t *)(base + QUEUE_SEL);
    *queue_sel = 0;

    // 2. check the QueueReady
    uint32_t *queue_ready = (uint32_t *)(base + QUEUE_READY);
    if ((*queue_ready) != 0)
    {
        put_string("the queue 0 is already used\n");
        return false;
    }

    // 3. read maximum queue size
    uint32_t *max_queue_size = (uint32_t *)(base + QUEUE_NUM_MAX);
    if ((*max_queue_size) == 0)
    {
        put_string("maximum queue size is zero\n");
        return false;
    }

// 4. allocate queue pages
#define PAGE_SIZE 0x1000
    uint32_t *pages = (uint32_t *)knalloc_4k(2);
    memory_set(pages, 0x00, PAGE_SIZE * 2);

    virtio_block.desc_avail = (virtqueue_pre_t *)pages;
    virtio_block.used = (virtq_used_t *)pages + PAGE_SIZE;

    // 5. notify the device about the queue size
    uint32_t *queue_num = (uint32_t *)(base + QUEUE_NUM);
    *queue_num = 1;

    // 6. write physical addresses of the queue's descriptor table.
    // - QueueDescLow | QueueDescHigh
    // - QueueAvailLow | QueueAvailHigh
    // - QueueUsedLow | QueueUsedHigh
    uint32_t *queues_desc_low = (uint32_t *)(base + QUEUE_DESC_LOW);
    uint32_t *queues_desc_high = (uint32_t *)(base + QUEUE_DESC_LOW);
    *queues_desc_low = (uintptr_t)virtio_block.desc_avail;
    *queues_desc_high = (uintptr_t)virtio_block.desc_avail >> 32;

    uint32_t *queues_avail_low = (uint32_t *)(base + QUEUE_AVAIL_LOW);
    uint32_t *queues_avail_high = (uint32_t *)(base + QUEUE_AVAIL_HIGH);
    *queues_avail_low = (uintptr_t)&virtio_block.desc_avail->avail;
    *queues_avail_high = (uintptr_t)&virtio_block.desc_avail->avail >> 32;

    uint32_t *queue_used_low = (uint32_t*)(base + QUEUE_USED_LOW);
    uint32_t *queue_used_high = (uint32_t*)(base + QUEUE_USED_HIGH);
    *queue_used_low = (uintptr_t)virtio_block.used;
    *queue_used_high = (uintptr_t)virtio_block.used >> 32;

    // 7. write 0x01 to QueueReady
    *queue_ready = 1;

    return true;
}

static bool get_base_register(uint64_t* base)
{
    const char *name = "reg";
    property_t *prop = get_property("/virtio_mmio@", name);
    if(!prop)
    {
        return false;
    }

    // 0x0, base address, 0x0, size
    if(prop->len != 4)
    {
        return false;
    }

    //uint64_t size;
    *base = (uint64_t)big2little_32(prop->value[0]) << 32 | big2little_32(prop->value[1]);
    //size = (uint64_t)big2little_32(prop->value[2]) << 32 | big2little_32(prop->value[3]);

    return true;
}

bool init_virtio_mmio(void)
{
    //TODO
    return true;

    //TODO get_base_register has bug.
    uint64_t base;
    bool result = get_base_register(&base);
    if(!result)
    {
        return false;
    }

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
