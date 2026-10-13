// Host contract for clone__7AntWave; not an EE input. Includes the source.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_10GameObject[8], vt_8Creature[8], vt_7AntWave[8];
static unsigned char arena[1040] __attribute__((aligned(16)));
static unsigned char src[1040] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned wordAt72AtCall[16];
static int callCount;
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
static void note() { std::memcpy(&wordAt72AtCall[callCount++], arena + 72, 4); }
void ListHead_copy(void* d, const void* s) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    note(); std::memset(d, 0, 4);
}
void AnimationState8_copy(void* d, const void* s) {
    calls += "an" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    note(); std::memcpy(d, s, 264);
}
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
int main() {
    int bad = 0;
    for (int i = 0; i < 1040; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    std::memset(arena, 0xa5, sizeof arena);
    failNext = true;
    if (clone__7AntWave(src) != nullptr || calls != "alloc976;") { std::puts("fail: null allocation"); bad = 1; }
    failNext = false; calls.clear();
    void* r = clone__7AntWave(src);
    if (r != arena) { std::puts("fail: returns the new object"); bad = 1; }
    if (calls != "alloc976;lh0<0;lh4<4;lh8<8;an288<288;lh576<576;lh752<752;lh888<888;lh968<968;") { std::puts(calls.c_str()); std::puts("fail: call order"); bad = 1; }
    if (wordAt72AtCall[0] != lo(vt_10GameObject)) { std::puts("fail: table word at call 0"); bad = 1; }
    if (wordAt72AtCall[1] != lo(vt_10GameObject)) { std::puts("fail: table word at call 1"); bad = 1; }
    if (wordAt72AtCall[2] != lo(vt_10GameObject)) { std::puts("fail: table word at call 2"); bad = 1; }
    if (wordAt72AtCall[3] != lo(vt_8Creature)) { std::puts("fail: table word at call 3"); bad = 1; }
    if (wordAt72AtCall[4] != lo(vt_8Creature)) { std::puts("fail: table word at call 4"); bad = 1; }
    if (wordAt72AtCall[5] != lo(vt_8Creature)) { std::puts("fail: table word at call 5"); bad = 1; }
    if (wordAt72AtCall[6] != lo(vt_8Creature)) { std::puts("fail: table word at call 6"); bad = 1; }
    if (wordAt72AtCall[7] != lo(vt_7AntWave)) { std::puts("fail: table word at call 7"); bad = 1; }
    unsigned w72; std::memcpy(&w72, arena + 72, 4);
    if (w72 != lo(vt_7AntWave)) { std::puts("fail: final table pointer"); bad = 1; }
    const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{76,76},{80,85},{88,156},{160,173},{176,189},{192,205},{208,233},{236,285},{552,570},{572,573},{580,749},{756,800},{804,887},{892,967}};
    static bool copied[1040];
    for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
    for (unsigned i = 0; i < 1040; ++i) {
        bool head = (i >= 0 && i < 4) || (i >= 4 && i < 8) || (i >= 8 && i < 12) || (i >= 288 && i < 552) || (i >= 576 && i < 580) || (i >= 752 && i < 756) || (i >= 888 && i < 892) || (i >= 968 && i < 972);
        if (i >= 72 && i < 76) continue;
        if (copied[i] && arena[i] != src[i]) { std::printf("fail: byte %u not copied\n", i); bad = 1; break; }
        if (!copied[i] && !head && arena[i] != 0xa5) { std::printf("fail: byte %u written but not in a copy range\n", i); bad = 1; break; }
    }
    return bad;
}
