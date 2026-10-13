// Observed playerBlockSparks__6Playeri at 0x001a7d20, 1592 bytes: arguments the player object and a
// kind (0, 1 or 2), no result. Byte offsets are into the object.
//  1. With kind 0, the animation "hit_mtlplate_slash_miss.anm" of lump "fx.lmp" is played with
//     soundPlaySequence at volume 1.0 on the word at player + (word at +960) * 23984 + 21872.
//  2. Two character attachment points (modelGetCharAttachmentPos, indices 0 and 1, on the model
//     word at +1332, with a copy of the position floats at +16, the halfword at +56 and the
//     animation state at +288) are averaged: mid = (point0 + point1) * 0.5.
//  3. Kind 0: base = (icos(yaw) * 10, isin(yaw) * 10, 10) with yaw the halfword at +56. Fifty times
//     a random point of the unit ball is drawn (axes 2 * r/65536 - 1 from three eRand calls per
//     try, retried while the squared length is above 1), scaled by 5 and ADDED to base, which is
//     not reset between passes; trailParticleSpark is added at mid with velocity base and
//     lifetime 0.0.
//  4. Other kinds: for each shadow point i of the model (modelGetShadowPos, on the animation state at
//     (word at record + 21872) + 288, the position copy, the yaw, i): point is the shadow point;
//     above = (x + 0.0, y, z + 36.0) of the object position. Five times: base = (point - above) *
//     0.349; a unit-ball random vector (axes from three consecutive steps of syncRandState, r = state
//     >> 16, state = state * 0x19660d + 0x3c6ef35f, same retry rule) scaled by 4.77 is added to base;
//     with kind 1 blueSpark, with kind 2 redSpark is added at the shadow point with that velocity
//     and lifetime (next state >> 16) * 2^-16 * 0.1. Other kinds add nothing.
// Inferred: sparks when blocking a hit (kind 0 is the metal-plate sound at the weapon, 1 and 2 are
// coloured bursts at the model's shadow points).
// Unknown: the meaning of kinds 1 and 2, and of the 0.349 and 4.77 scales.
extern "C" {
extern unsigned char player[];
extern int syncRandState;
extern unsigned char trailParticleSpark[];
extern unsigned char blueSpark[];
extern unsigned char redSpark[];
struct Point3 { float x, y, z; };
int sprintf(char* out, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* model, Point3 position, short yaw, void* state,
                                                                                         int index, Point3* out, Point3* second, float scale);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* model, void* state, Point3 position, short yaw,
                                                                                         int index, Point3* point, float* value, void* matrix,
                                                                                         float scale);
float icos__Fi(int angle);
float isin__Fi(int angle);
int eRand__10GameObject(void* self);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, Point3* position, Point3* velocity, float lifetime);
}

namespace {
template <class T>
inline T load(const unsigned char* base, int offset) {
    T value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
const float kInv = 1.0f / 65536.0f;
}

extern "C" void playerBlockSparks__6Playeri(void* self, int kind) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (kind == 0) {
        char name[128];
        sprintf(name, "hit_mtlplate_slash_miss.anm");
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name),
            load<void*>(player, *reinterpret_cast<int*>(object + 960) * 23984 + 21872), 1.0f);
    }
    void* model = load<void*>(object, 1332);
    const short yaw = *reinterpret_cast<short*>(object + 56);
    unsigned char* state = object + 288;
    Point3 first, second;
    Point3 copy = *reinterpret_cast<Point3*>(object + 16);
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, copy, yaw, state, 0, &first, nullptr, 1.0f);
    copy = *reinterpret_cast<Point3*>(object + 16);
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, copy, yaw, state, 1, &second, nullptr, 1.0f);
    Point3 mid = {(first.x + second.x) * 0.5f, (first.y + second.y) * 0.5f, (first.z + second.z) * 0.5f};

    if (kind == 0) {
        Point3 base;
        base.x = icos__Fi(yaw) * 10.0f;
        base.y = isin__Fi(yaw) * 10.0f;
        base.z = 10.0f;
        for (int i = 0; i < 50; ++i) {
            Point3 unit;
            do {
                unit.x = static_cast<float>(eRand__10GameObject(self)) * kInv;
                unit.x = unit.x + unit.x - 1.0f;
                unit.y = static_cast<float>(eRand__10GameObject(self)) * kInv;
                unit.y = unit.y + unit.y - 1.0f;
                unit.z = static_cast<float>(eRand__10GameObject(self)) * kInv;
                unit.z = unit.z + unit.z - 1.0f;
            } while (1.0f < unit.x * unit.x + unit.y * unit.y + unit.z * unit.z);
            unit.x *= 5.0f;
            unit.y *= 5.0f;
            unit.z *= 5.0f;
            base.x += unit.x;
            base.y += unit.y;
            base.z += unit.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(trailParticleSpark, &mid, &base, 0.0f);
        }
        return;
    }

    const int points = modelGetNmShadowPoints__FP12_modelHeader(model);
    for (int i = 0; i < points; ++i) {
        Point3 shadow;
        float value;
        Point3 positionCopy = *reinterpret_cast<Point3*>(object + 16);
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, load<unsigned char*>(player, *reinterpret_cast<int*>(object + 960) * 23984 + 21872) + 288, positionCopy, yaw, i, &shadow, &value,
            nullptr, 1.0f);
        Point3 hold = shadow;
        const Point3* position = reinterpret_cast<const Point3*>(object + 16);
        const Point3 above = {position->x + 0.0f, position->y, position->z + 36.0f};
        for (int n = 4; n >= 0; --n) {
            Point3 delta = {shadow.x - above.x, shadow.y - above.y, shadow.z - above.z};
            Point3 base = {delta.x * 0x1.656040p-2f, delta.y * 0x1.656040p-2f, delta.z * 0x1.656040p-2f};
            Point3 unit;
            do {
                unsigned state1 = static_cast<unsigned>(syncRandState) * 0x19660du + 0x3c6ef35fu;
                unsigned state2 = state1 * 0x19660du + 0x3c6ef35fu;
                unsigned state3 = state2 * 0x19660du + 0x3c6ef35fu;
                unit.x = static_cast<float>(state1 >> 16) * kInv;
                unit.y = static_cast<float>(state2 >> 16) * kInv;
                unit.z = static_cast<float>(state3 >> 16) * kInv;
                unit.x = unit.x + unit.x - 1.0f;
                unit.y = unit.y + unit.y - 1.0f;
                unit.z = unit.z + unit.z - 1.0f;
                syncRandState = static_cast<int>(state3);
            } while (1.0f < unit.x * unit.x + unit.y * unit.y + unit.z * unit.z);
            unit.x *= 0x1.3147aep+2f;
            unit.y *= 0x1.3147aep+2f;
            unit.z *= 0x1.3147aep+2f;
            base.x += unit.x;
            base.y += unit.y;
            base.z += unit.z;
            if (kind == 1 || kind == 2) {
                const unsigned next = static_cast<unsigned>(syncRandState) * 0x19660du + 0x3c6ef35fu;
                syncRandState = static_cast<int>(next);
                P_AddParticle__FP11ParticleDefP6Point3T1f(kind == 1 ? blueSpark : redSpark, &hold, &base,
                                                          static_cast<float>(next >> 16) * kInv * 0x1.999998p-4f);
            }
        }
    }
}
