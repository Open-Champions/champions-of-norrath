// Observed doUserZoom__6Camera at 0x0026e290, 1280 bytes. Nothing is returned.
// Nothing here was executed on the EE. Field meanings are not established, so
// offsets stay numeric. Notation: W/F/B are the word, float and signed byte at
// that camera offset; i = W(64); Y = the float at pad entry i + 32 (the 88-byte
// records of `pad`, table at 0x0034beb8); pressed = |Y| > 0.9f, taken in
// doubles through the soft routines fptodp, dpcmp, dpsub (the limit is the
// double 0x3fecccccc0000000).
//
// BLEND(m), used three ways below, with a = W(84), b = W(88), lo = F(176), hi =
// F(184), P(k) = F(176+4k) and Q(k) = F(188+4k):
//   z = P(b) + (P(a) - P(b)) * m;  F(140) = z;  F(216) = 1.0 - (z - lo) / (hi - lo);
//   F(252) = Q(b) + (Q(a) - Q(b)) * min(1.0, m)    (min is the hardware min.s).
// HUD(): fadeTimer[7*i] = 120; fadeIconY[7*i] = hudGetFadeIconPos__Fii(W(64), 0).
//
// If W(80) != 0 nothing else happens but: HUD() when pressed.
// Else if B(352) != 0: unless gameControlPaused != 0 with B(365) == 0 and W(72) == 0
// (which returns at once, W(92) untouched), W(84) = 1, W(88) = 2, T = 1.0 -
// min(1.0, 2 * max(0.0, F(212) - 0.1f)); F(144) = T when W(72) != 0 or
// B(365) != 0, otherwise F(144) += (T - F(144)) * 0.05f; F(148) = F(144);
// BLEND(F(144)); HUD() when pressed.
// Else if F(156) != 0.0: when F(144) == 0.0 engineSetBlur__Fi(100); F(160) +=
// F(156); r = evalHermite__Ffffff(F(160), 0.0, 1.0, 0.5, 0.5); F(148) = F(144)
// = r; when clampZoomMag__6Camera(camera) is nonzero F(156) = 0.0; BLEND(F(144)).
// Else, only if W(96) != 0 and (the word at pad entry i + 8) & automapButton[i]
// is zero: when F(200) != 0.0, m = F(148) + (1.0 - F(148)) * (1.0 - F(200)),
// F(144) = m, BLEND(m). Then, when pressed and W(92) != 0, with a = W(84):
// if a < 2 and Y > 0.9f: F(160) = 0, F(156) = 0.04f, W(84) = a + 1, F(144) = 0,
// W(88) = a; else if a != 0 and Y < -0.9f: W(84) = a - 1, F(156) = 0.04f, F(144)
// = 0, F(160) = 0, W(88) = a; else HUD().
// Every path that does not return early ends with W(92) = (pressed ? 0 : 1).
typedef unsigned long long Double;

extern "C" {
Double fptodp(float value);
int dpcmp(Double a, Double b);
Double dpsub(Double a, Double b);
float evalHermite__Ffffff(float t, float a, float b, float c, float d);
void engineSetBlur__Fi(int value);
int clampZoomMag__6Camera(void* camera);
int hudGetFadeIconPos__Fii(int a, int b);
extern unsigned char pad[];
extern int fadeTimer[];
extern int fadeIconY[];
extern unsigned automapButton[];
extern int gameControlPaused;
}

typedef int __attribute__((may_alias)) AliasedInt;
typedef float __attribute__((may_alias)) AliasedFloat;

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

#define WORD(off) (*reinterpret_cast<AliasedInt*>(camera + (off)))
#define FLOAT(off) (*reinterpret_cast<AliasedFloat*>(camera + (off)))
#define BYTE(off) (*reinterpret_cast<signed char*>(camera + (off)))

static inline __attribute__((always_inline)) void blend(unsigned char* camera, float m) {
    const int a = WORD(84);
    const int b = WORD(88);
    const float pa = FLOAT(176 + 4 * a);
    const float pb = FLOAT(176 + 4 * b);
    const float lo = FLOAT(176);
    const float hi = FLOAT(184);
    const float z = pb + (pa - pb) * m;
    FLOAT(140) = z;
    FLOAT(216) = 1.0f - (z - lo) / (hi - lo);
    const float qb = FLOAT(188 + 4 * b);
    const float qa = FLOAT(188 + 4 * a);
    FLOAT(252) = qb + (qa - qb) * fp_min(1.0f, m);
}

static inline __attribute__((always_inline)) void hud(unsigned char* camera, int i) {
    fadeTimer[7 * i] = 120;
    fadeIconY[7 * WORD(64)] = hudGetFadeIconPos__Fii(WORD(64), 0);
}

extern "C" void doUserZoom__6Camera(unsigned char* camera) {
    const int i = WORD(64);
    const float* yPtr = reinterpret_cast<const AliasedFloat*>(pad + 88 * i + 32);
    Double ay = fptodp(*yPtr);
    if (dpcmp(ay, 0) < 0) ay = dpsub(0, ay);
    const bool pressed = dpcmp(ay, 0x3fecccccc0000000ULL) > 0;

    if (WORD(80) != 0) {
        if (pressed) hud(camera, i);
    } else if (BYTE(352) != 0) {
        const int held = WORD(72);
        if (gameControlPaused != 0 && BYTE(365) == 0 && held == 0) return;
        WORD(84) = 1;
        WORD(88) = 2;
        const float target = 1.0f - fp_min(1.0f, 2.0f * fp_max(0.0f, FLOAT(212) - 0x1.999998p-4f /* bits 0x3dcccccc */));
        if (held != 0 || BYTE(365) != 0) {
            FLOAT(144) = target;
        } else {
            const float old = FLOAT(144);
            FLOAT(144) = old + (target - old) * 0x1.999998p-5f /* bits 0x3d4ccccc */;
        }
        FLOAT(148) = FLOAT(144);
        blend(camera, FLOAT(144));
        if (pressed) hud(camera, i);
    } else if (FLOAT(156) != 0.0f) {
        if (FLOAT(144) == 0.0f) engineSetBlur__Fi(100);
        const float t = FLOAT(160) + FLOAT(156);
        FLOAT(160) = t;
        const float r = evalHermite__Ffffff(t, 0.0f, 1.0f, 0.5f, 0.5f);
        FLOAT(148) = r;
        FLOAT(144) = r;
        if (clampZoomMag__6Camera(camera) != 0) FLOAT(156) = 0.0f;
        blend(camera, FLOAT(144));
    } else if (WORD(96) != 0 &&
               (*reinterpret_cast<const AliasedInt*>(pad + 88 * i + 8) & automapButton[i]) == 0) {
        if (FLOAT(200) != 0.0f) {
            const float base = FLOAT(148);
            const float m = base + (1.0f - base) * (1.0f - FLOAT(200));
            FLOAT(144) = m;
            blend(camera, m);
        }
        if (pressed && WORD(92) != 0) {
            const int a = WORD(84);
            if (a < 2 && 0x1.ccccccp-1f /* bits 0x3f666666 */ < *yPtr) {
                FLOAT(160) = 0.0f;
                FLOAT(156) = 0x1.47ae14p-5f /* bits 0x3d23d70a */;
                WORD(84) = a + 1;
                FLOAT(144) = 0.0f;
                WORD(88) = a;
            } else if (a != 0 && *yPtr < -0x1.ccccccp-1f /* bits 0x3f666666 */) {
                WORD(84) = a - 1;
                FLOAT(156) = 0x1.47ae14p-5f /* bits 0x3d23d70a */;
                WORD(144) = 0;
                WORD(160) = 0;
                WORD(88) = a;
            } else {
                hud(camera, i);
            }
        }
    }
    WORD(92) = pressed ? 0 : 1;
}
