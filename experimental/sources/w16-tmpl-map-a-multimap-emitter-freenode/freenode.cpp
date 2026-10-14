// Observed: FreeNode__t10VIMultiMap2ZfZ14VIEmitterEntryi at 0x011f4f20 is 136 bytes. With (map,
// index) it runs _$_10VIMatrix44 with flag 2 on the member 48 bytes into
// the 112-byte element (array base at map+8), then pushes the element on the free chain: element
// +4 takes the head word at map+16, element +8 becomes -2, the head becomes the index, and the
// used count at map+0 is decremented. It returns 0.
// Inferred: the matrix member is destroyed in place with the no-delete flag (bit 0 clear); -2 in
// the left link marks a free element, and the free chain runs through +4.
// Unknown: the layout of the rest of the element; template parameters beyond the mangled name.
extern "C" {
void _$_10VIMatrix44(void* matrix, int flags);

static inline unsigned* word(void* map, unsigned offset) {
    return reinterpret_cast<unsigned*>(static_cast<unsigned char*>(map) + offset);
}
static inline unsigned char* element(void* map, int index) {
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*word(map, 8)));
    return elements + index * 112;
}

int FreeNode__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* map, int index) {
    _$_10VIMatrix44(element(map, index) + 48, 2);
    *reinterpret_cast<unsigned*>(element(map, index) + 4) = *word(map, 16);
    *reinterpret_cast<int*>(element(map, index) + 8) = -2;
    *word(map, 16) = static_cast<unsigned>(index);
    *word(map, 0) -= 1;
    return 0;
}
}
