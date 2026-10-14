// Observed eventDeActivate__11LavaTractor at 0x284cb0, extent 48.
// soundStopSound is called on the word at object+2360 and then on the word at
// object+2364. These are the two handles eventActivate stores there.
extern "C" void soundStopSound__Fi(int handle);

namespace {
typedef int __attribute__((may_alias)) AliasedInt;
}

extern "C" void eventDeActivate__11LavaTractor(void* object) {
    unsigned char* o = static_cast<unsigned char*>(object);
    soundStopSound__Fi(*reinterpret_cast<AliasedInt*>(o + 2360));
    soundStopSound__Fi(*reinterpret_cast<AliasedInt*>(o + 2364));
}
