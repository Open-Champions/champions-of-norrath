// Observed: Key__Ct5VIMap2ZiZ17VIEESurfCacheInfoi at 0x01222838 is 36 bytes. With (map, index)
// it returns the address of the key word of an element: the array base in map+8 plus
// index * 104 plus 16. Nothing is written and index 0 is not special-cased.
// Inferred: elements of this map are 104 bytes (the same 16-byte tree header and key as the
// other maps, followed by the 17VIEESurfCacheInfo value). Unknown: layout of the value.
extern "C" {
void* Key__Ct5VIMap2ZiZ17VIEESurfCacheInfoi(const void* map, int index) {
    const unsigned char* bytes = static_cast<const unsigned char*>(map);
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(bytes + 8)));
    return elements + index * 104 + 16;
}
}
