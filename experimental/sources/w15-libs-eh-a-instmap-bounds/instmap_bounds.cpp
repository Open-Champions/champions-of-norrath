// Observed instmap_bounds at 0x00434050: 24 file-backed zero bytes in .data, a global object just after the 0x2000-byte
// instmap (0x00432050) and before mt_tiledata (0x00434068). The megatile routine megatileEntitize__Fpci reads its
// words 0 and 1 as signed ints (each multiplied by 6 and added to the position of the first and second axis).
// Unknown: what else reads or writes the other four words, and whether the image's initial zero is ever the value
// the callers see (the loader may fill it). The six-int shape is only the 24-byte extent.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
int instmap_bounds[6] NORRATH_LOADED(".data") = {0, 0, 0, 0, 0, 0};
}
