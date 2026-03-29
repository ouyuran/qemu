ENTRY(_reset)

SECTIONS
{
    . = 0x80000000;

    .text : {
        *(.text*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
    }

    . = ALIGN(8);
    . = . + 0x1000;
    _stack_top = .;
}