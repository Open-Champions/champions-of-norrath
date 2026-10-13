// Observed doUserYaw__6Camera at 0x0026e790, 1492 bytes. Nothing is returned.
// Nothing here was executed on the EE. Field meanings are not established, so
// offsets stay numeric. Notation: W/F/B/H are the word, float, signed byte and
// signed halfword at that camera offset; i = W(64); pad entry k is the 88-byte
// record k of `pad` (0x0034beb8); ent(k)[n] is its float at +n. Local state:
// hud (starts false), clamp (true), damp (false).
//
// Returns at once when levelFlags has bit 0x2000000, or F(156) != 0.0.
// x = |ent(i)[28]|, taken in doubles through fptodp, dpcmp, dpsub and then
// dptofp. fastestRotDirection__FssPi(from, to, &amount) returns a direction
// and stores an int amount; the player target below is the pointer at +21872 of
// the 23984-byte `player` entry, whose unsigned halfword at +56 gives
// to = (short)(16383 - that halfword).
//
// 1. W(56) != 0: amount comes from fastestRotDirection(H(104), H(138)). If it
//    is below 1000: W(60) = (int)(w + 0.5) when w >= 0.0 else (int)(w - 0.5)
//    with w = (float)W(60) * 0.85f, and W(56) = 0 when that W(60) is below 50
//    (signed). Else W(60) += 70 * W(56).
// 2. else W(100) != 0: the target belongs to player number
//    netGetLocalPlayerNm__Fi(0); clamp = false, hud = true; W(60) =
//    (short)(int)((float)dir * 70.0f * min(1.0f, (float)amount * 0.00025f))
//    with dir and amount from fastestRotDirection(H(104), to).
// 3. else if 0.01f < x: if localNmPlayersAlive >= 3 only hud = true. Otherwise,
//    when B(352) != 0 and 0.7f < F(212): W(60) = (short)(int)((float)W(60) *
//    0.95f), hud = true. Otherwise p = powf(|ent(i)[28]|, 0.95f) * 70.0f * 8.0f,
//    negated when ent(i)[28] < 0.0 and negated again when gameReverseCameraYaw
//    is nonzero; W(60) = (short)(int)(w + (p - w) * 0.125f), w = (float)W(60).
// 4. else (x not above 0.01f): damp = true when gameCombatCamera == 0, B(352)
//    != 0, W(84) != 0, F(144) != 1.0, gameControlPaused != 0 or
//    localNmPlayersAlive <= 0 (tested in that order). If none holds: clamp =
//    false; j = localPlayersAlive[0]; target from player j; dir and amount from
//    fastestRotDirection(H(104), to); q = powf(min(1.0f, (float)amount *
//    0.0000625f), 2.0f); s = (short)(int)((float)dir * 124.0f * q); r =
//    (short)(int)((float)s * sqrt(ent(i)[20]^2 + ent(i)[24]^2)); W(60) =
//    (short)(int)(w + ((float)r - w) * 0.1f) with w = (float)W(60).
// Then: when clamp, W(60) is limited to [-153, 153]. When damp and W(60) != 0:
// W(60) -= (short)(int)((float)W(60) * 0.25f), and W(60) = 0 if the new value
// is within [-19, 19]. When hud and 0.5f < x: fadeTimer[7*i] = 120 and
// fadeIconY[7*i] = hudGetFadeIconPos__Fii(W(64), 0). Finally H(104) += (short)
// (int)((float)W(60) * 0.9999999f) (the float with bits 0x3f7fffff).
typedef unsigned long long Double;

extern "C" {
Double fptodp(float value);
int dpcmp(Double a, Double b);
Double dpsub(Double a, Double b);
float dptofp(Double a);
int fastestRotDirection__FssPi(short from, short to, int* amount);
int netGetLocalPlayerNm__Fi(int code);
int hudGetFadeIconPos__Fii(int a, int b);
float yaw_powf(float base, float exponent) __asm__("powf");
extern unsigned levelFlags;
extern unsigned char pad[];
extern unsigned char player[];
extern int fadeTimer[];
extern int fadeIconY[];
extern int localPlayersAlive[];
extern int localNmPlayersAlive;
extern int gameCombatCamera;
extern int gameReverseCameraYaw;
extern int gameControlPaused;
}

typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;
typedef unsigned short __attribute__((may_alias)) AliasedUShort;
typedef float __attribute__((may_alias)) AliasedFloat;
typedef unsigned char* __attribute__((may_alias)) AliasedPtr;

#define WORD(off) (*reinterpret_cast<AliasedInt*>(camera + (off)))
#define FLOAT(off) (*reinterpret_cast<AliasedFloat*>(camera + (off)))
#define HALF(off) (*reinterpret_cast<AliasedShort*>(camera + (off)))
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

static inline float sqrt_s(float value) {
#if defined(__mips__)
    float out;
    __asm__("sqrt.s %0, %1" : "=f"(out) : "f"(value));
    return out;
#else
    return __builtin_sqrtf(value);
#endif
}

static inline short targetYaw(int playerNumber) {
    const unsigned char* target = *reinterpret_cast<AliasedPtr*>(player + 23984 * playerNumber + 21872);
    return static_cast<short>(16383 - *reinterpret_cast<const AliasedUShort*>(target + 56));
}

static inline int toShort(float value) {
    return static_cast<short>(static_cast<int>(value));
}

extern "C" void doUserYaw__6Camera(unsigned char* camera) {
    if (levelFlags & 0x2000000) return;
    if (FLOAT(156) != 0.0f) return;
    const int i = WORD(64);
    const unsigned char* entry = pad + 88 * i;
    Double ax = fptodp(*reinterpret_cast<const AliasedFloat*>(entry + 28));
    if (dpcmp(ax, 0) < 0) ax = dpsub(0, ax);
    const float x = dptofp(ax);
    bool hud = false, clamp = true, damp = false;

    if (WORD(56) != 0) {
        int amount;
        fastestRotDirection__FssPi(HALF(104), HALF(138), &amount);
        if (amount < 1000) {
            const float w = static_cast<float>(WORD(60)) * 0x1.b33332p-1f /* bits 0x3f599999 */;
            const int rounded = static_cast<int>(0.0f <= w ? w + 0.5f : w - 0.5f);
            WORD(60) = rounded;
            if (rounded < 50) WORD(56) = 0;
        } else {
            WORD(60) = WORD(60) + 70 * WORD(56);
        }
    } else if (WORD(100) != 0) {
        const short from = HALF(104);
        const int number = netGetLocalPlayerNm__Fi(0);
        clamp = false;
        hud = true;
        int amount;
        const int dir = fastestRotDirection__FssPi(from, targetYaw(number), &amount);
        const float scale = fp_min(1.0f, static_cast<float>(amount) * 0x1.0624dcp-12f /* bits 0x3983126e */);
        WORD(60) = toShort(static_cast<float>(dir) * 70.0f * scale);
    } else if (0x1.47ae14p-7f /* bits 0x3c23d70a */ < x) {
        if (localNmPlayersAlive >= 3) {
            hud = true;
        } else if (BYTE(352) != 0 && 0x1.666666p-1f /* bits 0x3f333333 */ < FLOAT(212)) {
            WORD(60) = toShort(static_cast<float>(WORD(60)) * 0x1.e66666p-1f /* bits 0x3f733333 */);
            hud = true;
        } else {
            const float raw = *reinterpret_cast<const AliasedFloat*>(entry + 28);
            const float magnitude = yaw_powf(__builtin_fabsf(raw), 0x1.e66666p-1f /* bits 0x3f733333 */);
            float p = magnitude * 70.0f * 8.0f;
            if (*reinterpret_cast<const AliasedFloat*>(pad + 88 * WORD(64) + 28) < 0.0f) p = -p;
            if (gameReverseCameraYaw != 0) p = -p;
            const float w = static_cast<float>(WORD(60));
            WORD(60) = toShort(w + (p - w) * 0.125f);
        }
    } else if (gameCombatCamera == 0 || BYTE(352) != 0 || WORD(84) != 0 || FLOAT(144) != 1.0f ||
               gameControlPaused != 0 || localNmPlayersAlive <= 0) {
        damp = true;
    } else {
        const int j = localPlayersAlive[0];
        clamp = false;
        int amount;
        const int dir = fastestRotDirection__FssPi(HALF(104), targetYaw(j), &amount);
        const float q = yaw_powf(fp_min(1.0f, static_cast<float>(amount) * 0x1.0624dcp-14f /* bits 0x3883126e */), 2.0f);
        const float w = static_cast<float>(WORD(60));
        const int s = toShort(static_cast<float>(dir) * 124.0f * q);
        const float ex = *reinterpret_cast<const AliasedFloat*>(pad + 88 * WORD(64) + 20);
        const float ey = *reinterpret_cast<const AliasedFloat*>(pad + 88 * WORD(64) + 24);
        const float len = sqrt_s(ex * ex + ey * ey);
        const int r = toShort(static_cast<float>(s) * len);
        WORD(60) = toShort(w + (static_cast<float>(r) - w) * 0x1.999998p-4f /* bits 0x3dcccccc */);
    }

    if (clamp) {
        int v = WORD(60);
        v = v < -153 ? -153 : v;
        v = v < 153 ? v : 153;
        WORD(60) = v;
    }
    if (damp && WORD(60) != 0) {
        const int v = WORD(60);
        const int r = v - toShort(static_cast<float>(v) * 0.25f);
        WORD(60) = r;
        if (static_cast<unsigned>(r + 19) < 39u) WORD(60) = 0;
    }
    if (hud && 0.5f < x) {
        fadeTimer[7 * WORD(64)] = 120;
        fadeIconY[7 * WORD(64)] = hudGetFadeIconPos__Fii(WORD(64), 0);
    }
    HALF(104) = static_cast<short>(HALF(104) + toShort(static_cast<float>(WORD(60)) * 0x1.fffffep-1f /* bits 0x3f7fffff */));
}
