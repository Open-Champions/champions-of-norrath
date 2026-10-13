// Observed word at 0x004861e8 in the pinned image: four bytes of uninitialised data (.sbss) with no
// symbol of its own, here published as `norrath_world_c_view_4861e8` (the name the
// patchSortedListWithLocalTime unit already imports; the name is address-based on purpose).
// Per the w10-world-c notes it is the loop count for that routine, written at the end of
// generateSortedListAndInfoHeaders__Fv (0x002562a8) and read by several memory-card routines
// between 0x00251600 and 0x0025ba00; it is not sortedDirCount (0x00485be8).
// This unit only defines the storage, zero at start, as a four-byte int. It does not decide who
// writes it. Unknown: the true name and whether the word is a count of directory entries or of
// something derived from them.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
int norrath_world_c_view_4861e8 NORRATH_LOADED(".bss") = 0;
}
