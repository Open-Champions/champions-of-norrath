// Observed, from the pinned SLUS_205.65 section table and symbol table: the image has an
// .eh_frame section at 0x00453970 (0x974 bytes) and the symbol __EH_FRAME_BEGIN__ (size 0) sits
// at exactly 0x00453970, together with a local __FRAME_BEGIN__ of the same address; two more
// local __FRAME_BEGIN__ symbols follow at 0x00453d8c and 0x00453f0c, the starts of the later
// objects' contributions. The startup constructor and destructor code passes
// __EH_FRAME_BEGIN__ to __register_frame_info / __deregister_frame_info.
// Authored: a zero-length array that starts this object's .eh_frame contribution, which is the
// usual start-of-frame-table marker. It carries no data and no unwind records.
// Inferred: in a final link the marker must come from the first .eh_frame contribution (the
// link order of the original start-up object); in the partial relocatable link the position is
// only this object's offset in the merged section. The marker's address is used as an opaque
// pointer, so only its presence matters to the code reviewed so far.
// Unknown: whether the original start-up object also contributed a terminator word (the
// image's last bytes of .eh_frame were not examined for that here) and the final link order.
#ifndef NORRATH_EH_FRAME_SECTION
#define NORRATH_EH_FRAME_SECTION __attribute__((section(".eh_frame"), aligned(8), used))
#endif
extern "C" {
char __EH_FRAME_BEGIN__[0] NORRATH_EH_FRAME_SECTION;
}
