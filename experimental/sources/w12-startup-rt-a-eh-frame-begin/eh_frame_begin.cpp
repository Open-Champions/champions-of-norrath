// Observed __EH_FRAME_BEGIN__ at 0x00453970, pinned SLUS_205.65: a global data symbol with no size that
// labels the first byte of the .eh_frame section (0x00453970..0x004542e4). The start-up code passes it to
// __register_frame_info and __deregister_frame_info.
// DEVIATION FROM THE ORIGINAL SECTION: the marker is a zero-length object in its own input section,
// .norrath_eh_frame_begin, not in .eh_frame. The earlier unit (w11-crt-a-eh-frame-begin) put it in
// .eh_frame, and the relocatable link of the whole selection then dies with a BFD assertion
// (elflink.c:9839) when the start-up code's relocations refer to a symbol inside an empty .eh_frame
// input; a differently named section links (checked with the reviewed ld -r against the full partial
// object). A final link has to place this input section first inside the output .eh_frame, for example
//     .eh_frame : { KEEP(*(.norrath_eh_frame_begin)) KEEP(*(.eh_frame)) }
// for the symbol to equal the start of that section as in the original.
// Unknown: which original object carries the marker (a C runtime start file is the usual owner).
extern "C" {
char __EH_FRAME_BEGIN__[] __attribute__((section(".norrath_eh_frame_begin"), aligned(4))) = {};
}
