#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
static unsigned long long dword(const unsigned char* p) { unsigned long long v; std::memcpy(&v, p, 8); return v; }
static std::vector<std::string> events;
static int lightAdvance, drawAdvance, boneCalls;
static unsigned char* lastPacket;
static unsigned cursorAtLight;
static unsigned char* drawModel; static long drawA; static int drawB;
static void* addList; static int addKind; static unsigned char* addTex; static unsigned char* addRec; static int addExtra;
static std::string lumpSeen[3]; static int lumpCalls;
static unsigned char* animEntries[4]; static int animAdds;
extern "C" {
unsigned char* glistp;
float worldCharViewMatrix[16];
float worldTileEntityLightScale;
unsigned char worldAnimation[64 * 48];
unsigned char* lumpFindResource__FPcT0(char* a, char* b) {
    lumpSeen[lumpCalls] = std::string(a) + "|" + b; return arena_at(0x3000 + 0x100 * lumpCalls++);
}
void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char* state, unsigned char* header, int flag) {
    if (animAdds < 4) animEntries[animAdds] = state;
    ++animAdds; state[4] = 1; sw_(state + 8, norrath_host_address(header)); if (flag != 1) events.push_back("badflag");
}
void animRandomizeTime__FP14AnimationState(unsigned char*) { events.push_back("rand"); }
void lightDisableVolumetricLighting__Fv() { events.push_back("disable"); }
static void advance(unsigned char* packet, const char* name) {
    lastPacket = packet; events.push_back(name);
    cursorAtLight = hw(packet);
    sw_(packet, hw(packet) + lightAdvance);
}
void lightSetupForCharacter__FP6PacketG6Point3(unsigned char* packet, const float* p) {
    advance(packet, "character"); std::memcpy(arena_at(0x2f00), p, 12);
}
void lightSetupNonDirectionalDirectional__FP6Packet(unsigned char* packet) { advance(packet, "nondir"); }
void lightSetAmbientColor__FPs(short* c) {
    if (c[0] != 128 || c[1] != 128 || c[2] != 128 || c[3] != 0) events.push_back("badambient");
    events.push_back("ambient");
}
void lightSetupAmbientOnly__FP6Packet(unsigned char* packet) { advance(packet, "ambientonly"); }
void lightSetupAmbientAndDirOnly__FP6PacketG6Point3f(unsigned char* packet, const float*, float s) {
    advance(packet, "ambientdir"); if (s != worldTileEntityLightScale) events.push_back("badscale");
}
void lightResetOptionalParams__Fv() { events.push_back("reset"); }
void animLoadBoneMatricies__FP6PacketP14AnimationState(unsigned char* packet, unsigned char*) {
    ++boneCalls; events.push_back("bones"); sw_(packet, hw(packet) + 16);
}
void modelDrawCharacter__FP6PacketP12_modelHeaderli(unsigned char* packet, unsigned char* model, long a, int b) {
    events.push_back("draw"); drawModel = model; drawA = a; drawB = b; sw_(packet, hw(packet) + drawAdvance);
}
void drawAddToList__FPviP8_textureP11_drawRecordi(void* list, int kind, unsigned char* tex, unsigned char* rec, int extra) {
    addList = list; addKind = kind; addTex = tex; addRec = rec; addExtra = extra;
}
void drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(unsigned char* world, unsigned char* ph, int layer);
}
static unsigned A(const unsigned char* p) { return norrath_host_address(p); }
int main() {
    unsigned char* world = arena_at(0x100);
    unsigned char* ph = arena_at(0x200);
    unsigned char* h = arena_at(0x400);
    unsigned char* model = arena_at(0x800);
    unsigned char* tex = arena_at(0xc00);
    unsigned char* hdr = arena_at(0x1000);
    unsigned char* g = arena_at(0x2000) + 4;
    unsigned char* state = arena_at(0x1400);
    float W[16];
    auto arm = [&] {
        events.clear(); lightAdvance = 16; drawAdvance = 32; boneCalls = 0; lumpCalls = 0; animAdds = 0;
        std::memset(norrath_arena + 0x100, 0, 0x4000); std::memset(worldAnimation, 0, sizeof worldAnimation);
        std::strcpy((char*)h, "stone"); std::strcpy((char*)h + 80, "rock");
        sw_(ph, A(h)); sw_(h + 160, A(model)); sw_(h + 164, A(tex)); sw_(h + 168, 0); sw_(h + 172, 0);
        sw_(ph + 36, 0); sw_(ph + 52, 0); sw_(ph + 40, (unsigned)-8); sw_(ph + 44, 40); sw_(ph + 48, 4);
        sh_(ph + 54, 0); sh_(ph + 56, 0); sh_(model + 16, 0);
        for (int i = 0; i < 16; ++i) { W[i] = 0.5f * i - 3.0f + (i % 3); worldCharViewMatrix[i] = W[i]; }
        worldTileEntityLightScale = 1.5f;
        for (int i = 0; i < 8; ++i) model[24 + i] = 0xa0 + i;
        std::memset(g - 4, 0xee, 0x800);
        glistp = g;
    };
    unsigned char* c = g + 12;
    while ((A(c) & 15) != 12) ++c;
    unsigned char* t4 = c + 4;
    // Flag path A, rows negated both: ph flag 1, bits 0x30000 and row choice 0.
    arm(); sw_(ph + 36, 1); sw_(ph + 52, 0x30000); sw_(h + 172, 1);
    drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, ph, 5);
    if (hw(g + 8) != 0x11000000u || hw(c) != 0x50000005u) return 1;
    if (dword(t4) != (0x1000000000008000ull | 4) || dword(t4 + 8) != 14 || dword(t4 + 24) != 63 || dword(t4 + 40) != 6) return 2;
    if (dword(t4 + 48) != 0xa7a6a5a4a3a2a1a0ull || dword(t4 + 56) != 66 || dword(t4 + 64) != 0x507f5 || dword(t4 + 72) != 71 || hw(t4 + 80) != 0x6c04c000u) return 3;
    for (int i = 0; i < 4; ++i) if (lf_(t4 + 84 + 4 * i) != -W[i] || lf_(t4 + 100 + 4 * i) != -W[4 + i]) return 4;
    for (int i = 0; i < 4; ++i) if (lf_(t4 + 116 + 4 * i) != W[8 + i]) return 5;
    const float P[3] = {-2.0f, 10.0f, 1.0f};
    for (int k = 0; k < 4; ++k) {
        const double want = (double)W[12 + k] + W[k] * P[0] + W[4 + k] * P[1] + W[8 + k] * P[2];
        if (std::fabs(lf_(t4 + 132 + 4 * k) - want) > 1e-3) return 6;
    }
    if (hw(t4 + 148) != (5u | 0x04000000u) || hw(t4 + 152) != 0x14000002u) return 7;
    if (events.size() < 2 || events[0] != "character" || cursorAtLight != A(t4 + 156)) return 8;
    // Light block advanced 16, padded to 16-byte multiple; the tag at g covers everything up to the pad.
    unsigned char* e = t4 + 156 + 16;
    while ((A(e) & 15) != 0) { if (*e != 0) return 9; ++e; }
    if (dword(g) != ((((unsigned long long)(e - g) >> 3) - 2) >> 1 | 0x10000000ull)) return 10;
    // The head quadword's type 1 is changed to 6 once the model has been drawn.
    if (dword(e) != 0x60000000ull || dword(e + 8) != 0x1400001eull) return 11;
    if (drawModel != model || drawA != -1 || drawB != 0 || events.back() != "draw") return 12;
    if (glistp != e + 16 + 32) return 13;
    if (addList != g || addTex != tex || addRec != nullptr || addExtra != 0 || addKind != 0) return 14;
    // Path A positive rows with row choice 1, flag 0x20000 negating the second.
    arm(); sw_(ph + 36, 1); sw_(ph + 52, 0x40000 | 0x20000); sw_(h + 172, 2);
    drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    for (int i = 0; i < 4; ++i) if (lf_(t4 + 84 + 4 * i) != W[4 + i] || lf_(t4 + 100 + 4 * i) != -W[i]) return 15;
    if (events[0] != "nondir") return 16;
    // Path B: halfwords a = 16384 (54) and b = 8192 (56) scaled by 0x1.0002p-15.
    arm(); sh_(ph + 54, 16384); sh_(ph + 56, 8192); sw_(h + 172, 8);
    drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    {
        const double s = 1.000030517578125 / 32768.0, a = 16384 * s, b = 8192 * s;
        for (int i = 0; i < 4; ++i) {
            if (std::fabs(lf_(t4 + 84 + 4 * i) - (a * W[i] + b * W[4 + i])) > 1e-3) return 17;
            if (std::fabs(lf_(t4 + 100 + 4 * i) - (-b * W[i] + a * W[4 + i])) > 1e-3) return 18;
        }
    }
    if (events[0] != "ambient" || events[1] != "ambientonly" || events[2] != "reset") return 19;
    arm(); sh_(ph + 54, 16384); sh_(ph + 56, 8192); sw_(ph + 36, 0x40);
    drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    {
        const double s = 1.000030517578125 / 32768.0, a = 16384 * s, b = 8192 * s;
        for (int i = 0; i < 4; ++i) if (std::fabs(lf_(t4 + 100 + 4 * i) - (b * W[i] - a * W[4 + i])) > 1e-3) return 20;
    }
    if (events[0] != "ambientdir") return 21;
    // Volumetric lighting bit first; model flag bit 3 gives kind 4.
    arm(); sw_(h + 172, 4 | 1); sh_(model + 16, 8);
    drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (events[0] != "disable" || events[1] != "character" || events[2] != "reset" || addKind != 4) return 22;
    // Resources: names, the underscore cut, flag 0x10 and the animation search.
    arm(); sw_(h + 160, 0); sw_(h + 164, 0); std::strcpy((char*)h + 80, "tree_big");
    for (int k = 0; k < 3; ++k) { worldAnimation[48 * k + 4] = 1; sw_(worldAnimation + 48 * k + 8, 0x700 + k); }
    drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (lumpCalls != 3 || lumpSeen[0] != "stone.lmp|tree.vif" || lumpSeen[1] != "stone.lmp|tree.tex" || lumpSeen[2] != "stone.lmp|tree.anm") return 23;
    if ((hw(h + 172) & 0x10) == 0 || hw(h + 160) != A(arena_at(0x3000)) || hw(h + 164) != A(arena_at(0x3100)) || hw(h + 168) != A(arena_at(0x3200))) return 24;
    if (animAdds != 1 || animEntries[0] != worldAnimation + 48 * 3 || hw(ph + 4) != A(worldAnimation + 48 * 3) || boneCalls != 1) return 25;
    // An entry that already holds this header is used without animAdd; ph+4 set wins afterwards.
    arm(); sw_(h + 168, A(hdr));
    worldAnimation[48 * 0 + 4] = 1; sw_(worldAnimation, 0); sw_(worldAnimation + 8, 0x1234);
    worldAnimation[48 * 1 + 4] = 1; sw_(worldAnimation + 48 + 8, A(hdr));
    drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (animAdds != 0 || hw(ph + 4) != A(worldAnimation + 48) || boneCalls != 1) return 26;
    arm(); sw_(h + 168, A(hdr)); sw_(ph + 4, A(state)); drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (hw(ph + 4) != A(state) || animAdds != 0) return 27;
    // No animation header: no search, no bone call.
    arm(); drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (boneCalls != 0 || hw(ph + 4) != 0) return 28;
    // All 64 entries in use with other headers: ph+4 stays zero.
    arm(); sw_(h + 168, A(hdr));
    for (int k = 0; k < 64; ++k) { worldAnimation[48 * k + 4] = 1; sw_(worldAnimation + 48 * k + 8, 0x700 + k); }
    drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (hw(ph + 4) != 0 || boneCalls != 0 || animAdds != 0) return 29;
    return 0;
}
