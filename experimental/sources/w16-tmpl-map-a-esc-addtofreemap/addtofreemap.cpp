// Observed: AddToFreeMap__13VIEESurfCachei at 0x01132b20 is 148 bytes. The cache holds a record
// array whose base is the word at cache+40 (records are 24 bytes) and an embedded
// VIMultiMap<unsigned,int> at cache+76. With (cache, index) it returns -1 when the word at
// record+12 of that index is not negative. Otherwise it builds the key (record word +8 << 24) |
// (record word +4), calls Insert__t10VIMultiMap2ZUiZiRCUiRCi(cache+76, &key, &index) with both
// arguments held in stack memory, and returns -1 when the result is negative. On success it
// re-reads the index from its stack slot and the record base from cache+40, stores the insert
// result at record+12 of that index, and returns 0.
// Inferred: the record word at +12 holds the element index of the record in the free multimap
// (-1 when it is not entered), and the key packs a small field in the top byte over a 24-bit
// value, so entries sort by the +8 field first. The meaning of the record fields is unknown.
// Unknown: what the callers do with the key ordering.
extern "C" {
int Insert__t10VIMultiMap2ZUiZiRCUiRCi(void* map, const unsigned* key, const int* value);

int AddToFreeMap__13VIEESurfCachei(void* cache, int index) {
    unsigned char* bytes = static_cast<unsigned char*>(cache);
    int slot = index;
    unsigned key;
    unsigned char* records = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(bytes + 40)));
    unsigned char* record = records + slot * 24;
    if (*reinterpret_cast<int*>(record + 12) >= 0) return -1;
    key = (*reinterpret_cast<unsigned*>(record + 8) << 24) | *reinterpret_cast<unsigned*>(record + 4);
    const int entered = Insert__t10VIMultiMap2ZUiZiRCUiRCi(bytes + 76, &key, &slot);
    if (entered < 0) return -1;
    records = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(bytes + 40)));
    *reinterpret_cast<int*>(records + slot * 24 + 12) = entered;
    return 0;
}
}
