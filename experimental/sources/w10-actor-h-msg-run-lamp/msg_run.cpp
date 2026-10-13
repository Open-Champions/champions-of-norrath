// Observed msg_run__4Lamp at 0x00230b20, 548 bytes.
// Per-frame behaviour of a lamp, in the order of the image (o is the object, p its pointer word):
//  - The float ef1Noise__Fi(0x007efe41 + (p * 0x19660d + (frameNm << 4))) is multiplied by 22.0,
//    converted to an integer (the compiler's float-to-int conversion) and added to 216; the sum,
//    converted back to float, is s. The light member at o+76 gets s at its offsets 28 and 36 (o+104
//    and o+112) and 1.0 / (s * s) at its offsets 32 and 40 (o+108 and o+116).
//  - Twice, a direction is drawn by rejection: each component is (float)eRand(o) * 2^-16, doubled,
//    minus 1.0, drawn in x, y, z order, and the draw repeats while 1.0 < (x*x + y*y) + z*z. The
//    direction is normalised with rsqrt.s (numerator 1.0) of that same sum and scaled by the float
//    constant 0x403624dd, and P_AddParticle__FP11ParticleDefP6Point3T1f runs with lampParticle, the
//    object position at o+16, that direction and the float 0.0.
//  - Then, when the signed byte at o+137 is 31 or more, the object is removed from the run list with
//    objectRemoveFromRunList__FP10GameObject and the byte at o+136 is cleared. In every case the
//    byte at o+137 becomes its value plus one (a byte wrap).
// Inferred: o+136 and o+137 are a queued flag and a frame counter (msg_draw sets the flag), and the
// member at o+76 is a light (clone copies it and calls lightAdd on it).
// Unknown: the meaning of the light's float offsets and of the constants. Nothing here ran on the EE.
extern "C" {
float ef1Noise__Fi(int index);
int eRand__10GameObject(void* object);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float extra);
void objectRemoveFromRunList__FP10GameObject(void* object);
extern int frameNm;
extern unsigned char lampParticle[128];
}

namespace {
float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}

float random_component(void* object) {
    float value = static_cast<float>(eRand__10GameObject(object)) * 0x1.0p-16f;
    value = value + value;
    return value - 1.0f;
}
}  // namespace

extern "C" void msg_run__4Lamp(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned low = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(self));
    float noise = ef1Noise__Fi(static_cast<int>(0x007efe41u + low * 0x19660du + (static_cast<unsigned>(frameNm) << 4)));
    const float size = static_cast<float>(static_cast<int>(noise * 22.0f) + 216);
    unsigned char* light = object + 76;
    *reinterpret_cast<float*>(light + 28) = size;
    *reinterpret_cast<float*>(light + 36) = size;
    const float inverse = 1.0f / (size * size);
    *reinterpret_cast<float*>(light + 40) = inverse;
    *reinterpret_cast<float*>(light + 32) = inverse;
    for (int i = 0; i < 2; ++i) {
        float v[3];
        float sum;
        do {
            v[0] = random_component(self);
            v[1] = random_component(self);
            v[2] = random_component(self);
            sum = v[0] * v[0] + v[1] * v[1];
            sum = sum + v[2] * v[2];
        } while (1.0f < sum);
        float velocity[3] = {v[0], v[1], v[2]};
        const float length_squared = (velocity[0] * velocity[0] + velocity[1] * velocity[1]) + velocity[2] * velocity[2];
        const float scale = reciprocal_sqrt(length_squared);
        const float speed = 0x1.6c49bap+1f;
        velocity[2] = velocity[2] * scale * speed;
        velocity[0] = velocity[0] * scale * speed;
        velocity[1] = velocity[1] * scale * speed;
        P_AddParticle__FP11ParticleDefP6Point3T1f(lampParticle, object + 16, velocity, 0.0f);
    }
    if (static_cast<signed char>(object[137]) >= 31) {
        objectRemoveFromRunList__FP10GameObject(self);
        object[136] = 0;
    }
    object[137] = static_cast<unsigned char>(object[137] + 1);
}
