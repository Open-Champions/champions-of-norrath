// Observed: DestroyPool__t7VIArray1Z20VICSpritePartEmitter at 0x012084e0, 164 bytes. The object is the only argument. Offsets are bytes from the object; n is the word at +0.
//  1. n == 0: return 0 without calling anything.
//  2. Otherwise for i = 0 upward: read the word at +4 afresh, call _$_17VIParticleEmitter on element+16 (flags 2) in that order, then
//     move on unless i was n - 1, where n is the count read just before the calls (it is read once at the start of
//     each pass, not after the calls). After each pass except the last, +0 is read again for the next pass.
//  3. The result is 0.
// Inferred: destroys the 1056-byte elements in place with flags 2 (no delete of the element), in index order.
// A negative count makes the image's loop run for billions of passes; that case is not reproduced.
// Unknown: whether the destructor can change +0 or +4 (they are simply re-read, as the image does).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline void* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline void* at(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
void _$_17VIParticleEmitter(void* object, int flags);

int DestroyPool__t7VIArray1Z20VICSpritePartEmitter(void* array) {
    unsigned* words = static_cast<unsigned*>(array);
    unsigned n = words[0];
    if (n == 0) return 0;
    unsigned i = 0;
    for (;;) {
        unsigned char* element = static_cast<unsigned char*>(at(words[1])) + 1056 * i;
        bool last = i == n - 1;
        _$_17VIParticleEmitter(element + 16, 2);
        if (last) break;
        ++i;
        n = words[0];
    }
    return 0;
}
}
