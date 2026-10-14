// Observed: Allocate__t7VIArray1Z11VIWorldLeaf at 0x01210220, 120 bytes. Offsets are bytes from the object; n is the word at +0.
//  1. n == 0: return 0 with nothing stored.
//  2. Otherwise p = __builtin_vec_new(n * 4) and p is stored at +4 even when it is null; a null p returns -1.
//  3. The count at +0 is read again and a loop over that many elements runs with an empty body (no stores, no calls). Result 0.
// Inferred: +4 is the element storage; 4-byte elements whose construction is a no-op.
// Unknown: the element type's real members.
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

int Allocate__t7VIArray1Z11VIWorldLeaf(void* array) {
    unsigned* words = static_cast<unsigned*>(array);
    if (words[0] == 0) return 0;
    void* storage = __builtin_vec_new(words[0] * 4);
    words[1] = address_of(storage);
    if (storage == 0) return -1;
    return 0;
}
}
