// Observed: blackoutTexCopy is a local .bss symbol at 0x008dd340 with ELF size 0; the next symbol, foregrndTexCopy, starts 64 bytes
// later. loadThreadMain__FPv copies a 64-byte block (eight doubleword-sized unaligned transfers, offsets 0 through 63) into this
// address, and texNormalizedWidth/Height read halfwords at +0 and +2 of a block of this kind.
// Inferred: the object is a 64-byte copy of a texture record (the _texture type of those two functions); the name says it is the
// black-out texture's copy. No claim is made about the record layout beyond the two halfwords.
// Unknown: the record layout, who else reads or writes this copy, and the real symbol linkage (the original is a local symbol; this
// definition is global). Nothing here ran on the EE.
extern "C" {
unsigned char blackoutTexCopy[64];
}
