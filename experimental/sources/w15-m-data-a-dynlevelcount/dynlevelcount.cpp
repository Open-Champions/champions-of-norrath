// Observed: dynlevelcount is the 4-byte global word at 0x00485e24 in the reference
// image (small-data section, symbol type D). Its four image bytes are zero.
// Inferred: a signed int, because the megatile entitize unit declares it as int and
// resets it to zero at the start of each run. Unknown: the original unit that owns it.
extern "C" { int dynlevelcount = 0; }
static_assert(sizeof(dynlevelcount) == 4, "dynlevelcount");
