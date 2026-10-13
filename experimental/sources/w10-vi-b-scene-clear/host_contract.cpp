#define NORRATH_HOST_CONTRACT
#include "scene_clear.cpp"

#include <cstring>
#include <map>
#include <string>
#include <vector>

// Fake 32-bit address space: each region maps a target address to host bytes.
struct Region {
    unsigned base;
    std::vector<unsigned char> bytes;
};
static std::vector<Region*> regions;
static Region* make_region(unsigned base, unsigned size) {
    Region* r = new Region{base, std::vector<unsigned char>(size, 0)};
    regions.push_back(r);
    return r;
}
extern "C" char* norrath_host_translate(unsigned address) {
    for (Region* r : regions)
        if (address >= r->base && address < r->base + r->bytes.size())
            return reinterpret_cast<char*>(r->bytes.data()) + (address - r->base);
    return nullptr;
}

static std::string trace;
static unsigned char* scene_bytes;
static int fail_at;      // 1-based index among checked steps, 0 for none
static int checked_seen;

extern "C" void norrath_host_call_destructor(unsigned function, char* object, int flags) {
    trace += "vdtor" + std::to_string(function) + "@" + std::to_string(object - reinterpret_cast<char*>(norrath_host_translate(0x30000000u))) +
             "," + std::to_string(flags) + ";";
}

static std::string at(const void* p) {
    return std::to_string(static_cast<const unsigned char*>(p) - scene_bytes);
}
static int checked(const char* name, const void* p, const std::string& extra = "") {
    trace += std::string(name) + "@" + at(p) + extra + ";";
    return ++checked_seen == fail_at ? -1 : 0;
}

extern "C" {
void GlobalFree__14VIEffectVolume() { trace += "gfree;"; }
int Clear__19VIRadialFloraSystem(Member* m) { trace += "flora@" + at(m) + ";"; return -9; }
int Clear__17VISceneSoundTrack(Member* m) { trace += "track@" + at(m) + ";"; return -9; }
void VITrailFxClear__FR8VIRasterR12VIDictionary(void* raster, void* dictionary) {
    trace += "trail" + std::to_string(reinterpret_cast<unsigned long>(raster)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(dictionary)) + ";";
}
int Clear__19VISpellEffectSystem(Member* m) { trace += "spell@" + at(m) + ";"; return -9; }
int Clear__12VIAtmosphereR7VIScene(Member* m, VIScene* s) {
    trace += "atmo@" + at(m) + "," + at(s) + ";";
    return -9;
}
int Clear__16VIParticleSystemR12VIDictionary(Member* m, void* d) {
    return checked("particles", m, "," + std::to_string(reinterpret_cast<unsigned long>(d)));
}
int Clear__t6VIList1Z7VIActor(Member* m) { return checked("actors", m); }
int Clear__t6VIList1ZQ27VIScene12VISceneOccup(Member* m) { return checked("occup", m); }

int Begin__Ct6VIPool1ZP8VISprite(const Member* p) { trace += "bs;"; return 0; }
int Next__Ct6VIPool1ZP8VISpritei(const Member* p, int i) { return i + 1; }
int End__Ct6VIPool1ZP8VISprite(const Member* p) { return *reinterpret_cast<const int*>(reinterpret_cast<const char*>(p) + 4); }
int Clear__t6VIPool1ZP8VISprite(Member* m) { return checked("spritepool", m); }
int Begin__Ct6VIPool1ZP16VIStaticLighting(const Member*) { trace += "bl;"; return 0; }
int Next__Ct6VIPool1ZP16VIStaticLightingi(const Member*, int i) { return i + 1; }
int End__Ct6VIPool1ZP16VIStaticLighting(const Member* p) { return *reinterpret_cast<const int*>(reinterpret_cast<const char*>(p) + 4); }
int Clear__t6VIPool1ZP16VIStaticLighting(Member* m) { return checked("lightpool", m); }
int Begin__Ct6VIPool1ZP8VIRefMap(const Member*) { trace += "bm;"; return 0; }
int Next__Ct6VIPool1ZP8VIRefMapi(const Member*, int i) { return i + 1; }
int End__Ct6VIPool1ZP8VIRefMap(const Member* p) { return *reinterpret_cast<const int*>(reinterpret_cast<const char*>(p) + 4); }
int Clear__t6VIPool1ZP8VIRefMap(Member* m) { return checked("mappool", m); }
int Begin__Ct6VIPool1ZP13VIHSpriteAnim(const Member*) { trace += "ba;"; return 0; }
int Next__Ct6VIPool1ZP13VIHSpriteAnimi(const Member*, int i) { return i + 1; }
int End__Ct6VIPool1ZP13VIHSpriteAnim(const Member* p) { return *reinterpret_cast<const int*>(reinterpret_cast<const char*>(p) + 4); }
int Clear__t6VIPool1ZP13VIHSpriteAnim(Member* m) { return checked("animpool", m); }

void _$_t7VIArray1Zi(Member* m, int flags) {
    trace += "arrdtor" + std::to_string(reinterpret_cast<char*>(m) - norrath_host_translate(0x30000000u)) + "," + std::to_string(flags) + ";";
}
void __builtin_delete(void* p) {
    trace += "delete" + std::to_string(static_cast<char*>(p) - norrath_host_translate(0x30000000u)) + ";";
}
void _$_t5VIMap2ZiZi(Member* m, int flags) {
    trace += "mapdtor" + std::to_string(reinterpret_cast<char*>(m) - norrath_host_translate(0x30000000u)) + "," + std::to_string(flags) + ";";
}
void _$_13VIHSpriteAnim(void* p, int flags) {
    trace += "animdtor" + std::to_string(static_cast<char*>(p) - norrath_host_translate(0x30000000u)) + "," + std::to_string(flags) + ";";
}
int Clear__t8VIVector1Z18VISceneDisplayElem(Member* m) { return checked("display", m); }
int Clear__11VIClipStack(Member* m) { return checked("clipstack", m); }
int Clear__14VIQueryTriList(Member* m) { return checked("querytri", m); }
int Clear__t7VIArray1Zi(Member* m) { return checked("array", m); }
int Clear__7VIWorld(Member* m) { return checked("world", m); }
}

// Heap objects (sprites, lights, maps, anims) live in one region at 0x30000000.
static const unsigned kHeap = 0x30000000u;
static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static unsigned get32(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

static const int kSize = 19400;
static unsigned char scene[kSize];
static unsigned char saved[kSize];

static void build(unsigned raster) {
    regions.clear();
    std::memset(scene, 0x5a, sizeof scene);
    scene_bytes = scene;
    Region* heap = make_region(kHeap, 0x2000);
    unsigned char* h = heap->bytes.data();
    // Pool arrays: 12-byte entries, first word is the pointer.
    Region* arrays = make_region(0x31000000u, 0x2000);
    unsigned char* a = arrays->bytes.data();
    // Vtable: slot 0 at +8: short adjust 16, function 0x7000.
    Region* vt = make_region(0x32000000u, 0x100);
    short adjust = 16;
    std::memcpy(vt->bytes.data() + 8, &adjust, 2);
    put32(vt->bytes.data() + 12, 0x7000u);

    struct PoolSpec { int off; unsigned array; std::vector<unsigned> entries; };
    std::vector<PoolSpec> pools = {
        {996, 0x31000000u, {kHeap + 0x000, 0, kHeap + 0x100}},      // sprites
        {1068, 0x31000100u, {kHeap + 0x200, kHeap + 0x300}},       // lights
        {1044, 0x31000200u, {0, kHeap + 0x400}},                    // maps
        {1020, 0x31000300u, {kHeap + 0x500, 0, kHeap + 0x600}},     // anims
    };
    for (auto& p : pools) {
        put32(scene + p.off + 8, p.array);
        put32(scene + p.off + 4, p.entries.size());
        for (size_t i = 0; i < p.entries.size(); ++i) {
            put32(a + (p.array - 0x31000000u) + 12 * i, p.entries[i]);
            put32(a + (p.array - 0x31000000u) + 12 * i + 4, 0xdeadbeef);
        }
    }
    put32(h + 0x000 + 60, 0x32000000u);
    put32(h + 0x100 + 60, 0x32000000u);
    put32(scene + 19352, 0x4444);
    put32(scene + 19356, raster);
    put32(scene + 19360, 0x6666);
    put32(scene + 19364, 0x7777);
    put32(scene + 19368, 0x8888);
    std::memcpy(saved, scene, sizeof scene);
    trace.clear();
    checked_seen = 0;
}

static std::string full_trace(bool raster) {
    // Heap objects are at kHeap + n; the helper prints object - heap base.
    std::string t = "gfree;flora@10876;track@6800;";
    if (raster) t += "trail" + std::to_string(raster ? 0x5555 : 0) + ",17476;";
    t += "spell@7024;";
    if (raster) t += "atmo@1600,0;";
    t += "particles@1352,17476;actors@972;occup@984;";
    t += "bs;vdtor28672@16,3;vdtor28672@272,3;spritepool@996;";  // adjust 16 added to 0x000 and 0x100
    t += "bl;arrdtor520,2;delete512;arrdtor776,2;delete768;lightpool@1068;";
    t += "bm;mapdtor1032,2;delete1024;mappool@1044;";
    t += "ba;animdtor1280,3;animdtor1536,3;animpool@1020;";
    t += "display@1112;clipstack@1296;querytri@6128;array@1104;world@4;";
    return t;
}

int main() {
    // The scene object is a host buffer, so its target "address" is only used
    // through offsets; raster/dictionary words are plain handles here.
    build(0x5555);
    // trace uses offsets relative to the scene for scene members and relative
    // to the heap region for heap objects.
    int result = Clear__7VIScene(reinterpret_cast<VIScene*>(scene));
    if (result != 0) return 1;
    std::string want = full_trace(true);
    // Atmosphere receives the scene itself, offset 0.
    if (trace != want) return 2;
    // Success stores.
    auto word = [&](int off) { return get32(scene + off); };
    if (word(19352) != 0 || word(19356) != 0 || word(19360) != 0 || word(19364) != 0) return 3;
    if (word(0) != 0 || word(19368) != 0 || word(10772) != 0 || word(10816) != 0) return 4;
    if (word(6268) != 0x451c4000u || word(6276) != 0xffffffffu || word(1124) != 0xffffffffu ||
        word(1128) != 0xffffffffu || word(1132) != 0xffffffffu || word(1136) != 0xffffffffu ||
        word(1140) != 0xffffffffu || word(1144) != 0xffffffffu || word(6272) != 0xffffffffu ||
        word(1092) != 0xffffffffu || word(1096) != 0xffffffffu)
        return 5;
    if (word(1152) != 0 || word(1156) != 0 || word(10808) != 0 || word(10812) != 0) return 6;
    if (word(6792) != 0) return 7;

    // No raster: trail and atmosphere skipped.
    build(0);
    if (Clear__7VIScene(reinterpret_cast<VIScene*>(scene)) != 0) return 8;
    if (trace != full_trace(false)) return 9;

    // Each checked step failing returns -1, runs nothing after it and leaves
    // the final four words alone. Checked steps in order:
    const char* order[] = {"particles", "actors", "occup", "spritepool", "lightpool", "mappool",
                           "animpool", "display", "clipstack", "querytri", "array", "world"};
    std::string full = full_trace(true);
    for (int k = 1; k <= 12; ++k) {
        build(0x5555);
        fail_at = k;
        int r = Clear__7VIScene(reinterpret_cast<VIScene*>(scene));
        fail_at = 0;
        if (r != -1) return 100 + k;
        // Expected trace: prefix of the full trace through the failing call.
        std::string needle = std::string(order[k - 1]) + "@";
        size_t at_pos = full.find(needle);
        size_t end = full.find(';', at_pos);
        if (trace != full.substr(0, end + 1)) return 200 + k;
        if (word(19352) != 0x4444 || word(19356) != 0x5555 || word(19360) != 0x6666 || word(19364) != 0x7777) return 300 + k;
        bool maps_started = k >= 6;
        if ((word(1092) == 0xffffffffu) != maps_started) return 400 + k;
        if ((word(6792) == 0) != (k >= 11)) return 500 + k;
    }
    return 0;
}
