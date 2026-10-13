// Observed Process__19VISpellEffectSystem at 0x01002898, 356 bytes. The result is zero.
// With the object as this and scene = the word at this + 0:
//   p = FirstPlayback(this); while p is nonzero:
//     next = NextPlayback(this, p)   (fetched before anything else touches p)
//     r = Process__19VISpellEffectDriver(*(p + 32))
//     if r == 0: q = Playback__19VISpellEffectSystemi(this, *(int*)(p + 28)); if q is nonzero, q is retired
//     else:      t = GetTime__7VIScene(scene); if (int)(t - *(long long*)(p + 16)) > 60000, q = Playback(...)
//                as above and q is retired when nonzero
//     p = next
//   Retiring q: if the word at q + 32 is nonzero, _$_19VISpellEffectDriver(that, 3); then, with id the
//   unsigned byte at q + 28, store zero at this + 4 + 4 * (id % 24) and call
//   DeletePlayback__19VISpellEffectSystem(this, q) in that order.
// Inferred: the word at p + 32 is a driver object and the 64-bit word at p + 16 is a start time in
// the unit GetTime returns; 60000 is compared as a signed difference truncated to 32 bits.
// NextPlayback and DeletePlayback are declared with the argument registers this caller sets.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
char* FirstPlayback__19VISpellEffectSystem(void* self);
char* NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(void* self, char* playback);
int Process__19VISpellEffectDriver(void* driver);
char* Playback__19VISpellEffectSystemi(void* self, int id);
void _$_19VISpellEffectDriver(void* driver, int flags);
void DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(void* self, char* playback);
long long GetTime__7VIScene(void* scene);

static void retire(void* self, char* q) {
    char* base = static_cast<char*>(self);
    unsigned driver = *reinterpret_cast<unsigned*>(q + 32);
    if (driver) _$_19VISpellEffectDriver(NORRATH_TRANSLATE(driver), 3);
    unsigned id = *reinterpret_cast<unsigned char*>(q + 28);
    *reinterpret_cast<int*>(base + 4 + 4 * (id % 24)) = 0;
    DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(self, q);
}

int Process__19VISpellEffectSystem(void* self) {
    char* base = static_cast<char*>(self);
    char* p = FirstPlayback__19VISpellEffectSystem(self);
    while (p) {
        char* next = NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(self, p);
        bool expired;
        if (Process__19VISpellEffectDriver(NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(p + 32))) == 0) {
            expired = true;
        } else {
            long long now = GetTime__7VIScene(NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(base)));
            expired = static_cast<int>(now - *reinterpret_cast<long long*>(p + 16)) > 60000;
        }
        if (expired) {
            char* q = Playback__19VISpellEffectSystemi(self, *reinterpret_cast<int*>(p + 28));
            if (q) retire(self, q);
        }
        p = next;
    }
    return 0;
}
}
