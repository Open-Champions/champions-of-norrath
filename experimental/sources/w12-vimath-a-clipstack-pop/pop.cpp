// Observed: Pop__11VIClipStack at 0x01099548 is 64 bytes. Fields of the object (a0):
// signed count at +0, pointer to an array of 16-byte entries at +8, running totals at
// +12 and +24. It takes the entry at index count-1, subtracts entry word +8 from the
// total at +12 and entry word +0 from the total at +24, stores count-1 at +0, and
// returns 0. The stores happen in the order +12, +0, +24. There is no empty-stack check.
// Inferred: this undoes one PushFrustum (which would add the same two entry words to
// the totals). The entry words +0 and +8 are signed counts. Unknown: member names.
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
int Pop__11VIClipStack(unsigned char* self) {
    int* count = reinterpret_cast<int*>(self + 0);
    int* total_b = reinterpret_cast<int*>(self + 12);
    int* total_a = reinterpret_cast<int*>(self + 24);
    int index = *count - 1;
    const int* entry = reinterpret_cast<const int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(self + 8)) + (index << 4));
    *total_b -= entry[2];
    int first = entry[0];
    *count = index;
    *total_a -= first;
    return 0;
}
}
