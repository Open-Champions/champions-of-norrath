// Observed: DestroyPool__t7VIArray1Z10VIZoneRoom at 0x01213db0, 160 bytes. Offsets are bytes from the object; n is the word at +0.
//  1. n == 0: return 0 without calling anything.
//  2. Otherwise for i = 0 upward: read the word at +4 afresh, call _$_10VIZoneRoom(storage + 120 * i, 2), then move on unless i was
//     n - 1, where n is the count read just before that call (it is read once at the start of each pass, not after the call). After
//     each call except the last, +0 is read again for the next pass. The result is 0.
// Inferred: destroys the 120-byte elements in place through the room destructor with flags 2 (in-place, no delete of the element),
// in index order. A negative count makes the image's loop run for billions of passes; that case is not reproduced.
// Unknown: whether the room destructor can change +0 or +4 (they are simply re-read, as the image does).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
void _$_10VIZoneRoom(void* room, int flags);

int DestroyPool__t7VIArray1Z10VIZoneRoom(void* array) {
    unsigned* words = static_cast<unsigned*>(array);
    unsigned n = words[0];
    if (n == 0) return 0;
    unsigned i = 0;
    for (;;) {
        unsigned char* room = at(words[1]) + 120 * i;
        bool last = i == n - 1;
        _$_10VIZoneRoom(room, 2);
        if (last) break;
        ++i;
        n = words[0];
    }
    return 0;
}
}
