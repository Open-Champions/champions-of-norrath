// Observed: RemoveFromLRU__13VIEESurfCachei at 0x01133770 is 280 bytes in the pinned SLUS-20565 image. The cache object keeps an
// entry-array pointer at +64, a first-entry index at +8 and a last-entry index at +12 (-1 appears to mean none). Entries are 104 bytes
// apart; entry e has two link indices, "next" at +92 and "previous" at +96 (named here only from how the routine uses them). Given an
// entry index i it:
//   1. if the first index equals i, sets the first index to entry[i].next; otherwise sets entry[entry[i].previous].next = entry[i].next
//      (no -1 check);
//   2. if the last index equals i, sets the last index to entry[i].previous; otherwise sets entry[entry[i].next].previous =
//      entry[i].previous (no -1 check);
//   3. sets entry[i].next and entry[i].previous to -1 and returns 0.
// Inferred: a doubly linked list of cache entries in least-recently-used order, first = least recent, with the link fields at +92/+96
// inside each 104-byte entry (the same list MoveToLRUEnd__13VIEESurfCachei maintains).
// Unknown: the rest of the entry, the capacity, behavior for an index that is not in the list, and which end is "least recent".
// Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int RemoveFromLRU__13VIEESurfCachei(unsigned char* self, int index) {
    int* first = reinterpret_cast<int*>(self + 8);
    int* last = reinterpret_cast<int*>(self + 12);
    unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
    int* mine = reinterpret_cast<int*>(entries + index * 104 + 92);
    if (*first == index) {
        *first = mine[0];
    } else {
        *reinterpret_cast<int*>(entries + mine[1] * 104 + 92) = mine[0];
    }
    if (*last == index) {
        *last = mine[1];
    } else {
        *reinterpret_cast<int*>(entries + mine[0] * 104 + 96) = mine[1];
    }
    mine[0] = -1;
    mine[1] = -1;
    return 0;
}
}
