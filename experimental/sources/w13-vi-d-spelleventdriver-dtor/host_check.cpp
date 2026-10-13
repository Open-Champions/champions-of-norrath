// Host check only; not a build input. Object words hold arena offsets (see NORRATH_HOST_ARENA); stubs record the call order with
// arena offsets of their pointer arguments.
#define NORRATH_HOST_ARENA 1
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(16) static unsigned char arena[0x4000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::string trace;
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - arena); }
static unsigned queue76[4], queue88[4];
static int head76, head88;
extern "C" {
void* RemHead__7VILList(void* list) {
    unsigned* q = list == arena + 0x100 + 76 ? queue76 : queue88;
    int& h = list == arena + 0x100 + 76 ? head76 : head88;
    trace += "rem:" + off(list) + ";";
    unsigned v = q[h];
    if (v != 0) ++h;
    return v ? arena + v : nullptr;
}
void ReleaseActor__7VIScenei(void* scene, int actor) { trace += "rel:" + off(scene) + "," + std::to_string(actor) + ";"; }
void _$_17VIParticleEmitter(void* p, int f) { trace += "pe:" + off(p) + "," + std::to_string(f) + ";"; }
void _$_14VIMotifBlender(void* p, int f) { trace += "mb:" + off(p) + "," + std::to_string(f) + ";"; }
void _$_11VILightning(void* p, int f) { trace += "lt:" + off(p) + "," + std::to_string(f) + ";"; }
void _$_7VILList(void* p, int f) { trace += "ll:" + off(p) + "," + std::to_string(f) + ";"; }
void __builtin_delete(void* p) { trace += "del:" + off(p) + ";"; }
}

static void set(unsigned base, int word, unsigned v) { std::memcpy(arena + base + 4 * word, &v, 4); }
// One record: scene, actor ids at 140/144, emitters at 120/124, blender at 128 (word indices 4, 35, 36, 30, 31, 32).
static void record(unsigned base, unsigned scene, unsigned a, unsigned b, unsigned e1, unsigned e2, unsigned mb) {
    set(base, 4, scene); set(base, 35, a); set(base, 36, b); set(base, 30, e1); set(base, 31, e2); set(base, 32, mb);
}
static std::string expectRecord(unsigned base, unsigned scene, int a, int b, unsigned e1, unsigned e2, unsigned mb) {
    std::string s;
    if (a != -1) s += "rel:" + std::to_string(scene) + "," + std::to_string(a) + ";";
    if (b != -1) s += "rel:" + std::to_string(scene) + "," + std::to_string(b) + ";";
    if (e1) s += "pe:" + std::to_string(e1) + ",3;";
    if (e2) s += "pe:" + std::to_string(e2) + ",3;";
    if (mb) s += "mb:" + std::to_string(mb) + ",3;";
    return s + "del:" + std::to_string(base) + ";";
}

int main() {
    bool ok = true;
    for (int flags : {0, 1, 2, 3, -1, -2}) {
        std::memset(arena, 0x5a, sizeof arena);
        trace.clear();
        head76 = head88 = 0;
        const unsigned self = 0x100;
        set(self, 8, 0x1000); set(self, 9, 0x1200); set(self, 10, 0x1400); set(self, 11, 0);
        record(0x800, 0x2000, 5, 0xffffffffu, 0x2100, 0, 0x2200);
        record(0x900, 0x2010, 0xffffffffu, 7, 0, 0x2110, 0);
        record(0xa00, 0x2020, 8, 9, 0x2120, 0x2130, 0x2230);
        record(0x1000, 0x2030, 1, 2, 0, 0, 0);
        record(0x1200, 0x2040, 0xffffffffu, 0xffffffffu, 0x2140, 0x2150, 0x2240);
        queue76[0] = 0x800; queue76[1] = 0x900; queue76[2] = 0; queue88[0] = 0xa00; queue88[1] = 0;
        unsigned selfObj = self;
        _$_18VISpellEventDriver(arena + selfObj, flags);
        std::string want = "rem:" + std::to_string(self + 76) + ";" + expectRecord(0x800, 0x2000, 5, -1, 0x2100, 0, 0x2200);
        want += "rem:" + std::to_string(self + 76) + ";" + expectRecord(0x900, 0x2010, -1, 7, 0, 0x2110, 0);
        want += "rem:" + std::to_string(self + 76) + ";";
        want += "rem:" + std::to_string(self + 88) + ";" + expectRecord(0xa00, 0x2020, 8, 9, 0x2120, 0x2130, 0x2230);
        want += "rem:" + std::to_string(self + 88) + ";";
        want += expectRecord(0x1000, 0x2030, 1, 2, 0, 0, 0);
        want += expectRecord(0x1200, 0x2040, -1, -1, 0x2140, 0x2150, 0x2240);
        want += "lt:5120,3;";
        want += "ll:" + std::to_string(self + 88) + ",2;ll:" + std::to_string(self + 76) + ",2;";
        if (flags & 1) want += "del:" + std::to_string(self) + ";";
        if (trace != want) { std::printf("flags %d\ngot  %s\nwant %s\n", flags, trace.c_str(), want.c_str()); ok = false; }
    }
    // Nothing queued and no single records: only the list teardown (and the optional delete) happens.
    std::memset(arena, 0, sizeof arena);
    trace.clear();
    head76 = head88 = 0; queue76[0] = queue88[0] = 0;
    _$_18VISpellEventDriver(arena + 0x100, 1);
    std::string want = "rem:" + std::to_string(0x100 + 76) + ";rem:" + std::to_string(0x100 + 88) + ";ll:" + std::to_string(0x100 + 88) +
                       ",2;ll:" + std::to_string(0x100 + 76) + ",2;del:256;";
    if (trace != want) { std::printf("empty got %s\n", trace.c_str()); ok = false; }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
