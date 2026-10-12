// Observed .sbss symbol runList at 0x486878. The ELF size field is 0.
// Inferred: deleteList begins eight bytes later, and that gap is this object.
// listOnewayLink reads and writes only the first word of the head it is given.
// The second word has no separate symbol. Unknown: which callers use it.
// The reviewed -G0 profile places these zeros in .bss rather than .sbss.
extern "C" {
unsigned char norrath_actor_view_486878[8];
}
