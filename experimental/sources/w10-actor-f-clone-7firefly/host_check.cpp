// Host contract for clone__7FireFly; not an EE input. Includes the source.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_GameObject[8], vt_Creature[8], norrath_ctors_ant_lantern_view_44bdb0[8];
static unsigned char arena[1144] __attribute__((aligned(16)));
static unsigned char src[1128] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned wordAt72AtCall[8];
static int callCount;
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
static void note() { std::memcpy(&wordAt72AtCall[callCount++], arena + 72, 4); }
void ListHead_copy(void* d, const void* s) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    note(); std::memset(d, 0, 4);
}
void AnimationState8_copy(void* d, const void* s) {
    calls += "an" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    note();
}
static int light_calls; static unsigned light_addr, light_words[2];
void lightAdd__FP5Light(void* l) { ++light_calls; calls += "light" + std::to_string(static_cast<unsigned char*>(l) - arena) + ";"; light_addr = static_cast<unsigned>(static_cast<unsigned char*>(l) - arena); std::memcpy(light_words, l, 8); }
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static int run_case(unsigned w0, unsigned w4, int& bad) {
    for (int i = 0; i < 1128; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    std::memcpy(src + 1064, &w0, 4); std::memcpy(src + 1068, &w4, 4);
    std::memset(arena, 0xa5, sizeof arena);
    calls.clear(); callCount = 0; light_calls = 0;
    void* r = clone__7FireFly(src);
    if (r != arena) { std::puts("fail: returns the new object"); bad = 1; }
    std::string want = "alloc1128;lh0<0;lh4<4;lh8<8;an288<288;lh576<576;lh752<752;lh888<888;";
    if (w0 != 0 || w4 != 0) want += "light1064;";
    if (calls != want) { std::puts(calls.c_str()); std::puts("fail: call order"); bad = 1; }
    for (int i = 0; i < 3; ++i) if (wordAt72AtCall[i] != lo(vt_GameObject)) { std::puts("fail: GameObject table before heads"); bad = 1; }
    for (int i = 3; i < callCount; ++i) if (wordAt72AtCall[i] != lo(vt_Creature) && !(false)) { std::puts("fail: Creature table before later calls"); bad = 1; }
    unsigned w72; std::memcpy(&w72, arena + 72, 4);
    if (w72 != lo(norrath_ctors_ant_lantern_view_44bdb0)) { std::puts("fail: final table pointer"); bad = 1; }
    const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{76,76},{80,85},{88,156},{160,173},{176,189},{192,205},{208,233},{236,285},{552,570},{572,573},{580,749},{756,800},{804,887},{892,987},{992,1063},{1064,1123}};
    bool copied[1144] = {};
    for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
    for (unsigned i = 0; i < 1144; ++i) {
        bool head = i < 12 || (i >= 576 && i < 580) || (i >= 752 && i < 756) || (i >= 888 && i < 892);
        bool table = i >= 72 && i < 76;
        bool lightfield = i >= 1064 && i < 1124 && (w0 != 0 || w4 != 0) && i < 1072;
        if (table) continue;
        if (lightfield) { if (i < 1072 && arena[i] != 0) { std::printf("fail: light word %u not cleared\n", i); bad = 1; break; } continue; }
        if (copied[i] && arena[i] != src[i]) { std::printf("fail: byte %u not copied\n", i); bad = 1; break; }
        if (!copied[i] && !head && arena[i] != 0xa5) { std::printf("fail: byte %u written but not in a copy range\n", i); bad = 1; break; }
    }
    return bad;
}
int main() {
    int bad = 0;
    for (int i = 0; i < 1128; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    std::memset(arena, 0xa5, sizeof arena);
    failNext = true; calls.clear();
    if (clone__7FireFly(src) != nullptr || calls != "alloc1128;") { std::puts("fail: null allocation"); bad = 1; }
    for (int i = 0; i < 16; ++i) if (arena[i] != 0xa5) { std::puts("fail: null allocation wrote"); bad = 1; break; }
    failNext = false;
    for (unsigned w0 : {0u, 7u}) for (unsigned w4 : {0u, 9u}) run_case(w0, w4, bad);
    return bad;
}
