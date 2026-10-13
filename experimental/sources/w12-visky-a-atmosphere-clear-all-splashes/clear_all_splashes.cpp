// Observed: ClearAllSplashes__12VIAtmosphere at 0x0106b8b0 is 176 bytes. Words of the
// atmosphere object (this): w1228 at byte offset 1228, w32, w48 and a signed count at +8. It
// returns without doing anything when w1228 is zero, or else when w32 is zero, and also
// (after those two checks) when w48 is zero. Otherwise it stores zero at +32, runs
// Init__7VILList on this+20 and then on this+36, and when the count at +8 is positive calls
// AddTail__7VILListP7VILNode(this+36, w48 + 36 * i) for i from 0 to count-1, re-reading w48 and the
// count each time. Nothing is returned.
// Inferred: this+20 and this+36 are two list heads (in use and free); w48 points to a pool of
// 36-byte splash records, and the clear rebuilds the free list from the whole pool.
// Unknown: member names and what +32 counts.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void Init__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);

void ClearAllSplashes__12VIAtmosphere(unsigned char* self) {
    if (*reinterpret_cast<const unsigned*>(self + 1228) == 0) return;
    if (*reinterpret_cast<const unsigned*>(self + 32) == 0) return;
    if (*reinterpret_cast<const unsigned*>(self + 48) == 0) return;
    *reinterpret_cast<unsigned*>(self + 32) = 0;
    Init__7VILList(self + 20);
    Init__7VILList(self + 36);
    for (int i = 0; i < *reinterpret_cast<const int*>(self + 8); ++i)
        AddTail__7VILListP7VILNode(self + 36, NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(self + 48)) + 36 * i);
}
}
