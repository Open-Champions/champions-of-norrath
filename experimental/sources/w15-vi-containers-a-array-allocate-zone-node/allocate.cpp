// Observed: Allocate__t7VIArray1Z10VIZoneNode at 0x01214fb8, 136 bytes. Offsets are bytes from the object; n is the word at +0.
//  1. n == 0: return 0 with nothing stored.
//  2. Otherwise p = __builtin_vec_new(n * 24) and p is stored at +4 even when it is null; a null p returns -1.
//  3. The count at +0 is read again. When it is positive, each of that many 24-byte elements starting at p gets the four words at +0,
//     +4, +8 and +12 set to float zero (all-zero bits); the element is skipped when its address is zero, which cannot happen after the
//     null test above. The result is 0.
// Inferred: 24-byte elements constructed in place by an inlined constructor that clears their first four words.
// Unknown: the element's members, and what the last two words hold.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned norrath_host_address(const void* pointer);
static inline unsigned address_of(const void* pointer) { return norrath_host_address(pointer); }
#else
static inline unsigned address_of(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(pointer)); }
#endif

extern "C" {
void* __builtin_vec_new(unsigned size);

int Allocate__t7VIArray1Z10VIZoneNode(void* array) {
    unsigned* words = static_cast<unsigned*>(array);
    if (words[0] == 0) return 0;
    unsigned* storage = static_cast<unsigned*>(__builtin_vec_new(words[0] * 24));
    words[1] = address_of(storage);
    if (storage == 0) return -1;
    int n = static_cast<int>(words[0]);
    unsigned* element = storage;
    for (int i = 0; i < n; ++i, element += 6) {
        if (element != 0) {
            element[0] = 0;
            element[1] = 0;
            element[2] = 0;
            element[3] = 0;
        }
    }
    return 0;
}
}
