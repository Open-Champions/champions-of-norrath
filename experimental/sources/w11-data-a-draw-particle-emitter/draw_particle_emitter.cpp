// drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25 at 0x00186b58,
// 1808 bytes, read from the pinned SLUS-20565 image. Arguments: the origin (passed by
// reference), the particle definition, the emitter data, the frame number, an angle, and
// three booleans (named flipA, flipB and swapXY here after what they do).
// Observed, with d the emitter data and offsets in bytes:
//  - the signed byte d[17] gives the burst size: positive means that many particles per call,
//    zero means none, negative means one particle on frames that are a multiple of its
//    magnitude and none otherwise (the call then ends);
//  - when the word d[56] is below the frame, a new d[56] is chosen with eRandState. The base is
//    d[44] when the flag byte d[60] is zero and d[48] otherwise, and it is nonzero in that case:
//    d[56] = int(float(frame + base) + (r * 2^-16 - 0.5) * float(base)), r being the high half
//    of the next random word. A zero d[44] skips the update. The flag byte is then set to 1
//    when it was zero, and to 0 otherwise, so the two intervals alternate;
//  - nothing is emitted unless the flag byte is nonzero and the burst size is positive;
//  - each particle draws three random words for a position offset: each axis is
//    (r * 2^-16 - 0.5) times the unsigned halfwords d[34], d[36], d[38]. The offset is turned by
//    z (angle + halfword d[30]) and x (d[32]), then the signed halfwords d[18], d[20] mix with
//    cos and sin of the angle into x and y and d[22] is added to z. flipA negates x, flipB negates
//    y, swapXY exchanges them. The origin is added;
//  - the velocity starts as the halfwords d[24], d[26], d[28]. One more random word scales the
//    unit direction of that vector by (r * 2^-16 - 0.5) * d[40], and the result is added to the
//    velocity. Two more random words give a z turn of the high half as a signed 16-bit value
//    and an x turn of (r * 2^-16) * d[42]; both turn the velocity. Then bit 0 of d[52] set turns
//    it by 0x4000 about x, else bit 1 set by z d[30] and x d[32]; otherwise it is turned by the
//    position matrix and takes the same flipA, flipB and swapXY changes. The first two forms
//    skip those flips;
//  - P_AddParticle is called with the definition, position, velocity and 0.0f.
// The meaning of the named fields is inferred from use; none is a confirmed name.
// Fresh compilation is not an original-byte match.
extern "C" {
extern int eRandState;
void matIdent__FP8Matrix34(void* matrix);
void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
void matTransform__FP6Point3P8Matrix34T0(void* in, void* matrix, void* out);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float extra);
}

struct Point3 {
    float x, y, z;
};

namespace {
template <typename T>
T& at(void* base, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(base) + offset);
}

__attribute__((always_inline)) inline float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}

unsigned nextRandom() {
    unsigned state = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
    eRandState = static_cast<int>(state);
    return state;
}

float unitRandom() {
    return static_cast<float>(nextRandom() >> 16) * 0.0000152587890625f;
}

float scheduledTime(int frame, int base) {
    return static_cast<float>(frame + base) + (unitRandom() - 0.5f) * static_cast<float>(base);
}
}

extern "C" void drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(
    Point3 origin, void* definition, void* data, int frame, short angle, bool flipA, bool flipB, bool swapXY) {
    int count = 1;
    const int burst = at<signed char>(data, 17);
    if (burst > 0) {
        count = burst;
    } else if (burst == 0) {
        count = 0;
    } else {
        const int period = -burst;
        if (frame % period != 0) return;
    }
    if (at<int>(data, 56) < frame) {
        const int shortBase = at<int>(data, 44);
        if (shortBase == 0) {
            at<signed char>(data, 60) = 1;
        } else {
            const int base = at<signed char>(data, 60) != 0 ? at<int>(data, 48) : shortBase;
            at<int>(data, 56) = static_cast<int>(scheduledTime(frame, base));
            at<signed char>(data, 60) = at<signed char>(data, 60) == 0;
        }
    }
    if (at<signed char>(data, 60) == 0) return;
    if (count <= 0) return;
    for (; count != 0; --count) {
        float matrix[12];
        float matrixB[12];
        float matrixC[12];
        float matrixD[12];
        Point3 position;
        Point3 velocity;
        Point3 direction;
        Point3 delta;
        Point3 scaled;
        position.x = (unitRandom() - 0.5f) * static_cast<float>(at<unsigned short>(data, 34));
        position.y = (unitRandom() - 0.5f) * static_cast<float>(at<unsigned short>(data, 36));
        position.z = (unitRandom() - 0.5f) * static_cast<float>(at<unsigned short>(data, 38));
        matIdent__FP8Matrix34(matrix);
        matRot__FP8Matrix34is(matrix, 2, static_cast<short>(angle + at<unsigned short>(data, 30)));
        matRot__FP8Matrix34is(matrix, 1, at<short>(data, 32));
        matTransform__FP6Point3P8Matrix34T0(&position, matrix, &position);
        const float cosine = icos__Fi(angle);
        const float sine = isin__Fi(angle);
        position.x += static_cast<float>(at<short>(data, 18)) * cosine + static_cast<float>(at<short>(data, 20)) * sine;
        const float sine2 = isin__Fi(angle);
        const float cosine2 = icos__Fi(angle);
        position.z += static_cast<float>(at<short>(data, 22));
        position.y += static_cast<float>(at<short>(data, 18)) * -sine2 + static_cast<float>(at<short>(data, 20)) * cosine2;
        if (flipA) position.x = -position.x;
        if (flipB) position.y = -position.y;
        if (swapXY) {
            const float x = position.x;
            position.x = position.y;
            position.y = x;
        }
        position.x += origin.x;
        position.y += origin.y;
        position.z += origin.z;
        velocity.x = static_cast<float>(at<short>(data, 24));
        velocity.y = static_cast<float>(at<short>(data, 26));
        velocity.z = static_cast<float>(at<short>(data, 28));
        const float spread = unitRandom() - 0.5f;
        direction = velocity;
        const float inverse = reciprocal_sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
        direction.x *= inverse;
        direction.y *= inverse;
        direction.z *= inverse;
        const float speed = static_cast<float>(at<short>(data, 40));
        scaled.x = direction.x * spread;
        scaled.y = direction.y * spread;
        scaled.z = direction.z * spread;
        delta.x = scaled.x * speed;
        delta.y = scaled.y * speed;
        delta.z = scaled.z * speed;
        velocity.x += delta.x;
        velocity.y += delta.y;
        velocity.z += delta.z;
        matIdent__FP8Matrix34(matrixB);
        matRot__FP8Matrix34is(matrixB, 2, static_cast<short>(static_cast<int>(nextRandom()) >> 16));
        matRot__FP8Matrix34is(matrixB, 1, static_cast<short>(static_cast<int>(unitRandom() * static_cast<float>(at<short>(data, 42)))));
        matTransform__FP6Point3P8Matrix34T0(&velocity, matrixB, &velocity);
        const unsigned char flags = at<unsigned char>(data, 52);
        if (flags & 1) {
            matIdent__FP8Matrix34(matrixC);
            matRot__FP8Matrix34is(matrixC, 1, 0x4000);
            matTransform__FP6Point3P8Matrix34T0(&velocity, matrixC, &velocity);
        } else if (flags & 2) {
            matIdent__FP8Matrix34(matrixD);
            matRot__FP8Matrix34is(matrixD, 2, at<short>(data, 30));
            matRot__FP8Matrix34is(matrixD, 1, at<short>(data, 32));
            matTransform__FP6Point3P8Matrix34T0(&velocity, matrixD, &velocity);
        } else {
            matTransform__FP6Point3P8Matrix34T0(&velocity, matrix, &velocity);
            if (flipA) velocity.x = -velocity.x;
            if (flipB) velocity.y = -velocity.y;
            if (swapXY) {
                const float x = velocity.x;
                velocity.x = velocity.y;
                velocity.y = x;
            }
        }
        P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &position, &velocity, 0.0f);
    }
}
