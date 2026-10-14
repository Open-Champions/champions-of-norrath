// Observed: FindFree__13VIEESurfCacheR13VIEESurfAlloc at 0x01132cf8 is 252 bytes. The cache holds
// an embedded VIMultiMap<unsigned,int> at cache+76 (24-byte elements from the base at map+8,
// key at +16, value at +20). With (cache, alloc) it first forms the key (alloc word +4 << 24) |
// (alloc word +8) and calls LowerBound__Ct10VIMultiMap2ZUiZiRCUi(map, &key). When the result is
// not End__Ct10VIMultiMap2ZUiZi(map) and the top byte of Key__Ct10VIMultiMap2ZUiZii(map, result)
// equals alloc word +4 (compared as a whole word), it stores that element's value word (+20) in
// alloc word +0 and returns 1. Otherwise it forms the key alloc word +8 | 0xff000000, calls
// LowerBound again, and when the result is not End it stores that element's value in alloc word
// +0 and returns 1; at End it returns 0 and leaves alloc word +0 alone.
// Inferred: a best-fit style search of the free multimap: first an entry whose top key byte
// matches the requested class and whose remaining bits are at least the request, then any entry
// at or above class 0xff. The meaning of the alloc fields is unknown.
// Unknown: field names of the allocation request.
extern "C" {
int LowerBound__Ct10VIMultiMap2ZUiZiRCUi(const void* map, const unsigned* key);
int End__Ct10VIMultiMap2ZUiZi(const void* map);
const unsigned char* Key__Ct10VIMultiMap2ZUiZii(const void* map, int index);

static inline int value_of(const unsigned char* map, int index) {
    const unsigned char* elements = reinterpret_cast<const unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(map + 8)));
    return *reinterpret_cast<const int*>(elements + index * 24 + 20);
}

int FindFree__13VIEESurfCacheR13VIEESurfAlloc(void* cache, void* request) {
    const unsigned char* map = static_cast<const unsigned char*>(cache) + 76;
    unsigned* alloc = static_cast<unsigned*>(request);
    unsigned key = (alloc[1] << 24) | alloc[2];
    int found = LowerBound__Ct10VIMultiMap2ZUiZiRCUi(map, &key);
    if (found != End__Ct10VIMultiMap2ZUiZi(map)) {
        if (static_cast<unsigned>(Key__Ct10VIMultiMap2ZUiZii(map, found)[3]) == alloc[1]) {
            alloc[0] = static_cast<unsigned>(value_of(map, found));
            return 1;
        }
    }
    key = alloc[2] | 0xff000000u;
    found = LowerBound__Ct10VIMultiMap2ZUiZiRCUi(map, &key);
    if (found == End__Ct10VIMultiMap2ZUiZi(map)) return 0;
    alloc[0] = static_cast<unsigned>(value_of(map, found));
    return 1;
}
}
