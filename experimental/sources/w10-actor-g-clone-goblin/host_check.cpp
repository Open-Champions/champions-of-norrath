// Host contract for clone__6Goblin; not an EE input. Includes the source (32-bit table words are
// truncated host addresses, compared against the same truncation).
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char vt_GameObject[8], vt_Creature[8], vt_Goblin[8];
static std::string calls; static unsigned char* arena; static int allocSize; static bool failAlloc;
static unsigned char* dest;
static unsigned w72() { unsigned v; std::memcpy(&v, dest + 72, 4); return v; }
extern "C" void* blockAlloc__Fi(int size) { allocSize = size; calls += "alloc;"; return failAlloc ? nullptr : arena; }
extern "C" void ListHead_copy(void* d, const void* s) {
    char b[48]; std::snprintf(b, sizeof b, "lh%ld;", static_cast<long>(static_cast<unsigned char*>(d) - dest)); calls += b;
    (void)s; std::memset(d, 0, 4);
}
extern "C" void AnimationState8_copy(void* d, const void*) { char b[48]; std::snprintf(b, sizeof b, "an%ld;", static_cast<long>(static_cast<unsigned char*>(d) - dest)); calls += b; }
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
int main() {
    int bad = 0;
    static unsigned char storage[1208]; arena = storage; dest = arena;
    static unsigned char src[1144];
    for (int i = 0; i < 1144; ++i) src[i] = static_cast<unsigned char>(i * 7 + 3);
    std::memset(storage, 0xA5, sizeof storage);
    void* r = clone__6Goblin(src);
    if (r != arena || allocSize != 1144) { std::puts("fail: result/alloc size"); bad = 1; }
    // expected copied ranges (inclusive)
    static const int copied[][2] = {{12,41}, {44,57}, {60,71}, {76,76}, {80,85}, {88,156}, {160,173}, {176,189}, {192,205}, {208,233}, {236,285}, {552,570}, {572,573}, {580,749}, {756,800}, {804,887}, {892,971}, {972,980}, {988,1143}};
    std::vector<bool> expectCopy(1208, false);
    for (auto& c : copied) for (int i = c[0]; i <= c[1]; ++i) expectCopy[i] = true;
    for (int i = 0; i < 1144; ++i) {
        bool inLh = false; static const int lhs[] = {0, 4, 8, 576, 752, 888, 984};
        for (int h : lhs) if (i >= h && i < h + 4) inLh = true;
        if (i >= 72 && i < 76) continue;
        if (inLh) { if (storage[i] != 0) { std::printf("fail: list head byte %d\n", i); bad = 1; } continue; }
        if (expectCopy[i]) { if (storage[i] != src[i]) { std::printf("fail: byte %d not copied\n", i); bad = 1; } }
        else if (i >= 288 && i < 552) { /* animation member written by its own constructor */ }
        else if (storage[i] != 0xA5) { std::printf("fail: byte %d written but not in a copy range\n", i); bad = 1; }
    }
    for (int i = 1144; i < 1208; ++i) if (storage[i] != 0xA5) { std::puts("fail: write past the allocation"); bad = 1; }
    if (w72() != low(vt_Goblin)) { std::puts("fail: final table pointer"); bad = 1; }
    if (calls != "alloc;lh0;lh4;lh8;an288;lh576;lh752;lh888;lh984;") { std::printf("fail: call order %s\n", calls.c_str()); bad = 1; }
    calls.clear(); failAlloc = true;
    std::memset(storage, 0xA5, sizeof storage);
    if (clone__6Goblin(src) != nullptr || calls != "alloc;") { std::puts("fail: null allocation"); bad = 1; }
    for (unsigned char c : storage) if (c != 0xA5) { std::puts("fail: wrote after failed allocation"); bad = 1; break; }
    if (!bad) std::puts("goblin clone ok");
    return bad;
}
