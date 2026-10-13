// Host contract for clone__3Orc; not an EE input. Includes the source. Checks the null
// allocation path, the call order, the table pointers seen by each constructor call, the
// final table pointer, and that exactly the stated byte ranges are copied.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_GameObject[8], vt_Creature[8], vt_Orc[8];
static unsigned char arena[1200] __attribute__((aligned(16)));
static unsigned char src[1144] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned wordAt72AtCall[16];
static int callCount;
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
void ListHead_copy(void* d, const void* s) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    std::memcpy(&wordAt72AtCall[callCount++], arena + 72, 4); std::memset(d, 0, 4);
}
void AnimationState8_copy(void* d, const void* s) {
    calls += "an" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    std::memcpy(&wordAt72AtCall[callCount++], arena + 72, 4);
}
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
int main() {
    int bad = 0;
    for (int i = 0; i < 1144; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    std::memset(arena, 0xa5, sizeof arena);
    failNext = true;
    if (clone__3Orc(src) != nullptr || calls != "alloc1144;") { std::puts("fail: null allocation"); bad = 1; }
    failNext = false; calls.clear();
    if (clone__3Orc(src) != arena) { std::puts("fail: result"); bad = 1; }
    if (calls != "alloc1144;lh0<0;lh4<4;lh8<8;an288<288;lh576<576;lh752<752;lh888<888;lh1128<1128;") { std::puts(calls.c_str()); bad = 1; }
    for (int i = 0; i < 3; ++i) if (wordAt72AtCall[i] != lo(vt_GameObject)) { std::puts("fail: GameObject table before heads"); bad = 1; }
    for (int i = 3; i < 7; ++i) if (wordAt72AtCall[i] != lo(vt_Creature)) { std::puts("fail: Creature table before later calls"); bad = 1; }
    if (wordAt72AtCall[7] != lo(vt_Orc)) { std::puts("fail: Orc table before the last head"); bad = 1; }
    unsigned w72; std::memcpy(&w72, arena + 72, 4);
    if (w72 != lo(vt_Orc)) { std::puts("fail: final table pointer"); bad = 1; }
    const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{76,76},{80,85},{88,156},{160,173},{176,189},{192,205},{208,233},{236,285},
                                  {552,570},{572,573},{580,749},{756,800},{804,887},{892,959},{960,1127},{1132,1132},{1136,1139}};
    bool copied[1200] = {};
    for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
    for (unsigned i = 0; i < 1200; ++i) {
        const bool head = i < 12 || (i >= 576 && i < 580) || (i >= 752 && i < 756) || (i >= 888 && i < 892) || (i >= 1128 && i < 1132);
        if (i >= 72 && i < 76) continue;
        if (head) continue;
        if (i >= 288 && i < 552) continue;  // owned by the AnimationState8 constructor
        if (copied[i] && arena[i] != src[i]) { std::printf("fail: byte %u not copied\n", i); bad = 1; break; }
        if (!copied[i] && arena[i] != 0xa5) { std::printf("fail: byte %u written outside the ranges\n", i); bad = 1; break; }
    }
    return bad;
}
