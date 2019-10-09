OUTPUT_ARCH("riscv")

ENTRY(_start)

SECTIONS
{
    /*
    . = 0x80000000;
    _ram_base = .;
    */
    . = 0x80200000;
    _start = .;

    .text :
    {
        *(.text.start);
        *(.text);
    }

    .data :
    {
        *(.data);
    }

    .rodata :
    {
        *(.rodata);
    }

    .bss :
    {
        *(.bss);
    }

    . = ALIGN(0x1000);
    . = . + 0x8000;
    _kernel_stack = .;

    _heap_start = .;

    _heap_end = 0x80000000 + 0x800000; /* 8MiB */
    _ram_end = .;
}
