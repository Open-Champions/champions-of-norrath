// Host contract for clone__8RollDoor; not an EE input. Includes the source. Checks the null
// allocation path, the call order, the table pointers seen by each constructor call, the
// final table pointer, and that exactly the stated byte ranges are copied.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_GameObject[8], vt_RollDoor[8];
static unsigned char arena[256] __attribute__((aligned(16)));
static unsigned char src[92] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned wordAt72AtCall[8];
static int callCount;
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
void ListHead_copy(void* d, const void* s) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    std::memcpy(&wordAt72AtCall[callCount++], arena + 72, 4); std::memset(d, 0, 4);
}
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
int main() {
    int bad = 0;
    for (int i = 0; i < 92; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    std::memset(arena, 0xa5, sizeof arena);
    failNext = true;
    if (clone__8RollDoor(src) != nullptr || calls != "alloc92;") { std::puts("fail: null allocation"); bad = 1; }
    failNext = false; calls.clear();
    if (clone__8RollDoor(src) != arena) { std::puts("fail: result"); bad = 1; }
    if (calls != "alloc92;lh0<0;lh4<4;lh8<8;") { std::puts(calls.c_str()); bad = 1; }
    for (int i = 0; i < 3; ++i) if (wordAt72AtCall[i] != lo(vt_GameObject)) { std::puts("fail: GameObject table before heads"); bad = 1; }
    unsigned w72; std::memcpy(&w72, arena + 72, 4);
    if (w72 != lo(vt_RollDoor)) { std::puts("fail: final table pointer"); bad = 1; }
    const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{76,91}};
    bool copied[256] = {};
    for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
    for (unsigned i = 0; i < 256; ++i) {
        if (i >= 72 && i < 76) continue;
        if (i < 12) continue;
        if (copied[i] && arena[i] != src[i]) { std::printf("fail: byte %u not copied\n", i); bad = 1; break; }
        if (!copied[i] && arena[i] != 0xa5) { std::printf("fail: byte %u written outside the ranges\n", i); bad = 1; break; }
    }
    return bad;
}
