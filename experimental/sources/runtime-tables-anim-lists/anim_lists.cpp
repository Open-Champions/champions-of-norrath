// Observed fsList at 0x48690c, arList at 0x486914, and arListBack at
// 0x486918: one nobits word each in .sbss (fsListBack sits between fsList
// and arList but is not in this package). The animation initializer stores
// the frame-state pool in fsList, the result pool in arList, and the last
// result record in arListBack; all three start cleared.
// Inferred: these reconstruction names are those three pointer words,
// zero-initialized like the nobits image.
// Unknown: later stores by code outside the initializer, and placement at
// the original addresses.
extern "C" {
unsigned char* fsList = nullptr;
unsigned char* arList = nullptr;
unsigned char* arListBack = nullptr;
}
