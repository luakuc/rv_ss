
RISCV_TOOLS	:= local
TOOL		:= riscv64-unknown-elf-
CC			:= $(RISCV_TOOLS)/bin/$(TOOL)gcc
LD			:= $(RISCV_TOOLS)/bin/$(TOOL)ld
GDB			:= $(RISCV_TOOLS)/bin/$(TOOL)gdb

CCFLAGS		:=-march=rv64ima -mabi=lp64 -nostdlib -ggdb3 -mcmodel=medany -Wall -Iinclude/
LDFLAGS		:= -T script.ld

IMAGE	:= rvss

DISK_IMG	:= disk.img

FIRMWARE	:= $(RISCV_TOOLS)/platform/qemu/virt/firmware/fw_jump.elf
RAM_SIZE	:= 128M
MACHINE		:= virt
QEMU		:= qemu/riscv64-softmmu/qemu-system-riscv64
QEMU_FLAGS	:= -monitor null -cpu rv64,x-h=true -display none -serial mon:stdio \
	-M $(MACHINE) \
	-m $(RAM_SIZE) \
	-kernel $(FIRMWARE) \
	-device loader,file=$(IMAGE),addr=0x80200000 -gdb tcp::12345 -S
QEMU_FLAGS 	+= -drive file=$(DISK_IMG),if=none,format=raw,id=file_0 \
			   -device virtio-blk-device,drive=file_0,bus=virtio-mmio-bus.0

VMM_OBJS	:= vmm/vcpu.o vmm/vmm.o vmm/enter.o vmm/mmu.o vmm/hfence.o vmm/test_guest.o vmm/trap.o
OBJS		:= entry.o main.o memory_map_content.o plic.o trap.o csr_func.o trap_handler.o uart.o register.o smp.o interrupt.o timer.o thread.o string.o context_switch.o io_interface.o utils.o memory_manager.o virtual_memory.o user_sample.o system_call.o virtio_mmio.o exception.o endian.o $(VMM_OBJS) fdt.o


all: build

.SUFFIX: .o.S
.S.o:
	$(CC) $(CCFLAGS) -c $< -o $@

.SUFFIX: .o.c
.c.o:
	$(CC) $(CCFLAGS) -c $< -o $@

build: $(IMAGE) $(DISK_IMG)
$(IMAGE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

run: $(IMAGE)
	$(QEMU) $(QEMU_FLAGS)

dump: $(IMAGE)
	$(RISCV_TOOLS)/bin/$(TOOL)objdump -D $< |less

debug: $(IMAGE)
	$(GDB) -x cmd.gdb $<

$(DISK_IMG):
	dd if=/dev/zero of=$(DISK_IMG) count=1024

clean:
	rm -rf $(IMAGE) *.o $(VMM_OBJS)
