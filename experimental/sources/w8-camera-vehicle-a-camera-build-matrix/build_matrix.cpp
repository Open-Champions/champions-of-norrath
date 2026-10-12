// Observed BuildMatrix__6CamerafffP6Point3 at 0x0026c190, 828 bytes.
// Arguments: camera, three floats (a point P) and a pointer T to a point that
// may be null. Builds a 12-float matrix M (nine floats at +0..+32 and three
// more), perturbs P with a random shake, and passes P and M to
// worldSetViewMatricies__FP6Point3P8Matrix34. Nothing is returned. Nothing here
// was executed on the EE; rsqrt.s is the observed normalization instruction.
//
// Null T: matIdent__FP8Matrix34, then matRot__FP8Matrix34is on axis 0 with the
// short of (unsigned halfword at camera+106) + 16339, then on axis 2 with the
// signed halfword at camera+104.
// Non-null T: B = (T.y, T.z*u - T.x, -(T.y*u)) with u = 0.0f (the observed code
// multiplies by a stored zero and omits the other products), scaled by
// 1/sqrt(B.B) through rsqrt.s with numerator 1. C = T x B using the scaled B,
// that is (T.y*B.z - T.z*B.y, T.z*B.x - T.x*B.z, T.x*B.y - T.y*B.x), scaled the
// same way. T is not normalized. Then matIdent and stores of the columns: the
// words at +0, +12, +24 are B.x, B.y, B.z; +4, +16, +28 are C.x, C.y, C.z; and
// +8, +20, +32 are T.x, T.y, T.z.
// Shake: when gameCameraShake (0x00484f64, float) is above 0 and either
// gameIsPaused__Fv is zero or DLG_Active__Fv is nonzero (the second call is made
// only when the first is nonzero), s is read, the global becomes s - 0.5, and
// eRandState (0x00483e58) advances three times by seed*0x19660d + 0x3c6ef35f
// using 32-bit wraparound. For each step in order, the high 16 bits as a float
// times 1/65536, minus 0.5, doubled and multiplied by s, are added to P.x, P.y
// and P.z respectively.
extern "C" {
void matIdent__FP8Matrix34(float* matrix);
void matRot__FP8Matrix34is(float* matrix, int axis, short angle);
void worldSetViewMatricies__FP6Point3P8Matrix34(float* point, float* matrix);
int gameIsPaused__Fv();
int DLG_Active__Fv();
extern float gameCameraShake;
extern unsigned eRandState;
}

#if !defined(__mips__)
extern "C" float sqrtf(float);
#endif

namespace {
inline float reciprocal_length(float length_squared) {
#if defined(__mips__)
    float result;
    const float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / sqrtf(length_squared);
#endif
}

typedef short __attribute__((may_alias)) AliasedShort;

inline float shake_offset(unsigned seed, float s) {
    return (static_cast<float>(static_cast<int>(seed >> 16)) * 0.0000152587890625f - 0.5f) * 2.0f * s;
}
}

extern "C" void BuildMatrix__6CamerafffP6Point3(unsigned char* camera, float x, float y, float z, const float* target) {
    float matrix[12];
    float point[3] = {x, y, z};
    if (target == nullptr) {
        matIdent__FP8Matrix34(matrix);
        const unsigned pitch = static_cast<unsigned short>(*reinterpret_cast<AliasedShort*>(camera + 106));
        matRot__FP8Matrix34is(matrix, 0, static_cast<short>(pitch + 16339u));
        matRot__FP8Matrix34is(matrix, 2, *reinterpret_cast<AliasedShort*>(camera + 104));
    } else {
        const float tx = target[0], ty = target[1], tz = target[2];
        const float up_x = 0.0f;
        float b[3] = {ty, tz * up_x - tx, -(ty * up_x)};
        float inverse = reciprocal_length(b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);
        b[0] *= inverse;
        b[1] *= inverse;
        b[2] *= inverse;
        float c[3] = {ty * b[2] - tz * b[1], tz * b[0] - tx * b[2], tx * b[1] - ty * b[0]};
        inverse = reciprocal_length(c[0] * c[0] + c[1] * c[1] + c[2] * c[2]);
        c[0] *= inverse;
        c[1] *= inverse;
        c[2] *= inverse;
        matIdent__FP8Matrix34(matrix);
        matrix[0] = b[0];
        matrix[3] = b[1];
        matrix[6] = b[2];
        matrix[1] = c[0];
        matrix[4] = c[1];
        matrix[7] = c[2];
        matrix[2] = tx;
        matrix[5] = ty;
        matrix[8] = tz;
    }
    if (gameCameraShake > 0.0f && (gameIsPaused__Fv() == 0 || DLG_Active__Fv() != 0)) {
        const float s = gameCameraShake;
        gameCameraShake = s - 0.5f;
        unsigned seed = eRandState * 0x19660du + 0x3c6ef35fu;
        const unsigned first = seed;
        seed = seed * 0x19660du + 0x3c6ef35fu;
        const unsigned second = seed;
        seed = seed * 0x19660du + 0x3c6ef35fu;
        eRandState = seed;
        point[0] += shake_offset(first, s);
        point[1] += shake_offset(second, s);
        point[2] += shake_offset(seed, s);
    }
    worldSetViewMatricies__FP6Point3P8Matrix34(point, matrix);
}
