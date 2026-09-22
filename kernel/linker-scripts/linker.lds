OUTPUT_FORMAT(elf64-x86-64)

ENTRY(kmain)

PHDRS{
    limine_requests PT_LOAD;
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
}

SECTIONS{
    . = 0xffffffff80000000;

    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .text : {
        text_start = .;
        *(.text .text.*)
        text_end = .;
    }:text

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .rodata : {
        rodata_start = .;
        *(.rodata .rodata.*)
    }:rodata

    .note.gnu.build-id : {
        *(.note.gnu.build-id)
        rodata_end = .;
    } :rodata

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .data : {
        data_start = .;
        *(.data .data.*)
    } :data

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
        data_end = .;
    } :data

    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}
