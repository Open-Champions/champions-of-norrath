// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 17];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static char trace[128];
static int addMaterialResult, beginResult, endResult, ditherSeen = -1;
static void T(const char* s) { std::strcat(trace, s); std::strcat(trace, ","); }
extern "C" {
void UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char*, unsigned*) { T("micro"); }
void SetDither__10VIMateriali(unsigned char*, int d) { T("dither"); ditherSeen = d; }
int AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char*, int, unsigned*) { T("add"); return addMaterialResult; }
int BlitBegin__8VIRasterPP10_sceDmaTag(unsigned char*, unsigned*) { T("begin"); return beginResult; }
int BlitEnd__8VIRasterPP10_sceDmaTag(unsigned char*, unsigned*) { T("end"); return endResult; }
void FlushDMABufferIfFull__8VIRaster(unsigned char*) { T("flush"); }
}
#include "blit.cpp"
#include <cstdio>
static int& W(unsigned off) { return *reinterpret_cast<int*>(arena + off); }
static float& F(unsigned off) { return *reinterpret_cast<float*>(arena + off); }
static void clearTrace() { trace[0] = 0; }
int main() {
    const unsigned table = 0xa000, surface = 0xa100, material = 0xa200, packet = 0xb000, pos = 0xc000, src = 0xc010, size = 0xc020;
    W(19388) = arena_base + table; W(table) = arena_base + surface; W(surface + 8) = 256; W(surface + 12) = 128;
    W(17848) = arena_base + material; W(52) = arena_base + packet; W(19532) = 1;
    F(17732) = 0.0f; F(17736) = 0.0f; F(17740) = 1.0f; F(17744) = 1.0f;
    W(17852) = 7; W(17856) = 9;
    W(pos) = 10; W(pos + 4) = 20; W(src) = 0; W(src + 4) = 0; W(size) = 64; W(size + 4) = 32;
    W(48) = 100;
    std::memset(arena + packet, 0xee, 512);
    clearTrace();
    bool ok = true;
#define CK(c) do { if (!(c)) { std::printf("line %d\n", __LINE__); ok = false; } } while (0)
    CK(Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(arena, 0, 0, reinterpret_cast<int*>(arena + pos), reinterpret_cast<int*>(arena + src), reinterpret_cast<int*>(arena + size), 2, 0.5f) == 0);
    CK(!std::strcmp(trace, "dither,add,begin,end,flush,") && ditherSeen == 1);
    CK(W(material + 48) == -1 && W(material + 60) == 2 && W(material + 32) == 1 && W(17852) == 0 && W(17856) == 2 && W(48) == 104);
    CK(W(52) == static_cast<int>(arena_base + packet + 256));
    const float ex[4] = {10.5f, 74.5f, 74.5f, 10.5f}, ey[4] = {-20.5f, -20.5f, -52.5f, -52.5f};
    const float eu[4] = {0.0f, 0.25f, 0.25f, 0.0f}, ev[4] = {0.0f, 0.0f, 0.25f, 0.25f};
    for (int k = 0; k < 4; ++k) {
        unsigned b = packet + 64 * k;
        CK(F(b) == ex[k] && F(b + 4) == ey[k] && F(b + 8) == 0.0f && F(b + 12) == 1.0f);
        CK(F(b + 16) == eu[k] && F(b + 20) == ev[k] && F(b + 24) == 1.0f && F(b + 28) == 0.0f);
        CK(F(b + 32) == 0.0f && F(b + 36) == 0.0f && F(b + 40) == 0.0f && F(b + 44) == 0.0f);
        CK(F(b + 48) == 1.0f && F(b + 52) == 1.0f && F(b + 56) == 1.0f && F(b + 60) == 0.5f);
    }
    CK(arena[packet + 256] == 0xee);
    // mirrored modes and dither bit; same cache id and blend leave the state flag alone; micro upload when the flag word is not 1
    W(material + 32) = 0; W(19532) = 0; W(52) = arena_base + packet; clearTrace();
    CK(Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(arena, 0, 3 | 4, reinterpret_cast<int*>(arena + pos), reinterpret_cast<int*>(arena + src), reinterpret_cast<int*>(arena + size), 2, 0.5f) == 0);
    CK(!std::strncmp(trace, "micro,dither,", 13) && ditherSeen == 0 && W(material + 32) == 0);
    const float mu[4] = {0.25f, 0.0f, 0.0f, 0.25f}, mv[4] = {0.25f, 0.25f, 0.0f, 0.0f};
    for (int k = 0; k < 4; ++k) CK(F(packet + 64 * k + 16) == mu[k] && F(packet + 64 * k + 20) == mv[k]);
    // failures: the material word +48 is left at the surface id when a callee fails
    W(material + 48) = 5; addMaterialResult = -1; clearTrace();
    CK(Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(arena, 0, 0, reinterpret_cast<int*>(arena + pos), reinterpret_cast<int*>(arena + src), reinterpret_cast<int*>(arena + size), 2, 0.5f) == -1 && !std::strstr(trace, "begin") && W(material + 48) == 0);
    addMaterialResult = 0; beginResult = -1; clearTrace();
    CK(Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(arena, 0, 0, reinterpret_cast<int*>(arena + pos), reinterpret_cast<int*>(arena + src), reinterpret_cast<int*>(arena + size), 2, 0.5f) == -1 && !std::strstr(trace, "end"));
    beginResult = 0; endResult = -1; clearTrace();
    CK(Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(arena, 0, 0, reinterpret_cast<int*>(arena + pos), reinterpret_cast<int*>(arena + src), reinterpret_cast<int*>(arena + size), 2, 0.5f) == -1 && !std::strstr(trace, "flush") && W(material + 48) == 0);
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
