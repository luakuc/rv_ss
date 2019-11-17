This is a experiment project for RISC-V(RV64) system programming.

##### target
- QEMU RISC-V virt
    - WIP
- K210 (on MAiXDUiNO)
    - in the future.

##### features
- FDT parser
- MMU driver
    - host virtual address into host physical address
- PLIC (RISC-V interrupt controller) driver
- UART driver
- VMM(WIP)
    - interrupt and exception delegation
    - MMU(guest physical address into host physical address)
- Timer controller
- SBI driver
    - for version 0.1
- virtio(WIP)
    - virtio-block(WIP)
    - virtio-net(WIP)

##### how to build
1. clone
```
$ git clone --recursive https://github.com/sux2mfgj/rv_ss
```

1. build toolchains(gcc, libc, gdb, ld, biutils, etc)
```
$ cd riscv-gnu-toolchain
$ ./configure --prefix=/home/hima/work/riscvos/../local
```

1. build opensbi
```
$ export CROSS_COMPILE=riscv64-unknown-elf-
$ make PLATFORM=qemu/virt I=../local install
```

1. build qemu
```
$ git clone git@github.com:kvm-riscv/qemu.git
$ cd qemu
$ git checkout mainline/alistair/riscv-hyp-ext-v0.4.1
```

1. build and run rv_ss
```
$ cd ${to project root}
$ make run
```

##### TODO

* Update the qemu. There is a new version of the qemu for risc-v hypervisor extension.
(mainline/alistair/riscv-hyp-ext-v0.4.2)
* add the qemu as a submodule.
