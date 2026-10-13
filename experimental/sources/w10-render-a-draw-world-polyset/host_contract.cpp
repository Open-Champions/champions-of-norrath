// Host contract for drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi. Not an EE plan input.
#define NORRATH_HOST_ARENA 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> events;
static int pausedResult;
extern "C" {
unsigned char textureUncompBufferLocked, worldLastDrawSucceded;
signed char texturePageTable[64];
unsigned char* texturePageLoc[17];
unsigned char* glistp;
int levelFlags, frameNm;
float worldWorldViewMatrix[16], worldCameraMatrix[12];
}
#include "draw_world_polyset.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" {
void drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(unsigned char*, unsigned char*, int l) { events.push_back("water " + std::to_string(l)); }
void drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(unsigned char*, unsigned char*, int l) { events.push_back("object " + std::to_string(l)); }
int gameIsPaused__Fv() { return pausedResult; }
void drawWorldParticleEmitter__FP13PolysetHeader(unsigned char*) { events.push_back("emitter"); }
void drawWorldDynamicLight__FP13PolysetHeader(unsigned char*) { events.push_back("dynamic"); }
void texCacheData__FP8_texturei(unsigned char*, int id) { events.push_back("cache " + std::to_string(id)); }
void lightSetupAmbientWorld__FP6Packeti(unsigned char* packet, int amount) {
    events.push_back("ambient " + std::to_string(amount));
    unsigned char* cursor = norrath_lal::load_pointer(packet);
    norrath_lal::store_word(cursor, 0xabcd);
    norrath_lal::store_pointer(packet, cursor + 4);
}
int lightSetupForWorld__FP6PacketP13PolysetHeader(unsigned char*, unsigned char*) { events.push_back("lights"); return 1; }
void drawAddToList__FPviP8_textureP11_drawRecordi(void* list, int kind, unsigned char* texture, unsigned char*, int) {
    events.push_back("add " + std::to_string(kind) + (texture ? " tex" : " none") + (list == arena_at(0x1000) ? " list" : " ?"));
}
}
int main() {
    unsigned char* world = arena_at(0x10);
    unsigned char* polyset = arena_at(0x100);
    unsigned char* page = arena_at(0x2000);
    unsigned char* list = arena_at(0x1000);
    auto reset = [&] {
        events.clear();
        std::memset(polyset, 0, 0x80);
        std::memset(list, 0xdd, 0x400);
        std::memset(page, 0, 0x200);
        sh_(polyset + 52, 3);
        texturePageTable[3] = 1;
        texturePageLoc[1] = page;
        sw_(polyset + 32, 0x40);
        sw_(page + 0x40 + 20, 1);
        glistp = list;
        levelFlags = 0;
        textureUncompBufferLocked = 0;
        pausedResult = 0;
        for (int i = 0; i != 16; ++i) worldWorldViewMatrix[i] = static_cast<float>(i + 1);
        for (int i = 0; i != 12; ++i) worldCameraMatrix[i] = static_cast<float>(100 + i);
    };
    reset(); sw_(polyset + 36, 0x80);
    drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(world, polyset, 9);
    if (events.size() != 1 || events[0] != "water 9") return 1;
    reset(); sw_(polyset + 36, 0x800);
    drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(world, polyset, 9);
    if (events.size() != 1 || events[0] != "object 9") return 2;
    reset(); sw_(polyset + 36, 0x4000);
    drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(world, polyset, 9);
    if (events.size() != 1 || events[0] != "emitter") return 3;
    reset(); sw_(polyset + 36, 0x8000); pausedResult = 1; textureUncompBufferLocked = 1;
    drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(world, polyset, 9);
    if (!events.empty() || hw(list + 8) != 0xdddddddd) return 4;
    reset(); sh_(polyset + 52, 0);
    drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(world, polyset, 9);
    if (!events.empty()) return 5;
    reset(); texturePageTable[3] = -1; worldLastDrawSucceded = 7;
    drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(world, polyset, 9);
    if (!events.empty() || worldLastDrawSucceded != 0) return 6;
    reset(); sw_(page + 0x40 + 20, 0); worldLastDrawSucceded = 7;
    drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(world, polyset, 9);
    if (events.size() != 1 || events[0] != "cache 3" || worldLastDrawSucceded != 0) return 7;
    // A full draw: flags 0x200 (ambient 0), no flag 0x10, no flag 0x1.
    reset(); sw_(polyset + 36, 0x200); sw_(polyset + 0, 5); sw_(polyset + 4, 6);
    sh_(polyset + 54, 1); sh_(polyset + 56, 2);
    drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(world, polyset, 9);
    if (events.size() != 2 || events[0] != "ambient 0" || events[1] != "add 4 tex list") { for (auto& e : events) std::puts(e.c_str()); return 8; }
    if (hw(list + 8) != 0x11000000u) return 9;
    // The arena offset 0x1000 is 16-byte aligned, so no padding: the cursor starts at list + 12.
    if (hw(list + 12) != 0x50000005u) return 10;
    // Group tag at list + 16: low field 0x8000 plus the quadword count, high word 0x10000000.
    if ((hw(list + 16) & 0xffff8000u) != 0x8000u || hw(list + 20) != 0x10000000u || hw(list + 24) != 14) return 11;
    if (hw(list + 32) != 0 || hw(list + 40) != 63 || hw(list + 48) != 0 || hw(list + 56) != 6) return 12;
    if (hw(list + 64) != 1 || hw(list + 72) != 66) return 13;           // flag 0x200 selects (1, 66)
    if (hw(list + 80) != 0x587f4u || hw(list + 88) != 71) return 14;
    if (hw(list + 96) != (0x04000000u | 9u)) return 15;
    // 0x200 flag, no 0x10 flag: word 4 holds ((0) + 4) << 16 combined with the constants.
    if (hw(list + 100) != ((4u << 16) | 0x60000000u | 0x0c00c000u)) return 16;
    // Without flag 0x1: two weighted matrix combinations of rows 0 and 1; a = 1 * k, b = 2 * k.
    const float k = 0x1.0002p-15f;
    const float a = 1.0f * k, b = 2.0f * k;
    const float* m = worldWorldViewMatrix;
    if (lf_(list + 104) != a * m[0] + b * m[4]) return 17;
    if (lf_(list + 108) != a * m[1] + b * m[5]) return 18;
    if (lf_(list + 120) != -b * m[0] + a * m[4]) return 19;
    if (lf_(list + 136) != m[8] || lf_(list + 164) != m[15]) return 20;
    if (hw(list + 168) != 0x14000016u || hw(list + 172) != 0xabcd) return 21;
    std::puts("drawWorldPolyset contract passed");
    return 0;
}
