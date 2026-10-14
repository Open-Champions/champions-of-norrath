// Observed: Allocate__t7VIArray1Z10VIZoneRoom at 0x01213ea0, 156 bytes. Offsets are bytes from the object; n is the word at +0.
//  1. n == 0: return 0 with nothing stored.
//  2. Otherwise p = __builtin_vec_new(n * 120) and p is stored at +4 even when it is null; a null p returns -1.
//  3. The count at +0 is read again. When it is positive, for each index i from 0 while i < count (the count is read afresh
//     after every constructor call) the element address is the word at +4 plus 120 * i, read again each time; when that address is nonzero
//     __10VIZoneRoom is called with it. The result is 0.
// Inferred: 120-byte elements constructed in place by __10VIZoneRoom (a constructor taking only the address).
// Unknown: the element's members, and the constructor's result (it is ignored).
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
void __10VIZoneRoom(void* room);

int Allocate__t7VIArray1Z10VIZoneRoom(void* array) {
    unsigned* words = static_cast<unsigned*>(array);
    if (words[0] == 0) return 0;
    void* storage = __builtin_vec_new(words[0] * 120);
    words[1] = address_of(storage);
    if (storage == 0) return -1;
    int n = static_cast<int>(words[0]);
    for (int i = 0; i < n; ++i) {
        unsigned char* element = at(words[1]) + 120 * i;
        if (element != 0) {
            __10VIZoneRoom(element);
            n = static_cast<int>(words[0]);
        }
    }
    return 0;
}
}
