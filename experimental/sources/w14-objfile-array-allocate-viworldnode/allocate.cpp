// Observed: Allocate__t7VIArray1Z11VIWorldNode at 0x012100f8, 136 bytes. Offsets are bytes from the object; n is the word at +0.
//  1. n == 0: return 0 with nothing stored.
//  2. Otherwise p = __builtin_vec_new(n * 24) and p is stored at +4 even when it is null; a null p returns -1.
//  3. The count at +0 is read again; when it is positive, each of that many 24-byte elements (when its address is nonzero) gets
//     its first four words (bytes 0 to 15) set to float zero. The remaining eight bytes are untouched. The result is 0.
// Inferred: +4 is the element storage; each element's constructor zeroes four floats (the bit pattern stored is zero).
// Unknown: the element type's real members, and what the last eight bytes of an element hold.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
static inline unsigned address_of(const void* pointer) { return norrath_host_address(pointer); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
static inline unsigned address_of(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(pointer)); }
#endif

extern "C" {
void* __builtin_vec_new(unsigned size);

int Allocate__t7VIArray1Z11VIWorldNode(void* array) {
    unsigned* words = static_cast<unsigned*>(array);
    if (words[0] == 0) return 0;
    unsigned* storage = static_cast<unsigned*>(__builtin_vec_new(words[0] * 24));
    words[1] = address_of(storage);
    if (storage == 0) return -1;
    int n = static_cast<int>(words[0]);
    unsigned* element = reinterpret_cast<unsigned*>(storage);
    for (; n > 0; --n, element += 6) {
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
