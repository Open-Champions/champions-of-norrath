#include "../w11-rend-a-common/rend.hpp"
// Observed drawWorldObject__FP12_worldHeaderP13PolysetHeaderi at 0x00179248, 2292 bytes. Arguments:
// the world header (not used), a polyset header ph and an integer layer. Words used: ph+0 (pointer to
// an object record h), ph+4 (a pointer to an animation state), ph+36 (flags: 0x1, 0x40), ph+40, +44,
// +48 (ints), ph+52 (bit 18 chooses a matrix row; 0x10000 and 0x20000 negate the two rows),
// ph+54 and +56 (signed halfwords); h+0 (a name string), h+80 (a second name), h+160, +164, +168
// (resource pointers: model, texture, animation header), h+172 (flags 0x1, 0x2, 0x4, 0x8, 0x10).
//  1. When h+160 is zero the resources are found: lump = sprintf("%s.lmp", h); name = copy of h+80,
//     cut at its first '_' (and h+172 |= 0x10 when there is one); h+160, +164, +168 =
//     lumpFindResource(lump, sprintf("%s.vif" / "%s.tex" / "%s.anm", name)).
//  2. With h+168 nonzero the animation state s5 is ph+4 when set; otherwise the first of 64
//     48-byte worldAnimation entries is used whose byte +4 is zero (animAdd(entry, h+168, 1) and
//     animRandomizeTime(entry) first) or whose word +8 equals h+168, and ph+4 is set to it. s5 stays
//     zero when none matches or h+168 is zero.
//  3. Packet at glistp g: 0x11000000 at g+8, cursor g+12 padded to low bits 12 (c); word 0x50000005
//     at c; from c+4 quadwords (0x1000000000008000, 14), (0, 63), (0, 6), (the 64-bit word at model+24, 66),
//     (0x507f5, 71); the first tag gets the count 4; the word 0x6c04c000. Then two matrix rows of
//     four floats from worldCharViewMatrix: with ph flag 0x1 the rows (bit 18 of ph+52) and its
//     other row, negated by 0x10000 and 0x20000 respectively; without it, rows built from the
//     halfwords ph+54, ph+56 scaled by 0x1.0002p-15 (the second row rotated, negated when flag
//     0x40 is clear). Then worldCharViewMatrix's third row, and four floats W[12 + k] +
//     W[k] * px + W[4 + k] * py + W[8 + k] * pz with p = the ints ph+40..48 times 0.25, then
//     layer | 0x04000000 and 0x14000002.
//  4. Lights by h+172: bit 4 first calls lightDisableVolumetricLighting; then bit 1 lightSetupForCharacter(p),
//     else bit 2 lightSetupNonDirectionalDirectional, else bit 8 lightSetAmbientColor({128,128,128,0})
//     and lightSetupAmbientOnly, else lightSetupAmbientAndDirOnly(p, worldTileEntityLightScale); with
//     bits 1 or 8 lightResetOptionalParams follows. Zero bytes pad to 16.
//  5. The tag at g gets ((end - g) >> 3 - 2) >> 1 | 0x10000000. A quadword (0x10000000, 0x1400001e) (or, when
//     an animation state exists, animLoadBoneMatricies(packet, s5) then (0x10000000, 0x1400000c)) follows;
//     modelDrawCharacter(packet, model, -1, 0) draws; then the tag saved before it has its ID changed
//     from 1 to 6 if its bits 28..30 equal 1, else a (0x60000000, 0) quadword is added. glistp
//     advances, and drawAddToList(g, (halfword model+16 >> 1) & 4, h+164, 0, 0) is called.
// Nothing is returned.
// Inferred: draws one placed world object model, with its animation and lighting. Unknown: the layouts
// of h and the model beyond the words named, and what the numeric tags mean. Nothing here was executed
// on the EE.
extern "C" {
extern unsigned char* glistp;
extern float worldCharViewMatrix[16];
extern float worldTileEntityLightScale;
extern unsigned char worldAnimation[];
int sprintf(char* buffer, const char* format, ...);
char* strcpy(char* to, const char* from);
char* strchr(const char* string, int c);
unsigned char* lumpFindResource__FPcT0(char* a, char* b);
void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char* state, unsigned char* header, int flag);
void animRandomizeTime__FP14AnimationState(unsigned char* state);
void lightDisableVolumetricLighting__Fv();
void lightSetupForCharacter__FP6PacketG6Point3(unsigned char* packet, const float* point);
void lightSetupNonDirectionalDirectional__FP6Packet(unsigned char* packet);
void lightSetAmbientColor__FPs(short* color);
void lightSetupAmbientOnly__FP6Packet(unsigned char* packet);
void lightSetupAmbientAndDirOnly__FP6PacketG6Point3f(unsigned char* packet, const float* point, float scale);
void lightResetOptionalParams__Fv();
void animLoadBoneMatricies__FP6PacketP14AnimationState(unsigned char* packet, unsigned char* state);
void modelDrawCharacter__FP6PacketP12_modelHeaderli(unsigned char* packet, unsigned char* model, long a, int b);
void drawAddToList__FPviP8_textureP11_drawRecordi(void* list, int kind, unsigned char* texture, unsigned char* record, int extra);
}
namespace {
using namespace rend;
typedef unsigned long long __attribute__((may_alias)) u64;
inline unsigned low4(const unsigned char* p) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_address(p) & 15u;
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)) & 15u;
#endif
}
inline void quad(unsigned char* p, u64 low, u64 high) {
    at<u64>(p, 0) = low;
    at<u64>(p, 8) = high;
}
inline void rowOut(unsigned char* to, const float* row, bool negate) {
    for (int i = 0; i < 4; ++i) at<f32>(to, i * 4) = negate ? -row[i] : row[i];
}
}  // namespace
extern "C" void drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(unsigned char* world, unsigned char* ph, int layer) {
    (void)world;
    unsigned char* h = ptr(ph, 0);
    char lump[160];
    char name[160];
    char path[160];
    if (ptr(h, 160) == nullptr) {
        sprintf(lump, "%s.lmp", reinterpret_cast<char*>(h));
        strcpy(name, reinterpret_cast<char*>(h + 80));
        if (strchr(name, '_') != nullptr) {
            *strchr(name, '_') = 0;
            at<u32>(h, 172) = at<u32>(h, 172) | 0x10;
        }
        sprintf(path, "%s.vif", name);
        set_ptr(h, 160, lumpFindResource__FPcT0(lump, path));
        sprintf(path, "%s.tex", name);
        set_ptr(h, 164, lumpFindResource__FPcT0(lump, path));
        sprintf(path, "%s.anm", name);
        set_ptr(h, 168, lumpFindResource__FPcT0(lump, path));
    }
    unsigned char* state = nullptr;
    if (ptr(h, 168) != nullptr) {
        if (ptr(ph, 4) != nullptr) {
            state = ptr(ph, 4);
        } else {
            unsigned char* entry = worldAnimation;
            for (int i = 0; i < 64;) {
                ++i;
                if (at<i8>(entry, 4) == 0) {
                    animAdd__FP14AnimationStateP15AnimationHeaderi(entry, ptr(h, 168), 1);
                    animRandomizeTime__FP14AnimationState(entry);
                }
                if (ptr(entry, 8) == ptr(h, 168)) {
                    set_ptr(ph, 4, entry);
                    break;
                }
                entry += 48;
            }
            if (ptr(ph, 4) != nullptr) state = ptr(ph, 4);
        }
    }
    unsigned char packet[12];
    unsigned char* const g = glistp;
    set_ptr(packet, 0, g);
    set_ptr(packet, 4, g);
    at<u32>(g, 8) = 0x11000000u;
    unsigned char* model = ptr(h, 160);
    unsigned char* cursor = g + 12;
    set_ptr(packet, 0, cursor);
    while (low4(cursor) != 12) *cursor++ = 0;
    unsigned char* const t4 = cursor + 4;
    set_ptr(packet, 0, t4);
    set_ptr(packet, 8, t4);
    at<u32>(cursor, 0) = 0x50000005u;
    quad(t4, 0x1000000000008000ull, 14);
    set_ptr(packet, 0, t4 + 80);
    quad(t4 + 16, 0, 63);
    quad(t4 + 32, 0, 6);
    const float px = static_cast<float>(at<i32>(ph, 40)) * 0.25f;
    const float py = static_cast<float>(at<i32>(ph, 44)) * 0.25f;
    const float pz = static_cast<float>(at<i32>(ph, 48)) * 0.25f;
    quad(t4 + 48, at<u64>(model, 24), 66);
    at<u32>(t4, 80) = 0x6c04c000u;
    quad(t4 + 64, 0x507f5, 71);
    at<u64>(t4, 0) = at<u64>(t4, 0) | static_cast<unsigned>(((80 >> 3) - 2) >> 1);
    float P[3] = {px, py, pz};
    const unsigned flags = at<u32>(ph, 36);
    const float* W = worldCharViewMatrix;
    unsigned char* const M = t4 + 84;
    set_ptr(packet, 0, M);
    unsigned char* after;
    if ((flags & 1) != 0) {
        const unsigned w52 = at<u32>(ph, 52);
        const unsigned row = (w52 >> 18) & 1u;
        const unsigned other = row ^ 1u;
        set_ptr(packet, 0, t4 + 100);
        rowOut(M, W + row * 4, (w52 & 0x10000) != 0);
        unsigned char* const q = ptr(packet, 0);
        rowOut(q, W + other * 4, (at<u32>(ph, 52) & 0x20000) != 0);
        after = q + 16;
    } else {
        const float b = static_cast<float>(at<i16>(ph, 56)) * 0x1.000200p-15f;
        const float a = static_cast<float>(at<i16>(ph, 54)) * 0x1.000200p-15f;
        set_ptr(packet, 0, t4 + 100);
        at<f32>(M, 0) = a * W[0] + b * W[4];
        at<f32>(M, 4) = a * W[1] + b * W[5];
        at<f32>(M, 8) = a * W[2] + b * W[6];
        at<f32>(M, 12) = a * W[3] + b * W[7];
        unsigned char* const q = t4 + 100;
        if ((flags & 0x40) != 0) {
            at<f32>(q, 0) = b * W[0] - a * W[4];
            at<f32>(q, 4) = b * W[1] - a * W[5];
            at<f32>(q, 8) = b * W[2] - a * W[6];
            at<f32>(q, 12) = b * W[3] - a * W[7];
        } else {
            const float nb = -b;
            at<f32>(q, 0) = nb * W[0] + a * W[4];
            at<f32>(q, 4) = nb * W[1] + a * W[5];
            at<f32>(q, 8) = nb * W[2] + a * W[6];
            at<f32>(q, 12) = nb * W[3] + a * W[7];
        }
        after = q + 16;
    }
    set_ptr(packet, 0, after);
    unsigned char* const r = after;
    for (int i = 0; i < 4; ++i) at<f32>(r, i * 4) = W[8 + i];
    for (int k = 0; k < 4; ++k) {
        float v = W[12 + k] + W[k] * P[0];
        v = v + W[4 + k] * P[1];
        v = v + W[8 + k] * P[2];
        at<f32>(r, 16 + k * 4) = v;
    }
    at<u32>(r, 32) = static_cast<unsigned>(layer) | 0x04000000u;
    at<u32>(r, 36) = 0x14000002u;
    set_ptr(packet, 0, r + 40);
    unsigned lf = at<u32>(h, 172);
    if ((lf & 4) != 0) {
        lightDisableVolumetricLighting__Fv();
        lf = at<u32>(h, 172);
    }
    if ((lf & 1) != 0) {
        lightSetupForCharacter__FP6PacketG6Point3(packet, P);
    } else if ((lf & 2) != 0) {
        lightSetupNonDirectionalDirectional__FP6Packet(packet);
    } else if ((lf & 8) != 0) {
        short ambient[4] = {128, 128, 128, 0};
        lightSetAmbientColor__FPs(ambient);
        lightSetupAmbientOnly__FP6Packet(packet);
    } else {
        lightSetupAmbientAndDirOnly__FP6PacketG6Point3f(packet, P, worldTileEntityLightScale);
    }
    if ((at<u32>(h, 172) & 9) != 0) lightResetOptionalParams__Fv();
    unsigned char* end = ptr(packet, 0);
    while (low4(end) != 0) *end++ = 0;
    set_ptr(packet, 0, end);
    unsigned char* const start = ptr(packet, 4);
    at<u64>(start, 0) = static_cast<unsigned>(((static_cast<int>(end - start) >> 3) - 2) >> 1) | 0x10000000u;
    unsigned head;
    if (state == nullptr) {
        set_ptr(packet, 4, end);
        head = 0x1400001e;
    } else {
        animLoadBoneMatricies__FP6PacketP14AnimationState(packet, state);
        set_ptr(packet, 4, ptr(packet, 0));
        head = 0x1400000c;
    }
    unsigned char* const now = ptr(packet, 0);
    quad(now, 0x10000000, head);
    set_ptr(packet, 0, now + 16);
    modelDrawCharacter__FP6PacketP12_modelHeaderli(packet, model, -1, 0);
    unsigned char* const saved = ptr(packet, 4);
    const u64 tag = at<u64>(saved, 0);
    if (((tag >> 28) & 7) == 1) {
        at<u64>(saved, 0) = (tag & 0x8fffffffu) | 0x60000000u;
    } else {
        unsigned char* const q = ptr(packet, 0);
        set_ptr(packet, 4, nullptr);
        quad(q, 0x60000000u, 0);
        set_ptr(packet, 0, q + 16);
    }
    glistp = ptr(packet, 0);
    drawAddToList__FPviP8_textureP11_drawRecordi(g, (at<u16>(model, 16) >> 1) & 4, ptr(h, 164), nullptr, 0);
}
