// Observed: MC_GetTempPlayerStorage__Fi at 0x0025b3b8 is 24 bytes. It returns the address
// tempPlayerStruct (0x007e05e0) plus the argument times 23984, with no bounds check.
// Inferred: 23984 is the size of one player record, since the same stride separates the four
// slots walked by MC_GetRamPlayerPtr__Fi.
// Unknown: how many records tempPlayerStruct holds.

extern "C" unsigned char tempPlayerStruct[];

extern "C" unsigned char* MC_GetTempPlayerStorage__Fi(int index) {
    return tempPlayerStruct + index * 23984;
}
