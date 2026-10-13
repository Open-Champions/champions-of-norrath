// Observed: RenderStars__12VIAtmosphereR7VISceneR8VICamera at 0x0106a4d0 is 1640 bytes. The camera
// argument is not read. Words are 32-bit. The atmosphere (this) holds floats F[o] at byte offsets o;
// the scene's word at +19356 is the raster. Billboards are 48-byte records written the same way in all
// three loops: float x, y, z at +0, +4, +8; word 0 at +12; two floats at +16 and +20; and four
// floats (c, c, c, 1.0) at +32..+47 (the original builds that colour in a stack temporary and copies
// 16 bytes). Each billboard is obtained as in RenderSplashes: when the cursor is 0,
// BeginBillboards__8VIRasterPP11VIBillboardi(raster, &cursor, remaining) fills it and returns n; n and
// remaining are decremented per billboard, the cursor advances by 48, and EndBillboards__8VIRaster(raster)
// runs (cursor = 0) when n reaches 0; a nonzero cursor is ended after each loop.
// Part A runs only when F[1564] > 0. It calls SetFog__8VIRasteriffRC10VIColor32F(raster, 0, 0.0, 0.0, c)
// with c four zero floats, then SetMaterial__8VIRasteri(raster, this[1244]). Shooting star: with
// timer T = F[3880] and progress P = F[3884]:
//   if T > 0: T = T - F[1192].
//   else: if P == 0, a new star starts: a = (131 * rand() & 255) as float * 3.9 (0x40799999) * 0.0123
//     (0x3c49d9b2); F[3888] = sinf(a) * 100, F[3892] = 100, F[3896] = cosf(a) * 100 (sinf, cosf order);
//     a second rand gives F[3900] = sinf(a) * 100, F[3904] = 0, F[3908] = cosf(a) * 100, and P = 1e-5
//     (0x3727c5ac). Then, if P > 0 (also on later frames): P = P + F[1192] * 0.2 (0x3e4ccccc); two more
//     rand() values r1, r2 give b = float(r1 - r2 as floats) * 2^-30 * 0.3 + 0.7; if P < 0.2 b = b * (P * 5)
//     else b = b * (1 - (P - 0.2) * 1.25); b is clamped to [0, 1] (max.s then min.s); one billboard
//     (remaining 1) is drawn at start + (end - start) * P, where start is F[3888..3896] and end is
//     F[3900..3908]; its +16 and +20 floats are 0.5 and its colour is (b, b, b, 1.0). If P >= 1.0
//     afterwards: P = 0 and T = float((rand() & 15) + 10).
//   Twinkling stars, always in part A: k = (signed char)(int)F[3876]; F[3876] += F[1192] * 15, minus 32
//   when above 32. For 128 stars i (remaining starts at 128): star data are 16-byte records at this+1568
//   (x, y, z, w) and signed bytes at this+3616; the billboard gets x, y, z, with w at +16 and +20, and colour
//   c = F[3744 + 4 * ((k + byte[i]) & 31)] * F[1564] (colour (c, c, c, 1.0)).
// Part B runs when F[3912] > 0: SetFog(raster, 0, 0.0, 0.0, zero colour), SetMaterial(raster, this[3920]),
// then 14 billboards from 16-byte records at this+3936 (x, y, z, w; w at +16 and +20), remaining starts at
// 14, colour c = min(F[3912] * 0.45, 0.15), colour (c, c, c, 1.0), F[3912] re-read per record.
// Inferred: stars with twinkle, a shooting star with a timer, and a second (fainter) layer.
// Unknown: member names, the layout of the 16-byte records beyond the use here, and whether rand/sinf
// are the C library versions (they are called as such).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void SetFog__8VIRasteriffRC10VIColor32F(unsigned char* raster, int mode, float a, float b, const float* color);
void SetMaterial__8VIRasteri(unsigned char* raster, int material);
int BeginBillboards__8VIRasterPP11VIBillboardi(unsigned char* raster, unsigned* cursor, int count);
void EndBillboards__8VIRaster(unsigned char* raster);
int rand(void);
float sinf(float x);
float cosf(float x);

static inline float K(unsigned bits) { union { unsigned u; float f; } v; v.u = bits; return v.f; }
static inline float& F(unsigned char* base, unsigned offset) { return *reinterpret_cast<float*>(base + offset); }
static inline unsigned& W(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned*>(base + offset); }

// Writes position, the two w floats and the (c, c, c, 1.0) colour of one billboard at the cursor.
static inline void put_color(unsigned char* b, float c) {
    F(b, 32) = c; F(b, 36) = c; F(b, 40) = c; F(b, 44) = K(0x3f800000);
}

void RenderStars__12VIAtmosphereR7VISceneR8VICamera(unsigned char* self, unsigned char* scene, unsigned char* camera) {
    (void)camera;
    unsigned cursor = 0;
    unsigned char* raster = NORRATH_WORD_TO_POINTER(W(scene, 19356));
    if (0.0f < F(self, 1564)) {
        const float zero_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        SetFog__8VIRasteriffRC10VIColor32F(raster, 0, 0.0f, 0.0f, zero_color);
        SetMaterial__8VIRasteri(raster, static_cast<int>(W(self, 1244)));
        float dt;
        if (F(self, 3880) > 0.0f) {
            F(self, 3880) = F(self, 3880) - F(self, 1192);
            dt = F(self, 1192);
        } else {
            float progress = F(self, 3884);
            if (progress == 0.0f) {
                float angle = static_cast<float>((rand() * 131) & 0xff) * K(0x40799999) * K(0x3c49d9b2);
                const float s0 = sinf(angle);
                F(self, 3892) = K(0x42c80000);
                F(self, 3888) = s0 * K(0x42c80000);
                const float c0 = cosf(angle);
                F(self, 3896) = c0 * K(0x42c80000);
                angle = static_cast<float>((rand() * 131) & 0xff) * K(0x40799999) * K(0x3c49d9b2);
                const float s1 = sinf(angle);
                F(self, 3904) = 0.0f;
                F(self, 3900) = s1 * K(0x42c80000);
                const float c1 = cosf(angle);
                progress = K(0x3727c5ac);
                F(self, 3884) = progress;
                F(self, 3908) = c1 * K(0x42c80000);
            }
            if (0.0f < progress) {
                F(self, 3884) = progress + F(self, 1192) * K(0x3e4ccccc);
                const int r1 = rand();
                const int r2 = rand();
                const float p = F(self, 3884);
                float b = (static_cast<float>(r1) - static_cast<float>(r2)) * K(0x30000000) * K(0x3e999999) + K(0x3f333333);
                if (p < K(0x3e4ccccc)) b = b * (p * K(0x40a00000));
                else b = b * (K(0x3f800000) - (p - K(0x3e4ccccc)) * K(0x3fa00000));
                BeginBillboards__8VIRasterPP11VIBillboardi(raster, &cursor, 1);
                unsigned char* bb = NORRATH_WORD_TO_POINTER(cursor);
                const float zero = 0.0f;
                if (b < zero) b = zero;
                if (b > K(0x3f800000)) b = K(0x3f800000);
                const float ex = F(self, 3900), ey = F(self, 3904), ez = F(self, 3908);
                F(bb, 0) = ex; W(bb, 12) = 0; F(bb, 4) = ey; F(bb, 8) = ez;
                float x = ex - F(self, 3888);
                float y = ey - F(self, 3892);
                float z = ez - F(self, 3896);
                F(bb, 0) = x; F(bb, 4) = y; F(bb, 8) = z;
                const float p2 = F(self, 3884);
                z = z * p2; x = x * p2; y = y * p2;
                F(bb, 8) = z; F(bb, 0) = x; F(bb, 4) = y;
                x = x + F(self, 3888); F(bb, 0) = x;
                y = y + F(self, 3892); F(bb, 4) = y;
                z = z + F(self, 3896);
                F(bb, 20) = K(0x3f000000); F(bb, 16) = K(0x3f000000); F(bb, 8) = z;
                put_color(bb, b);
                EndBillboards__8VIRaster(raster);
                cursor = 0;
                if (K(0x3f800000) <= F(self, 3884)) {
                    F(self, 3884) = zero;
                    F(self, 3880) = static_cast<float>((rand() & 0xf) + 10);
                }
            }
            dt = F(self, 1192);
        }
        const int phase = static_cast<signed char>(static_cast<int>(F(self, 3876)));
        const float next_phase = F(self, 3876) + dt * K(0x41700000);
        F(self, 3876) = next_phase;
        if (K(0x42000000) < next_phase) F(self, 3876) = next_phase - K(0x42000000);
        const signed char* offsets = reinterpret_cast<const signed char*>(self + 3616);
        unsigned char* star = self + 1568;
        int remaining = 128;
        int available = 0;
        for (int i = 0; i < 128; ++i) {
            const float c = F(self, 3744 + 4 * ((phase + offsets[i]) & 0x1f)) * F(self, 1564);
            if (cursor == 0) available = BeginBillboards__8VIRasterPP11VIBillboardi(raster, &cursor, remaining);
            unsigned char* bb = NORRATH_WORD_TO_POINTER(cursor);
            --available;
            F(bb, 0) = F(star, 0); --remaining; W(bb, 12) = 0;
            F(bb, 4) = F(star, 4); F(bb, 8) = F(star, 8);
            F(bb, 16) = F(star, 12); F(bb, 20) = F(star, 12);
            star += 16;
            put_color(bb, c);
            cursor = cursor + 48;
            if (available == 0) { EndBillboards__8VIRaster(raster); cursor = 0; }
        }
        if (cursor != 0) { EndBillboards__8VIRaster(raster); cursor = 0; }
    }
    if (0.0f < F(self, 3912)) {
        const float zero_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        SetFog__8VIRasteriffRC10VIColor32F(raster, 0, 0.0f, 0.0f, zero_color);
        SetMaterial__8VIRasteri(raster, static_cast<int>(W(self, 3920)));
        unsigned char* record = self + 3936;
        int remaining = 14;
        int available = 0;
        for (int i = 0; i < 14; ++i) {
            if (cursor == 0) available = BeginBillboards__8VIRasterPP11VIBillboardi(raster, &cursor, remaining);
            unsigned char* bb = NORRATH_WORD_TO_POINTER(cursor);
            --available;
            F(bb, 0) = F(record, 0); W(bb, 12) = 0; --remaining;
            F(bb, 4) = F(record, 4); F(bb, 8) = F(record, 8);
            F(bb, 16) = F(record, 12); F(bb, 20) = F(record, 12);
            record += 16;
            float c = F(self, 3912) * K(0x3ee66666);
            if (c > K(0x3e199999)) c = K(0x3e199999);
            put_color(bb, c);
            cursor = cursor + 48;
            if (available == 0) { EndBillboards__8VIRaster(raster); cursor = 0; }
        }
        if (cursor != 0) { EndBillboards__8VIRaster(raster); cursor = 0; }
    }
}
}
