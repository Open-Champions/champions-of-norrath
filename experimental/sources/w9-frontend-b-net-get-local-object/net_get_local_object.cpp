// Observed: netGetLocalObject__FP8Creature at 0x001353d8 is 96 bytes and calls
// nothing. It returns its argument unchanged unless all of these hold: bit 0x40
// of the word at offset 12 of the object is set, the signed byte netNetworkMode
// is nonzero, and localPredictedPlayer is nonzero. In that case it returns
// localPredictedPlayer. When the word at offset 960 of the object (index) is
// below nmPartners, the byte at partners + 17620 + 17632 * index must also be
// nonzero (a zero byte returns the argument); an index at or above nmPartners
// skips that test and counts as nonzero. The index is compared as a signed int.
// Inferred: the object word at 960 is a player index and the 17632-byte stride
// is the partner record size, from other callers that scale by the same value.
// Unknown: the type of localPredictedPlayer beyond being a pointer-sized value,
// and the meaning of bit 0x40 in the object flags.

extern "C" signed char netNetworkMode;
extern "C" int nmPartners;
extern "C" unsigned char partners[];
extern "C" void* localPredictedPlayer;

extern "C" void* netGetLocalObject__FP8Creature(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if ((*reinterpret_cast<unsigned*>(bytes + 12) & 0x40) == 0) return object;
    if (netNetworkMode == 0) return object;
    int index = *reinterpret_cast<int*>(bytes + 960);
    bool present = true;
    if (index < nmPartners) present = partners[17620 + 17632 * index] != 0;
    if (present && localPredictedPlayer != 0) return localPredictedPlayer;
    return object;
}
