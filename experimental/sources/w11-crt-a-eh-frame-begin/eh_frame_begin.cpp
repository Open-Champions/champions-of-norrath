// Observed __EH_FRAME_BEGIN__ at 0x00453970, pinned SLUS_205.65: a global symbol with no recorded
// size that labels the first byte of the .eh_frame section (0x00453970..0x004542e4); a local
// __FRAME_BEGIN__ shares the address (the first object's own start). The start-up code passes it to
// __register_frame_info and __deregister_frame_info (runtime-startup/constructors.cpp and
// destructors.cpp). The object is therefore a zero-length marker in the .eh_frame section.
// What this unit does not do: it does not place itself first. In the original link the object that
// carries this marker is the first .eh_frame contributor; the final link has to put this unit's
// .eh_frame input first for the address to be the start of the section.
// Unknown: which original object carries the marker (a C runtime start file is the usual owner).
extern "C" {
#if defined(__mips__)
char __EH_FRAME_BEGIN__[] __attribute__((section(".eh_frame"), aligned(4))) = {};
#else
char __EH_FRAME_BEGIN__[] __attribute__((aligned(4))) = {};
#endif
}
