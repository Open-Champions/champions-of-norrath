// Observed: MoveToLRUEnd__13VIEESurfCachei at 0x011335f0 is 380 bytes in the pinned SLUS-20565 image. It uses the same list as
// RemoveFromLRU__13VIEESurfCachei (first index at +8, last index at +12, entry array pointer at +64, 104-byte entries with "next" at
// +92 and "previous" at +96). For entry index i:
//   - if i already equals the last index, nothing changes;
//   - if the first index is -1 (empty list), entry[i].next = entry[i].previous = -1 and both first and last become i;
//   - otherwise it unlinks i: if i is the first entry, entry[entry[i].next].previous = -1 and first = entry[i].next; else, if
//     entry[i].previous is not -1, entry[previous].next = entry[i].next, and if entry[i].next is not -1, entry[next].previous =
//     entry[i].previous;
//     then it appends: entry[last].next = i, entry[i].next = -1, entry[i].previous = last, last = i.
// It always returns 0.
// Inferred: moves a cache entry to the most-recently-used end of the doubly linked list.
// Unknown: the rest of the entry, behavior for an index that is not in the list, and capacity. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int MoveToLRUEnd__13VIEESurfCachei(unsigned char* self, int index) {
    int* first = reinterpret_cast<int*>(self + 8);
    int* last = reinterpret_cast<int*>(self + 12);
    if (*last == index) return 0;
    unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
    int* mine = reinterpret_cast<int*>(entries + index * 104 + 92);
    if (*first == -1) {
        mine[1] = -1;
        mine[0] = -1;
        *last = index;
        *first = index;
        return 0;
    }
    if (*first == index) {
        *reinterpret_cast<int*>(entries + mine[0] * 104 + 96) = -1;
        *first = mine[0];
    } else {
        if (mine[1] != -1) *reinterpret_cast<int*>(entries + mine[1] * 104 + 92) = mine[0];
        if (mine[0] != -1) *reinterpret_cast<int*>(entries + mine[0] * 104 + 96) = mine[1];
    }
    *reinterpret_cast<int*>(entries + *last * 104 + 92) = index;
    mine[0] = -1;
    mine[1] = *last;
    *last = index;
    return 0;
}
}
