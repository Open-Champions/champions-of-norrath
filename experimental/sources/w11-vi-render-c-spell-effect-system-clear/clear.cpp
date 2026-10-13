// Observed Clear__19VISpellEffectSystem at 0x01002bc0, 136 bytes. With the object as this:
//   If the word at this + 1092 is zero: store zero at this + 1104 and return 0.
//   Otherwise: p = FirstPlayback(this). While p is nonzero, next = NextPlayback(this, p) is
//   fetched first, then Stop__19VISpellEffectSystemi(this, *(int*)(p + 28)) runs and p = next.
//   Then ReleaseAllSpellEffects__19VISpellEffectSystemR12VIDictionary(this, dict) with dict the
//   word at scene + 19352, scene being the word at this + 0. Finally 0 is stored at this + 1092
//   and this + 1104 and the result is zero.
// NextPlayback is declared with the two argument registers the call site sets up; its mangled
// name suggests further parameters that this caller does not pass.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
char* FirstPlayback__19VISpellEffectSystem(void* self);
char* NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(void* self, char* playback);
int Stop__19VISpellEffectSystemi(void* self, int id);
int ReleaseAllSpellEffects__19VISpellEffectSystemR12VIDictionary(void* self, unsigned dictionary);

int Clear__19VISpellEffectSystem(void* self) {
    char* base = static_cast<char*>(self);
    if (*reinterpret_cast<int*>(base + 1092) == 0) {
        *reinterpret_cast<int*>(base + 1104) = 0;
        return 0;
    }
    char* p = FirstPlayback__19VISpellEffectSystem(self);
    while (p) {
        char* next = NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(self, p);
        Stop__19VISpellEffectSystemi(self, *reinterpret_cast<int*>(p + 28));
        p = next;
    }
    const char* scene = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(base));
    ReleaseAllSpellEffects__19VISpellEffectSystemR12VIDictionary(self, *reinterpret_cast<const unsigned*>(scene + 19352));
    *reinterpret_cast<int*>(base + 1092) = 0;
    *reinterpret_cast<int*>(base + 1104) = 0;
    return 0;
}
}
