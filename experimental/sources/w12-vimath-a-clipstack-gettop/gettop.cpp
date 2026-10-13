// Observed: GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane at 0x01099678 is 92 bytes.
// Arguments: the stack object (a0), then a1, a2, a3 and t0, all output pointers. The
// object holds a count at +0, a pointer to 16-byte entries at +8, a base pointer at +20
// and another base pointer at +32. With e = entries + (count-1)*16 it stores, in this
// order: *a1 = e[+8]; *a3 = e[+0]; *a2 = (+20 base) + e[+12] * 12; *t0 = (+32 base) +
// e[+4] * 16. It returns 0. Nothing in the object is changed and an empty stack is not
// checked.
// Inferred: the mangled name gives a1 and a3 the type int& and a2 the type VIVect3**,
// t0 the type VIPlane**; the 12-byte and 16-byte strides match the sizes of a vector of
// three floats and a plane record. The +20 base is the vector array and the +32 base is
// the plane array. The entry words +0 and +8 are the counts that Pop subtracts from the
// totals at +24 and +12.
// Unknown: member names and the meaning of the four entry words beyond their use here.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const unsigned char* pointer);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_POINTER_TO_WORD(pointer) norrath_host_address(pointer)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_POINTER_TO_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif

extern "C" {
int GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(const unsigned char* self, int* out_b, unsigned* out_vectors, int* out_a, unsigned* out_planes) {
    int index = *reinterpret_cast<const int*>(self) - 1;
    const int* entry = reinterpret_cast<const int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(self + 8)) + (index << 4));
    const unsigned planes = *reinterpret_cast<const unsigned*>(self + 32);
    const unsigned vectors = *reinterpret_cast<const unsigned*>(self + 20);
    *out_b = entry[2];
    int first = entry[0];
    int vector_index = entry[3];
    *out_a = first;
    int plane_index = entry[1];
    *out_vectors = vectors + vector_index * 12;
    *out_planes = planes + (plane_index << 4);
    return 0;
}
}
