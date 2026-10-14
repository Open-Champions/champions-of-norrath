// Observed eventCollisionWall__11LavaTractorP6Point3T1 at 0x284e10, extent 164.
// A dot product of the two vectors is summed as (b.x*a.x + b.y*a.y) + b.z*a.z.
// When it is less than the float -2.6469669 (bits 0xc02967e8, the .sdata
// word at 0x482410) the same random-animation sequence as the other
// LavaTractor events runs, with the format "lavatractor_coll_metal%d.anm" at
// 0x476dd0. Otherwise nothing is called. eRand runs only on the taken path.
// Unknown: why the threshold is below -1 for what looks like a normal test.
extern "C" unsigned eRand__10GameObject(void* object);
extern "C" int sprintf(char* buffer, const char* format, ...);
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);

namespace {
char tractor_lump[] = "tractor.lmp";
const char format[] = "lavatractor_coll_metal%d.anm";
union FloatBits { unsigned bits; float value; };
const FloatBits threshold = {0xc02967e8u};
}

extern "C" void eventCollisionWall__11LavaTractorP6Point3T1(void* object, const float* a, const float* b) {
    const float dot = b[0] * a[0] + b[1] * a[1] + b[2] * a[2];
    if (dot < threshold.value) {
        char name[128];
        sprintf(name, format, eRand__10GameObject(object) % 3u + 1u);
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(tractor_lump, name), object, 1.0f);
    }
}
