OUTPUT_ARCH("riscv")

ENTRY(_start)

SECTIONS
{
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
    kernel_stack = . + 0x1000;

    _end = .;
}
