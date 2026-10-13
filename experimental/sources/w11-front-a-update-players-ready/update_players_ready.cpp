// Observed: updatePlayersReady__9Savepoint at 0x00215570 is 392 bytes, argument self (the save
// point object), no return value.
//  1. Bytes 200, 199 and 201 of self are cleared, in that order.
//  2. For each i below nmPlayersAlive, idx = playersAlive[i] (a 4-word table) selects record idx
//     of player (23984 bytes each). The word at record + 21872 is a pointer p. When p is nonzero
//     and the word at p + 12 has bit 0x400000 set, a flag is raised (it is never lowered).
//     Whether p is zero or not, the point at p + 16 (three floats; for a zero p this is address 16,
//     which the original reads without a test) is compared with the point at self + 16: the
//     squared distance d = (dx*dx + dy*dy) + dz*dz is converted to an int (truncated).
//     - d < 14400: netPlayerIsLocal__Fi(idx); when nonzero byte 201 of self becomes 1. Byte 200
//       is incremented and the near count rises by one. nmPlayersAlive is read again after the
//       call.
//     - d < 22500 (any d, including the near ones): the second count rises by one.
//  3. When the signed byte at 198 is zero and the second count is zero, byte 198 becomes 1.
//  4. Byte 199 becomes the low 8 bits of the near count, unless that is nonzero and the flag is
//     set, when it becomes 0xff.
// Inferred: 14400 and 22500 are the squares of 120 and 150, so the two counts are the players
// within 120 and within 150 distance units of the save point; byte 199 > 0 means "players near",
// -1 "players near but flagged", which the caller (the save point run) reads.
// Unknown: what the bit 0x400000 of the word at p + 12 means, and what p points to.
extern "C" {
extern int nmPlayersAlive;
extern int playersAlive[4];
extern unsigned char player[];
int netPlayerIsLocal__Fi(int player_index);
}

#ifdef W11_FRONT_A_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
inline unsigned readWord(__UINTPTR_TYPE__ address) {
    unsigned v;
    __builtin_memcpy(&v, reinterpret_cast<const void*>(address), 4);
    return v;
}
inline __UINTPTR_TYPE__ wordAddress(unsigned word) {
#ifdef W11_FRONT_A_HOST
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
    int farCount = 0;
    bool flagged = false;
    for (int i = 0; i < nmPlayersAlive; ++i) {
        const int index = playersAlive[i];
        const __UINTPTR_TYPE__ p =
            wordAddress(readWord(reinterpret_cast<__UINTPTR_TYPE__>(player) + index * 23984 + 21872));
        if (p != 0 && (readWord(p + 12) & 0x400000u) != 0) flagged = true;
        const float* point = reinterpret_cast<const float*>(p + 16);
        const float* mine = reinterpret_cast<const float*>(object + 16);
        const float dx = point[0] - mine[0];
        const float dy = point[1] - mine[1];
        const float dz = point[2] - mine[2];
        const int d = static_cast<int>(dx * dx + dy * dy + dz * dz);
        if (d < 14400) {
            if (netPlayerIsLocal__Fi(index)) object[201] = 1;
            ++object[200];
            ++nearCount;
        }
        if (d < 22500) ++farCount;
    }
    if (static_cast<signed char>(object[198]) == 0 && farCount == 0) object[198] = 1;
    object[199] = static_cast<unsigned char>(nearCount);
    if ((nearCount & 0xff) != 0 && flagged) object[199] = 0xff;
}
