// Observed msg_run__10DoorSecret at 0x213160, 348 bytes.
// Let t = frameNm - (word at object+84), the frames since msg_poke__10DoorSecreti stored
// the opening frame. Then, in order:
//  - if t < 20, add one to the unsigned halfword at +80;
//  - if t == 120, play the sequence "door_stnslid_open.anm" from the "fx.lmp" lump on the
//    object at rate 1.0 (soundPlaySequence);
//  - if t > 120, let s = (t - 120) * 0x3c888888 (as a float), limited to 1.0, and store
//    the truncation of evalHermite(s, 0, 1, 0, 1.5) * 400.0 in the halfword at +82;
//  - if t > 180, call worldSetDoorCollision(world, door, 0), door being the signed
//    halfword at +76;
//  - if t > 300, remove the object from the run list.
// Last, whatever t is, worldMoveDoor(world, door, point, 0) with the point
// (-(signed halfword at +80) * 0.25, (signed halfword at +82) * 0.25, 0).
// The sequence name sits at 0x470628 in the image's string area; this source holds
// its own copy.
// Unknown: what the halfwords at +80 and +82 mean beyond being door offsets.

extern "C" {
struct Point3 { float x, y, z; };
int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, Point3 point, short extra);
void worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int enable);
void objectRemoveFromRunList__FP10GameObject(void* object);
char* lumpFindResource__FPcT0(const char* lump, const char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
float evalHermite__Ffffff(float t, float a, float b, float c, float d);
extern int frameNm;
extern int world;
}

typedef short Half __attribute__((may_alias));
typedef unsigned short UHalf __attribute__((may_alias));
typedef int Word __attribute__((may_alias));

static inline float single_from_bits(unsigned bits) { return __builtin_bit_cast(float, bits); }

extern "C" void msg_run__10DoorSecret(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    int elapsed = frameNm - *reinterpret_cast<Word*>(object + 84);
    if (elapsed < 20) *reinterpret_cast<UHalf*>(object + 80) = *reinterpret_cast<UHalf*>(object + 80) + 1;
    if (elapsed == 120)
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "door_stnslid_open.anm"), self, 1.0f);
    if (elapsed > 120) {
        float progress = static_cast<float>(elapsed - 120) * single_from_bits(0x3c888888u);
        if (1.0f < progress) progress = 1.0f;
        float eased = evalHermite__Ffffff(progress, 0.0f, 1.0f, 0.0f, 1.5f);
        *reinterpret_cast<Half*>(object + 82) = static_cast<short>(static_cast<int>(eased * 400.0f));
    }
    if (elapsed > 180)
        worldSetDoorCollision__FP12_worldHeaderii(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(world)), *reinterpret_cast<Half*>(object + 76), 0);
    if (elapsed > 300) objectRemoveFromRunList__FP10GameObject(self);
    Point3 point = {static_cast<float>(-*reinterpret_cast<Half*>(object + 80)) * 0.25f,
                    static_cast<float>(*reinterpret_cast<Half*>(object + 82)) * 0.25f, 0.0f};
    worldMoveDoor__FP12_worldHeaderiG6Point3s(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(world)),
                                              *reinterpret_cast<Half*>(object + 76), point, 0);
}
