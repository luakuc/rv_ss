
RISCV_TOOLS	:= /home/hima/kvm_riscv/local
TOOL		:= riscv64-unknown-elf-
CC			:= $(RISCV_TOOLS)/bin/$(TOOL)gcc
LD			:= $(RISCV_TOOLS)/bin/$(TOOL)ld
GDB			:= $(RISCV_TOOLS)/bin/$(TOOL)gdb

CCFLAGS		:=-march=rv64ima -mabi=lp64 -nostdlib -ggdb3 -mcmodel=medany
LDFLAGS		:= -T script.ld

FIRMWARE	:= $(RISCV_TOOLS)/platform/qemu/virt/firmware/fw_jump.elf
RAM_SIZE	:= 128M
MACHINE		:= virt
QEMU		:= qemu/riscv64-softmmu/qemu-system-riscv64
QEMU_FLAGS	:= -monitor null -cpu rv64,x-h=true -display none -serial mon:stdio \
	-M $(MACHINE) \
	-m $(RAM_SIZE) \
	-kernel $(FIRMWARE) \
	-device loader,file=rvos,addr=0x80200000 -gdb tcp::12345 -S

OBJS		:= entry.o main.o memory_map_content.o plic.o trap.o csr_func.o trap_handler.o uart.o register.o smp.o interrupt.o timer.o thread.o string.o context_switch.o stdio.o utils.o memory_manager.o virtual_memory.o user_sample.o

IMAGE	:= rvos

all: build

.SUFFIX: .o.S
.S.o:
	$(CC) $(CCFLAGS) -c $<

.SUFFIX: .o.c
.c.o:
	$(CC) $(CCFLAGS) -c $<

build: $(IMAGE)
rvos: $(OBJS)
	$(LD) $(LDFLAGS) *.o -o rvos

run: $(IMAGE)
	$(QEMU) $(QEMU_FLAGS)

dump: $(IMAGE)
	$(RISCV_TOOLS)/bin/$(TOOL)objdump -D rvos |less

debug:
	$(GDB) -x cmd.gdb ./rvos

clean:
	rm -rf rvos *.o
