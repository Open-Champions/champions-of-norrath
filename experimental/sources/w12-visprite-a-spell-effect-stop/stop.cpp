// Observed: Stop__19VISpellEffectSystemi at 0x01002e58, 124 bytes. With the
// object as this and id the second argument: q = Playback__19VISpellEffectSystemi(this, id).
// When q is zero nothing else happens. Otherwise, if the word at q + 32 is
// non-zero, _$_19VISpellEffectDriver(that word, 3) is called; then the table
// word at this + 4 + 4 * (u % 24) is set to zero, u being the unsigned byte at
// q + 28, and DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(this, q)
// is called, in that order. The image leaves an unspecified value in v0 on
// both paths, so no result is defined here.
// Inferred: q + 32 is a driver object released with delete flags 3 (the same
// retire sequence appears in Process__19VISpellEffectSystem).
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
char* Playback__19VISpellEffectSystemi(void* self, int id);
void _$_19VISpellEffectDriver(void* driver, int flags);
void DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(void* self, char* playback);

void Stop__19VISpellEffectSystemi(void* self, int id) {
    char* q = Playback__19VISpellEffectSystemi(self, id);
    if (!q) return;
    unsigned driver = *reinterpret_cast<unsigned*>(q + 32);
    if (driver) _$_19VISpellEffectDriver(NORRATH_TRANSLATE(driver), 3);
    unsigned slot = *reinterpret_cast<unsigned char*>(q + 28) % 24;
    *reinterpret_cast<int*>(static_cast<char*>(self) + 4 + 4 * slot) = 0;
    DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(self, q);
}
}
