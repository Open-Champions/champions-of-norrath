// Observed doAutoPitch__6Camera at 0x0026daa8, 2020 bytes. Nothing is returned.
// Nothing here was executed on the EE. Field meanings are not established, so
// offsets stay numeric. Notation: W/F/H/UH/B are the word, float, signed
// halfword, unsigned halfword and signed byte at that camera offset; list is a
// stack array of player numbers and count its length.
//
// List. When netNetworkMode != 0 and nmPlayersAlive != 0 and
// localNmPlayersAlive == 0, the list is the single entry B(353), and the
// function returns at once if that is -1. Otherwise the list is the first
// localNmPlayersAlive entries of localPlayersAlive, and the function returns at
// once if there are none. (The original keeps the list in four stack words
// followed directly by other locals; more than four entries are not meant.)
//
// Sweep. lim = 12000.0 when B(352) != 0, else F(216) * 6000.0 + 4000.0. One
// pass is repeated while (W(72) != 0 or B(365) != 0) and the counter W(76), which
// is incremented after every pass, is below 4 * count; with neither set exactly
// one pass runs. A pass with t = W(76): player = list[(t % (4 * count)) / 4],
// sub = t % 4 (C remainders). The player's `player` entry (23984 bytes) holds a
// pointer at +21872; the function returns if it is null. pos is the Point3 at
// +16 of that target. (px, py) is pos.x, pos.y when B(352) != 0 and F(24), F(28)
// otherwise. d = (worldCameraPos.x - px, worldCameraPos.y - py), scaled by
// rsqrt.s(1.0, dx*dx + dy*dy). angle = iatan2__Fff(dx, dy) of the scaled
// vector. base = (short)(angle + (int)(lim * 0.5) - (int)((float)sub * (lim *
// 0.25))). The ray origin is pos with 40.0 added to z. For k = 1..5: a matrix
// is set to identity, rotated about axis 0 by (short)(UH(126 + 2k) + 16339) and
// then about axis 2 by base; the vector (0, -1, 0) is taken through
// matUntransform__FP6Point3P8Matrix33T0 to give a direction;
// worldHitscan__FP12_worldHeaderG6Point3T1fi(world, origin, direction, F(180),
// 0) is called, and byte camera[256 + 24 * player + 6 * sub + k] becomes 1 when
// the result is above 0.0, else 0. Afterwards W(76) = W(76) + 1.
//
// Hit index. first = 6, lowered to the smallest k in 1..5 whose byte
// camera[256 + 24 * p + 6 * s + k] is nonzero, over every player p in the list
// and s in 0..3.
//
// Follow. If F(156) != 0.0: F(152) = 0.0 and H(106) = (short)(int)((float)H(116)
// + (float)(m - H(116)) * F(144)) with m the larger of H(114) and H(108 + 2 *
// W(84)); then it returns. Otherwise idx = max(first - (B(352) != 0 ? 2 : 1), 0),
// H(114) = (short)(16383 - UH(126 + 2 * idx)), m the larger of H(114) and
// H(108 + 2 * W(84)), s1 = (short) of m rounded half away from zero, and s3 =
// H(106). With W(72) != 0: F(152) = 0.0 and s3 = s1. Otherwise F(152) is
// adjusted by the gap s1 - s3 (nothing when equal): with B(352) != 0 the step
// is (s1 - s3) * 0.001 * 0.9999999 added in the original's soft doubles when s1
// is above s3 (fptodp, litodp, dpmul by the doubles 0.001 and 0x3fefffffe0000000,
// dpadd, dptofp) and in floats (same constants) when below, then F(152) =
// max(-15.0, min(35.0, F(152) * 0.95 * (1.0 - F(212)^3))). With B(352) == 0
// and s1 above s3 it adds (s1 - s3) * 0.9999999 * ((F(216) * 0.008 + 0.002) *
// max(0, min(1, s1 * 6.25e-5))); below, (s1 - s3) * 0.001 * 0.9999999; then
// F(152) = max(F(232 + 4 * W(84)), min(F(220 + 4 * W(84)), F(152) * 0.92)).
// Finally, if F(152) > 0.0 it is replaced by min((16383 - s3) * 0.05, F(152)).
// s3 = (short)(s3 + (int)(F(152) * 0.9999999)); H(116) = s3; H(106) = s3.
typedef unsigned long long Double;
struct Matrix33 { float m[9]; };

extern "C" {
Double fptodp(float value);
Double litodp(int value);
Double dpmul(Double a, Double b);
Double dpadd(Double a, Double b);
float dptofp(Double a);
int iatan2__Fff(float first, float second);
void matIdent__FP8Matrix33(Matrix33* matrix);
void matRot__FP8Matrix33is(Matrix33* matrix, int axis, short angle);
void* matUntransform__FP6Point3P8Matrix33T0(float* out, Matrix33* matrix, float* in);
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* world, float* origin, float* direction, float range, int mode);
extern signed char netNetworkMode;
extern int nmPlayersAlive;
extern int localNmPlayersAlive;
extern int localPlayersAlive[];
extern void* world;
extern float worldCameraPos[];
extern unsigned char player[];
}

typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;
typedef unsigned short __attribute__((may_alias)) AliasedUShort;
typedef float __attribute__((may_alias)) AliasedFloat;
typedef unsigned char* __attribute__((may_alias)) AliasedPtr;

#define WORD(off) (*reinterpret_cast<AliasedInt*>(camera + (off)))
#define FLOAT(off) (*reinterpret_cast<AliasedFloat*>(camera + (off)))
#define HALF(off) (*reinterpret_cast<AliasedShort*>(camera + (off)))
#define UHALF(off) (*reinterpret_cast<AliasedUShort*>(camera + (off)))
#define BYTE(off) (*reinterpret_cast<signed char*>(camera + (off)))

static inline float fp_min(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return a < b ? a : b;
#endif
}

static inline float fp_max(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("max.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return a > b ? a : b;
#endif
}

static inline float rsqrt_s(float numerator, float value) {
#if defined(__mips__)
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(numerator), "f"(value));
    return out;
#else
    return numerator / __builtin_sqrtf(value);
#endif
}

extern "C" void doAutoPitch__6Camera(unsigned char* camera) {
    int list[4];
    int count;
    if (netNetworkMode != 0 && nmPlayersAlive != 0) {
        const int n = localNmPlayersAlive;
        if (n == 0) {
            const int only = BYTE(353);
            if (only == -1) return;
            list[0] = only;
            count = 1;
        } else {
            count = n;
            for (int i = 0; i < n; ++i) list[i] = localPlayersAlive[i];
        }
    } else {
        const int n = localNmPlayersAlive;
        if (n == 0) return;
        count = n;
        for (int i = 0; i < n; ++i) list[i] = localPlayersAlive[i];
    }

    const float lim = BYTE(352) != 0 ? 12000.0f : FLOAT(216) * 6000.0f + 4000.0f;
    const int period = count * 4;
    do {
        const int t = WORD(76);
        const int slot = (t % period) / 4;
        const int sub = t % 4;
        const int who = list[slot];
        const unsigned char* target = *reinterpret_cast<AliasedPtr*>(player + 23984 * who + 21872);
        if (target == nullptr) return;
        float pos[3];
        pos[0] = reinterpret_cast<const AliasedFloat*>(target + 16)[0];
        pos[1] = reinterpret_cast<const AliasedFloat*>(target + 16)[1];
        pos[2] = reinterpret_cast<const AliasedFloat*>(target + 16)[2];
        float px, py;
        if (BYTE(352) != 0) {
            px = pos[0];
            py = pos[1];
        } else {
            px = FLOAT(24);
            py = FLOAT(28);
        }
        float dx = worldCameraPos[0] - px;
        float dy = worldCameraPos[1] - py;
        const float inverse = rsqrt_s(1.0f, dx * dx + dy * dy);
        dx = dx * inverse;
        dy = dy * inverse;
        float down[3] = {0.0f, -1.0f, 0.0f};
        float origin[3] = {pos[0], pos[1], pos[2] + 40.0f};
        const int angle = iatan2__Fff(dx, dy);
        const int base = static_cast<short>(angle + static_cast<int>(lim * 0.5f) -
                                            static_cast<int>(static_cast<float>(sub) * (lim * 0.25f)));
        Matrix33 matrix;
        float direction[3];
        for (int k = 1; k < 6; ++k) {
            matIdent__FP8Matrix33(&matrix);
            matRot__FP8Matrix33is(&matrix, 0, static_cast<short>(UHALF(126 + 2 * k) + 16339));
            matRot__FP8Matrix33is(&matrix, 2, static_cast<short>(base));
            matUntransform__FP6Point3P8Matrix33T0(direction, &matrix, down);
            const float hit = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, origin, direction, FLOAT(180), 0);
            camera[256 + 24 * who + 6 * sub + k] = 0.0f < hit ? 1 : 0;
        }
        WORD(76) = WORD(76) + 1;
    } while ((WORD(72) != 0 || BYTE(365) != 0) && WORD(76) < period);

    int first = 6;
    for (int i = 0; i < count; ++i) {
        for (int s = 0; s < 4; ++s) {
            for (int k = 5; k > 0; --k) {
                if (camera[256 + 24 * list[i] + 6 * s + k] != 0 && k < first) first = k;
            }
        }
    }

    if (FLOAT(156) != 0.0f) {
        const int a = HALF(114);
        const int b = HALF(108 + 2 * WORD(84));
        const int m = b < a ? a : b;
        FLOAT(152) = 0.0f;
        HALF(106) = static_cast<short>(static_cast<int>(
            static_cast<float>(HALF(116)) + static_cast<float>(m - HALF(116)) * FLOAT(144)));
        return;
    }

    const bool close = BYTE(352) != 0;
    const int step = first - (close ? 2 : 1);
    const int idx = step < 0 ? 0 : step;
    HALF(114) = static_cast<short>(16383 - UHALF(126 + 2 * idx));
    const int a = HALF(114);
    const int b = HALF(108 + 2 * WORD(84));
    const float wanted = static_cast<float>(b < a ? a : b);
    const int s1 = static_cast<short>(0.0f <= wanted ? static_cast<int>(wanted + 0.5f) : static_cast<int>(wanted - 0.5f));
    int s3 = HALF(106);
    if (WORD(72) != 0) {
        FLOAT(152) = 0.0f;
        s3 = s1;
    } else {
        if (close) {
            if (s3 < s1) {
                const Double sum = dpadd(fptodp(FLOAT(152)),
                                         dpmul(dpmul(litodp(s1 - s3), 0x3f50624dd2f1a9fbULL), 0x3fefffffe0000000ULL));
                FLOAT(152) = dptofp(sum);
            } else if (s1 < s3) {
                FLOAT(152) = FLOAT(152) + static_cast<float>(s1 - s3) * 0x1.0624dcp-10f * 0x1.fffffep-1f;
            }
            const float e = FLOAT(212);
            const float shaped = FLOAT(152) * 0x1.e66666p-1f * (1.0f - e * e * e);
            FLOAT(152) = fp_max(-15.0f, fp_min(35.0f, shaped));
        } else {
            if (s3 < s1) {
                const float k = fp_max(0.0f, fp_min(1.0f, static_cast<float>(s1) * 0x1.0624dcp-14f));
                const float rate = (FLOAT(216) * 0x1.0624dcp-7f + 0x1.0624dcp-9f) * k;
                FLOAT(152) = FLOAT(152) + static_cast<float>(s1 - s3) * 0x1.fffffep-1f * rate;
            } else if (s1 < s3) {
                FLOAT(152) = FLOAT(152) + static_cast<float>(s1 - s3) * 0x1.0624dcp-10f * 0x1.fffffep-1f;
            }
            const float eased = FLOAT(152) * 0x1.d70a3cp-1f;
            FLOAT(152) = eased;
            const float upper = fp_min(FLOAT(220 + 4 * WORD(84)), eased);
            FLOAT(152) = upper;
            FLOAT(152) = fp_max(FLOAT(232 + 4 * WORD(84)), upper);
        }
    }

    float speed = FLOAT(152);
    if (0.0f < speed) {
        speed = fp_min(static_cast<float>(16383 - s3) * 0x1.999998p-5f, speed);
        FLOAT(152) = speed;
    }
    s3 = static_cast<short>(s3 + static_cast<int>(speed * 0x1.fffffep-1f));
    HALF(116) = static_cast<short>(s3);
    HALF(106) = static_cast<short>(s3);
}
