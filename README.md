This is a experiment project for RISC-V(RV64) system programming.

##### target
- QEMU RISC-V virt
- K210 (on MAiXDUiNO)
    - WIP

##### features
- MMU driver
    - host virtual address into host physical address
- PLIC (RISC-V interrupt controller) driver
- UART driver
- VMM(WIP)
    - interrupt and exception delegation
    - MMU(guest physical address into host physical address)
- Timer controller
- SBI driver
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
$ #wip 
```
WIP
