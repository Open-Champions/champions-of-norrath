// Observed updatePlayersReady__9Savepoint at 0x00215570, 392 bytes: one argument (the save point
// object), no return value. Byte offsets below are into that object.
//   1. Bytes 200, 199 and 201 are cleared.
//   2. For each i below nmPlayersAlive, index = playersAlive[i] selects a 23984-byte record of
//      player. The word at record + 21872 is a pointer p. If p is nonzero and the word at p + 12 has
//      bit 0x400000 set, a flag is raised (never lowered). The three floats at p + 16 are compared
//      with the three floats at object + 16 whether or not p is zero (for a zero p the original
//      reads address 16 without a test). d = (dx*dx + dy*dy) + dz*dz is truncated to an int.
//        d < 14400: netPlayerIsLocal__Fi(index); a nonzero result sets byte 201 to 1. Byte 200 is
//                   incremented and the near count rises. nmPlayersAlive is read again afterwards.
//        d < 22500: (any d, near ones included) the wide count rises.
//   3. If the signed byte 198 is zero and the wide count is zero, byte 198 becomes 1.
//   4. Byte 199 becomes the low 8 bits of the near count, or 0xff when that is nonzero and the flag
//      is set.
// Inferred: 14400 and 22500 are the squares of 120 and 150, so the counts are players within those
// distances of the save point.
// Unknown: what bit 0x400000 at p + 12 means and what p points to.
extern "C" {
extern int nmPlayersAlive;
extern int playersAlive[4];
extern unsigned char player[];
int netPlayerIsLocal__Fi(int playerIndex);
}

#ifdef W12_HUD_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
inline unsigned readWord(__UINTPTR_TYPE__ address) {
    unsigned v;
    __builtin_memcpy(&v, reinterpret_cast<const void*>(address), 4);
    return v;
}
inline __UINTPTR_TYPE__ wordAddress(unsigned word) {
#ifdef W12_HUD_HOST
    return hostWordAddress(word);
#else
    return word;
#endif
}
}

extern "C" void updatePlayersReady__9Savepoint(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    object[200] = 0;
    object[199] = 0;
    object[201] = 0;
    int nearCount = 0;
    int wideCount = 0;
    bool flagged = false;
    const float* mine = reinterpret_cast<const float*>(object + 16);
    for (int i = 0; i < nmPlayersAlive; ++i) {
        const int index = playersAlive[i];
        const unsigned pointerWord = readWord(reinterpret_cast<__UINTPTR_TYPE__>(player) + index * 23984 + 21872);
        const __UINTPTR_TYPE__ p = wordAddress(pointerWord);
        if (pointerWord != 0 && (readWord(p + 12) & 0x400000u) != 0) flagged = true;
        const float* theirs = reinterpret_cast<const float*>(p + 16);
        const float dx = theirs[0] - mine[0];
        const float dy = theirs[1] - mine[1];
        const float dz = theirs[2] - mine[2];
        const int d = static_cast<int>(dx * dx + dy * dy + dz * dz);
        if (d < 14400) {
            if (netPlayerIsLocal__Fi(index)) object[201] = 1;
            ++object[200];
            ++nearCount;
        }
        if (d < 22500) ++wideCount;
    }
    if (static_cast<signed char>(object[198]) == 0 && wideCount == 0) object[198] = 1;
    object[199] = static_cast<unsigned char>(nearCount);
    if ((nearCount & 0xff) != 0 && flagged) object[199] = 0xff;
}
