// Observed eventSteeringChange__11LavaTractor at 0x284eb8, extent 104.
// eRand__10GameObject runs on the object. Its result modulo 3 (unsigned) plus
// 1 fills the %d of the format "lavatractor_wheel_turn%d.anm" (at 0x476df0) in a 128-byte
// local. lumpFindResource looks that name up in the lump named "tractor.lmp"
// (at 0x476d50), and soundPlaySequence starts the animation header on the
// object at volume 1.0. The result of the sound call is dropped.
// Unknown: what the three numbered variants are.
extern "C" unsigned eRand__10GameObject(void* object);
extern "C" int sprintf(char* buffer, const char* format, ...);
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);

namespace {
char tractor_lump[] = "tractor.lmp";
const char format[] = "lavatractor_wheel_turn%d.anm";
}

extern "C" void eventSteeringChange__11LavaTractor(void* object) {
    char name[128];
    sprintf(name, format, eRand__10GameObject(object) % 3u + 1u);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(tractor_lump, name), object, 1.0f);
}
