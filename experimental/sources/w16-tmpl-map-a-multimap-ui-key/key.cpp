// Observed: Key__Ct10VIMultiMap2ZUiZii at 0x01222910 is 28 bytes. With (map, index) it returns
// the address of the key word of an element: the array base in map+8 plus index * 24 plus 16.
// No memory is read beyond the base word and nothing is written; index 0 is not special-cased.
// Inferred: the multimap's elements are 24 bytes with the key at +16, like VIMap<int,int>.
// Unknown: the template parameters beyond the mangled name.
extern "C" {
void* Key__Ct10VIMultiMap2ZUiZii(const void* map, int index) {
    const unsigned char* bytes = static_cast<const unsigned char*>(map);
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(bytes + 8)));
    return elements + index * 24 + 16;
}
}
