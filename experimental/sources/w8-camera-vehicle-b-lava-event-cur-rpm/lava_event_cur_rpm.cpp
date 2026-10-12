// Observed eventCurRPM__11LavaTractor at 0x284db0, extent 84.
// When the word at object+956 is -1 nothing happens. Otherwise, with r the
// float at object+740, soundChangeSequenceVolume is called on the handle at
// object+2360 with 1.0 - r, then on the handle at object+2364 with r reloaded
// from the object. Those are the idle and move handles from eventActivate.
// Inferred, not observed: r is the engine speed fraction.
extern "C" void soundChangeSequenceVolume__Fif(int handle, float volume);

namespace {
typedef int __attribute__((may_alias)) AliasedInt;
typedef float __attribute__((may_alias)) AliasedFloat;
}

extern "C" void eventCurRPM__11LavaTractor(void* object) {
    unsigned char* o = static_cast<unsigned char*>(object);
    if (*reinterpret_cast<AliasedInt*>(o + 956) == -1) return;
    soundChangeSequenceVolume__Fif(*reinterpret_cast<AliasedInt*>(o + 2360), 1.0f - *reinterpret_cast<AliasedFloat*>(o + 740));
    soundChangeSequenceVolume__Fif(*reinterpret_cast<AliasedInt*>(o + 2364), *reinterpret_cast<AliasedFloat*>(o + 740));
}
