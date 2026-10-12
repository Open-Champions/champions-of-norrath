// Observed: targetObjs starts at 0x008d5d40 in zero-filled storage; the next symbol, dummyList, starts 48 bytes
// later and lightMats precedes it. changeSkin__Fii, init__Fv and npcLookAtCamera__Fv take its address.
// Inferred: ten pointers, one per record of targets, because the existing authored init stores one object
// pointer for each of the ten targets. The extra 8 bytes before dummyList are not accounted for and are not
// reproduced here (padding is a guess).
// Unknown: the real extent of the array.

extern "C" {
unsigned char* targetObjs[10];
}
#if defined(__mips__)
static_assert(sizeof(targetObjs) == 40, "targetObjs");
#endif
