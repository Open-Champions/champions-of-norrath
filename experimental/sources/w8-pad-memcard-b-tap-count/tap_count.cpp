// Observed: tapCount is two bytes at 0x00484060 in the image, both 1. padProcess
// stores scePadGetSlotMax results there; padClear and padGetUnprocessedData
// read them as signed bytes. Byte 0 is the slot count on port 0, byte 1 on
// port 1.
// Inferred: the two bytes are per-port multitap slot counts.
// Unknown: nothing beyond the extent, which is the symbol size 2.
extern "C" signed char tapCount[2] = {1, 1};
