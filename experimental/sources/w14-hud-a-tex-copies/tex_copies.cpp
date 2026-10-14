// Observed: blackoutTexCopy (0x008dd340) and foregrndTexCopy (0x008dd380) are bss
// objects of 64 and 128 bytes: the symbols around them are exactly that far
// apart, and the authored loading-screen code copies one 64-byte texture record
// into the first and two into the second. Inferred: a 64-byte record, once and
// twice. The 64 bytes are kept as opaque storage here; the record layout is
// owned by the loading-screen code that fills them.
extern "C" unsigned char blackoutTexCopy[64] __attribute__((aligned(16))) = {};
extern "C" unsigned char foregrndTexCopy[128] __attribute__((aligned(16))) = {};
static_assert(sizeof(blackoutTexCopy) == 64 && sizeof(foregrndTexCopy) == 128, "tex copies");
