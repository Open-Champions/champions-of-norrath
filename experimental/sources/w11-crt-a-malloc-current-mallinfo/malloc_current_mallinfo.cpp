// Observed __malloc_current_mallinfo at 0x004488b8, 40 bytes, pinned SLUS_205.65: ten zero words in
// the initialised data section, between __malloc_max_total_mem (0x004488b0) and
// __malloc_lock_owner (0x004488e0). _mallinfo_r copies the whole block to its caller after
// __malloc_update_mallinfo has refreshed it. The field order is not established here, so the
// object is ten plain words. The initial value is all zero.
// The memory package keeps a separate zero view of the same 40 bytes under a different name;
// this unit supplies the original symbol name that _mallinfo_r imports.
extern "C" {
struct MallinfoBlock {
    int word[10];
};

#if defined(__mips__)
// Initialised data section, like the original object (a zero initialiser alone would go to .bss).
__attribute__((section(".data")))
#endif
MallinfoBlock __malloc_current_mallinfo = {};
}
