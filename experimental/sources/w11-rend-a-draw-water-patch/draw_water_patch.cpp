#include "../w11-rend-a-common/rend.hpp"
// Observed drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi at 0x00178ac0, 1928 bytes. Arguments:
// the world header, a polyset header ph and an integer layer. Words used: ph+0 (pointer to a patch
// record h), ph+8, +12, +16, +20, +24, +28 (floats), ph+36 (flags: 0x1000 selects the second set of
// water globals), ph+48 (an int); h+8 (pointer to a water record wd). Water records are linked in
// a list by the words at wd+4760 (towards older) and wd+4764 (towards newer); waterList is the newest,
// waterListBack the oldest. Other wd words: +4752 world, +4756 frame number, +4768 owning polyset.
//  1. Record: with no wd, the oldest record is recycled: waterListBack moves to its newer
//     neighbour (whose older link is cleared), the previous owner's patch record gets +8 cleared, and
//     the record becomes the newest with +4752 = world, +4756 = frameNm, +4768 = ph and h+8 = record;
//     then smoothPatch__FP12_worldHeaderP16WaterPolysetData(world, h) runs. With a wd it gets
//     +4756 = frameNm and, unless already the newest, is moved to the front of the list.
//  2. Packet at glistp (g): the word 0x11000000 at g+8; the cursor starts at g+12 and zero bytes pad
//     it until its low four bits are 12 (c). From c: a word 0x50000000 later or-ed with a quadword
//     count; quadwords at c+4 (0x1000000000008000, 14), (0, 63), (0, 6), (0x71001, 71); then
//     ((alpha << 32) | 100 or, for negative alpha, ((-alpha) << 32) | 104, 66) with alpha the
//     int worldWaterAlpha (worldWaterAlpha2 when flag 0x1000); (97, 20); (colour, 1) with
//     worldWaterColor (worldWaterColor2 with the flag) stored as a 64-bit sign-extended int; then the
//     words layer | 0x04000000 and 0x6c054000. After that come 16 floats: matMul of a 3-row matrix
//     m (zero, with m[2] = worldWaterWavyness (worldWaterWavyness2 with the flag), m[4] = 1,
//     m[6] = 1 and translation m[9] = floor(ph[8] + 0.5), m[10] = floor(ph[12] + 0.5),
//     m[11] = (float)(int)ph[48] * 0.25) with worldCharViewMatrix; then the words 0x803e, 0x20ae4000,
//     67, the bits of floor((ph[20] - ph[8]) + 0.5) * 0x1.11111p-5, and 0x14000036. Zero bytes pad to a
//     multiple of 16. The quadword count is stored into the 64-bit tag at g as
//     ((((end - g) >> 3) - 2) >> 1) | 0x10000000, and the 0x50000000 word at c is or-ed with
//     ((end of the matrix block - c - 4) >> 4) (the quadword group before the matrix words).
//     A tag ((wd + 64) << 32) | 0xa1 | 0x30000000 and a zero quadword follow at the end; when bits 28 and
//     29 of that tag equal 1 (which this tag never gives) its ID is changed to 6, otherwise a
//     (0x60000000, 0) quadword is appended. glistp advances to the end and
//     drawAddToList__FPviP8_textureP11_drawRecordi(g, 3, waterTexture, 0, 0) is called.
//  3. When levelFlags has bit 0x10 and gameIsPaused__Fv() is zero: one eRandState step; unless
//     (state >> 16) % 300 is zero nothing more happens. Otherwise a second step gives u =
//     (float)(state >> 16) * 2^-16. Points P = (ph[8], ph[12], ph[16]) and Q = (ph[20], ph[24], ph[28]);
//     P.x += (Q.x - P.x) * u and P.y += (Q.y - P.y) * u (P.z is unchanged);
//     worldPerturbWater__FP12_worldHeaderG6Point3ff(world, P, 1500.0f, 6.0f); P.z =
//     worldFindHeight__FP12_worldHeaderG6Point3f(world, P, 6.0f); P_AddParticle(rainSplashParticle,
//     P, (0, 0, 0), 0.0f); a third step gives v and P.z += v * 2^-16 * 80.0f + 24.0f;
//     P_AddParticle(droplet, P, (0, 0, 0), 0.0f).
// Nothing is returned.
// Inferred: draws one animated water patch from a recycled record and occasionally spawns a splash
// and droplet on it. Unknown: the water-record layout beyond the words above, the meaning of the
// numeric packet tags, and the matrix. Nothing here was executed on the EE.
extern "C" {
extern int worldWaterAlpha, worldWaterAlpha2, worldWaterColor, worldWaterColor2;
extern float worldWaterWavyness, worldWaterWavyness2;
extern unsigned char* waterTexture;
extern unsigned char* waterList;
extern unsigned char* waterListBack;
extern unsigned char* glistp;
extern int frameNm, levelFlags, eRandState;
extern float worldCharViewMatrix[16];
extern unsigned char rainSplashParticle[80];
extern unsigned char droplet[80];
void smoothPatch__FP12_worldHeaderP16WaterPolysetData(unsigned char* world, unsigned char* data);
void matMul__FP8Matrix44P8Matrix34T0(float* out, const float* a, const float* b);
void drawAddToList__FPviP8_textureP11_drawRecordi(void* list, int kind, unsigned char* texture, unsigned char* record, int extra);
int gameIsPaused__Fv();
void worldPerturbWater__FP12_worldHeaderG6Point3ff(unsigned char* world, const float* point, float a, float b);
float worldFindHeight__FP12_worldHeaderG6Point3f(unsigned char* world, const float* point, float f);
void P_AddParticle__FP11ParticleDefP6Point3T1f(unsigned char* def, float* point, float* velocity, float f);
float floorf(float);
}
namespace {
using namespace rend;
typedef unsigned long long __attribute__((may_alias)) u64;
typedef long long __attribute__((may_alias)) i64;
inline unsigned low4(const unsigned char* p) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_address(p) & 15u;
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)) & 15u;
#endif
}
inline unsigned address(const unsigned char* p) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_address(p);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p));
#endif
}
inline void quad(unsigned char* p, u64 low, u64 high) {
    at<u64>(p, 0) = low;
    at<u64>(p, 8) = high;
}
}  // namespace
extern "C" void drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(unsigned char* world, unsigned char* ph, int layer) {
    unsigned char* h = ptr(ph, 0);
    unsigned char* wd = ptr(h, 8);
    if (wd == nullptr) {
        unsigned char* record = waterListBack;
        unsigned char* newer = ptr(record, 4764);
        unsigned char* owner = ptr(record, 4768);
        waterListBack = newer;
        at<u32>(newer, 4760) = 0;
        if (owner != nullptr) at<u32>(ptr(owner, 0), 8) = 0;
        unsigned char* head = waterList;
        set_ptr(record, 4760, head);
        set_ptr(record, 4768, ph);
        set_ptr(head, 4764, record);
        waterList = record;
        at<u32>(record, 4764) = 0;
        at<i32>(record, 4756) = frameNm;
        set_ptr(h, 8, record);
        set_ptr(record, 4752, world);
        smoothPatch__FP12_worldHeaderP16WaterPolysetData(world, h);
        wd = ptr(h, 8);
    } else {
        at<i32>(wd, 4756) = frameNm;
        unsigned char* newer = ptr(wd, 4764);
        if (newer != nullptr) {
            unsigned char* older = ptr(wd, 4760);
            if (older == nullptr) waterListBack = newer;
            else set_ptr(older, 4764, newer);
            unsigned char* head = waterList;
            unsigned char* nowNewer = ptr(wd, 4764);
            at<u32>(nowNewer, 4760) = at<u32>(wd, 4760);
            set_ptr(wd, 4760, head);
            set_ptr(head, 4764, wd);
            waterList = wd;
            at<u32>(wd, 4764) = 0;
            wd = ptr(h, 8);
        }
    }
    unsigned char* const g = glistp;
    const unsigned flags = at<u32>(ph, 36);
    const bool second = (flags & 0x1000) != 0;
    at<u32>(g, 8) = 0x11000000u;
    unsigned char* cursor = g + 12;
    while (low4(cursor) != 12) *cursor++ = 0;
    unsigned char* const c = cursor;
    at<u32>(c, 0) = 0x50000000u;
    unsigned char* const first = c + 4;
    quad(c + 4, 0x1000000000008000ull, 14);
    quad(c + 20, 0, 63);
    quad(c + 36, 0, 6);
    quad(c + 52, 0x71001, 71);
    int alpha = second ? worldWaterAlpha2 : worldWaterAlpha;
    if (alpha < 0) quad(c + 68, (static_cast<u64>(static_cast<unsigned>(-alpha)) << 32) | 104, 66);
    else quad(c + 68, (static_cast<u64>(static_cast<unsigned>(alpha)) << 32) | 100, 66);
    unsigned char* const t0 = c + 84;
    quad(t0, 97, 20);
    quad(t0 + 16, static_cast<u64>(static_cast<i64>(second ? worldWaterColor2 : worldWaterColor)), 1);
    unsigned char* const t3 = t0 + 32;
    const float width = at<f32>(ph, 20) - at<f32>(ph, 8);
    const unsigned countA = static_cast<unsigned>((static_cast<int>(t3 - first) >> 3) - 2 >> 1);
    const unsigned countB = static_cast<unsigned>(static_cast<int>(t3 - c - 4) >> 4);
    at<u64>(first, 0) = at<u64>(first, 0) | countA;
    at<u32>(c, 0) = at<u32>(c, 0) | countB;
    at<u32>(t3, 0) = static_cast<unsigned>(layer) | 0x04000000u;
    at<u32>(t3, 4) = 0x6c054000u;
    unsigned char* const matrixAt = t0 + 40;
    const float scaled = floorf(width + 0.5f);
    float m1[12];
    for (int i = 0; i < 12; ++i) m1[i] = 0.0f;
    m1[2] = second ? worldWaterWavyness2 : worldWaterWavyness;
    m1[4] = 1.0f;
    m1[6] = 1.0f;
    float m2[12];
    for (int i = 0; i < 12; ++i) m2[i] = m1[i];
    m2[9] = floorf(at<f32>(ph, 8) + 0.5f);
    m2[10] = floorf(at<f32>(ph, 12) + 0.5f);
    m2[11] = static_cast<float>(at<i32>(ph, 48)) * 0.25f;
    float m3[16];
    matMul__FP8Matrix44P8Matrix34T0(m3, m2, worldCharViewMatrix);
    for (int i = 0; i < 16; ++i) at<f32>(matrixAt, i * 4) = m3[i];
    const float tail = scaled * 0x1.111110p-5f;
    at<u32>(matrixAt, 64) = 0x803eu;
    at<u32>(matrixAt, 68) = 0x20ae4000u;
    at<u32>(matrixAt, 72) = 67;
    at<f32>(matrixAt, 76) = tail;
    at<u32>(matrixAt, 80) = 0x14000036u;
    unsigned char* e = matrixAt + 84;
    while (low4(e) != 0) *e++ = 0;
    const u64 tag = ((static_cast<u64>(address(wd) + 64u) << 32) | 0xa1u) | 0x30000000u;
    at<u64>(g, 0) = (static_cast<unsigned>((static_cast<int>(e - g) >> 3) - 2 >> 1)) | 0x10000000u;
    unsigned char* end = e + 16;
    at<u64>(e, 0) = tag;
    at<u64>(e, 8) = 0;
    if (((tag >> 28) & 3) == 1) {
        at<u64>(e, 0) = (tag & 0x8fffffffu) | 0x60000000u;
    } else {
        quad(e + 16, 0x60000000u, 0);
        end = e + 32;
    }
    glistp = end;
    drawAddToList__FPviP8_textureP11_drawRecordi(g, 3, waterTexture, nullptr, 0);
    if ((levelFlags & 0x10) == 0) return;
    if (gameIsPaused__Fv() != 0) return;
    unsigned s = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
    eRandState = static_cast<int>(s);
    if ((s >> 16) % 300u != 0) return;
    unsigned s2 = s * 0x19660du + 0x3c6ef35fu;
    eRandState = static_cast<int>(s2);
    float P[3] = {at<f32>(ph, 8), at<f32>(ph, 12), at<f32>(ph, 16)};
    float Q[3] = {at<f32>(ph, 20), at<f32>(ph, 24), at<f32>(ph, 28)};
    const float u = static_cast<float>(s2 >> 16) * 0x1p-16f;
    float D[3] = {Q[0] - P[0], Q[1] - P[1], Q[2] - P[2]};
    float D2[3] = {D[0] * u, D[1] * u, D[2] * u};
    P[0] = P[0] + D2[0];
    P[1] = P[1] + D2[1];
    worldPerturbWater__FP12_worldHeaderG6Point3ff(world, P, 0x1.77000p+10f, 6.0f);
    float V[3] = {0.0f, 0.0f, 0.0f};
    P[2] = worldFindHeight__FP12_worldHeaderG6Point3f(world, P, 6.0f);
    P_AddParticle__FP11ParticleDefP6Point3T1f(rainSplashParticle, P, V, 0.0f);
    unsigned s3 = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
    eRandState = static_cast<int>(s3);
    P[2] = P[2] + static_cast<float>(s3 >> 16) * 0x1p-16f * 80.0f + 24.0f;
    P_AddParticle__FP11ParticleDefP6Point3T1f(droplet, P, V, 0.0f);
}
