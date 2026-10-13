// Observed resurectEffect__6Player at 0x001a7698, 1420 bytes: one argument (the player object), no
// result. Byte offsets are into that object; record = player + (word at +960) * 23984.
//  1. The halfword at +1188 becomes 90 in a network game, else 60, and 1 when recallOn is set.
//     Bit 2 of the word at +12 is set.
//  2. A 188-byte block is taken with blockAlloc__Fi. When it succeeds, the LightEffect constructor
//     (__11LightEffectG6Point3ffffffffffffP10GameObjecti) runs on it with the position
//     (x + 0.0, y, z + 36.0) of the object (floats at +16), the twelve floats 128, 128, 256, -50,
//     0, 0, 0, 0, 240, 120, 0 and (float)halfword(+1188) / 60, a null object and 0.
//  3. The animation "spel_cas_curelw.anm" of lump "fx.lmp" is played with soundPlaySequence at
//     volume 1.0, its object argument being the word at record + 21872.
//  4. On the first call a local ParticleDef (80 bytes) is constructed with
//     (70, 83, 74, 35, 31, 43, 75, 55, 0.0, 19.6, 0.0, 3, 1, 8, null).
//  5. For each shadow point i of the model (word at +1332; modelGetNmShadowPoints__FP12_modelHeader),
//     modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f
//     (model, object + 288, a copy of the position, halfword at +56, i, &point, &value, null, 1.0) gives a point and a
//     value. cube += value^3 (cube carries over from point to point). While cube is above zero,
//     one particle is made per pass and cube falls by 80899997696.0:
//       scale = value * 1e-5 * 35.35; the jitter is point + ((r/65536 - 0.5) * scale) per axis
//       (three eRand calls: x, y, z); the direction is a random point of the unit ball (axes
//       2 * r/65536 - 1, three eRand calls per try, retried while the squared length is above 1),
//       scaled by 40.963 and added to the jitter point; D = that minus the jitter point;
//       velocity = D * -0.2808989 * 0.5; the lifetime is eRand * 2^-16 * 0.4; the particle is added
//       with P_AddParticle__FP11ParticleDefP6Point3T1f(def, position, velocity, lifetime).
// Inferred: this is the visual burst for a player coming back to life; the cube term makes bigger
// shadow points emit more particles.
// Unknown: the meaning of the numeric constants and of the LightEffect floats. Constants that are not
// short decimals are written as hexadecimal float literals: the EE compiler converts a long decimal
// literal with truncation and would land one unit below the observed float.
extern "C" {
extern signed char netNetworkMode;
extern signed char recallOn;
extern unsigned char player[];
struct Point3 { float x, y, z; };
void* blockAlloc__Fi(int bytes);
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(void* self, Point3 position, float a, float b, float c, float d, float e, float f,
                                                       float g, float h, float i, float j, float k, float l, void* object, int mode);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float i, float j, float k,
                                         int l, int m, int n, void (*callback)(void*));
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* model, void* state, Point3 position, short yaw,
                                                                                         int index, Point3* point, float* value, void* matrix,
                                                                                         float scale);
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
alignas(16) unsigned char resurectParticle[80];
int resurectParticleReady;
const float kInv = 1.0f / 65536.0f;
}

extern "C" void resurectEffect__6Player(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<short*>(object + 1188) = netNetworkMode != 0 ? 90 : 60;
    if (recallOn != 0) *reinterpret_cast<short*>(object + 1188) = 1;
    *reinterpret_cast<unsigned*>(object + 12) |= 2u;
    void* block = blockAlloc__Fi(188);
    if (block != nullptr) {
        const Point3* position = reinterpret_cast<const Point3*>(object + 16);
        const Point3 start = {position->x + 0.0f, position->y, position->z + 36.0f};
        __11LightEffectG6Point3ffffffffffffP10GameObjecti(block, start, 128.0f, 128.0f, 256.0f, -50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 240.0f, 120.0f,
                                                          0.0f, static_cast<float>(*reinterpret_cast<short*>(object + 1188)) * 0x1.111110p-6f,
                                                          nullptr, 0);
    }
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_cas_curelw.anm")),
        load<void*>(player, *reinterpret_cast<int*>(object + 960) * 23984 + 21872), 1.0f);
    if (resurectParticleReady == 0) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(resurectParticle, 70, 83, 74, 35, 31, 43, 75, 55, 0.0f, 0x1.399998p+4f, 0.0f, 3, 1, 8, nullptr);
        resurectParticleReady = 1;
    }
    void* model = load<void*>(object, 1332);
    const int points = modelGetNmShadowPoints__FP12_modelHeader(model);
    float cube = 0.0f;
    for (int i = 0; i < points; ++i) {
        Point3 where;
        float value;
        Point3 copy = *reinterpret_cast<Point3*>(object + 16);
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, object + 288, copy, *reinterpret_cast<short*>(object + 56), i,
                                                                                            &where, &value, nullptr, 1.0f);
        cube += value * value * value;
        while (cube > 0.0f) {
            const float scale = value * 0x1.4f8b58p-17f * 0x1.1accccp+5f;
            cube -= 0x1.2d6040p+36f;
            const float rx = static_cast<float>(eRand__10GameObject(self));
            const float ry = static_cast<float>(eRand__10GameObject(self));
            const float rz = static_cast<float>(eRand__10GameObject(self));
            Point3 jitter = {(rx * kInv - 0.5f) * scale, (ry * kInv - 0.5f) * scale, (rz * kInv - 0.5f) * scale};
            jitter.x += where.x;
            jitter.y += where.y;
            jitter.z += where.z;
            Point3 unit;
            do {
                unit.x = static_cast<float>(eRand__10GameObject(self)) * kInv;
                unit.x = unit.x + unit.x - 1.0f;
                unit.y = static_cast<float>(eRand__10GameObject(self)) * kInv;
                unit.y = unit.y + unit.y - 1.0f;
                unit.z = static_cast<float>(eRand__10GameObject(self)) * kInv;
                unit.z = unit.z + unit.z - 1.0f;
            } while (1.0f < unit.x * unit.x + unit.y * unit.y + unit.z * unit.z);
            Point3 velocity = {unit.x * 0x1.47b438p+5f, unit.y * 0x1.47b438p+5f, unit.z * 0x1.47b438p+5f};
            Point3 end = {jitter.x + velocity.x, jitter.y + velocity.y, jitter.z + velocity.z};
            Point3 delta = {end.x - jitter.x, end.y - jitter.y, end.z - jitter.z};
            Point3 faster = {delta.x * -0x1.1fa3f4p-2f, delta.y * -0x1.1fa3f4p-2f, delta.z * -0x1.1fa3f4p-2f};
            Point3 slower = {faster.x * 0.5f, faster.y * 0.5f, faster.z * 0.5f};
            const float lifetime = static_cast<float>(eRand__10GameObject(self)) * kInv * 0x1.999998p-2f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(resurectParticle, &end, &slower, lifetime);
        }
    }
}
