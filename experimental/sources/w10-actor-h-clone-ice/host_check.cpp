// Host contract for clone__ice; not an EE input. Includes the source.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_GameObject[8], vt_Ice[8];
static unsigned char arena[120] __attribute__((aligned(16)));
static unsigned char src[104] __attribute__((aligned(16)));
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
    for (int run = 0; run < 1; ++run) {
        for (int i = 0; i < 104; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
        
        std::memset(arena, 0xa5, sizeof arena);
        calls.clear(); callCount = 0;
        failNext = true;
        if (clone__3Ice(src) != nullptr || calls != "alloc104;") { std::puts("fail: null allocation"); bad = 1; }
        failNext = false; calls.clear(); callCount = 0;
        void* r = clone__3Ice(src);
        if (r != arena) { std::puts("fail: returns the new object"); bad = 1; }
        if (calls != "alloc104;lh0<0;lh4<4;lh8<8;" ) { std::puts(calls.c_str()); std::puts("fail: call order"); bad = 1; }
        for (int i = 0; i < 3; ++i) if (wordAt72AtCall[i] != lo(vt_GameObject)) { std::puts("fail: GameObject table before heads"); bad = 1; }
        unsigned w72; std::memcpy(&w72, arena + 72, 4);
        if (w72 != lo(vt_Ice)) { std::puts("fail: final table pointer"); bad = 1; }
        const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{76,103}};
        bool copied[120] = {};
        for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
        for (unsigned i = 0; i < 120; ++i) {
            bool head = i < 12;
            bool table = i >= 72 && i < 76;
            if (table || head) continue;
            unsigned char want = src[i];
            if (copied[i] && arena[i] != want) { std::printf("fail: byte %u not copied\n", i); bad = 1; break; }
            if (!copied[i] && arena[i] != 0xa5) { std::printf("fail: byte %u written but not in a copy range\n", i); bad = 1; break; }
        }
        
    }
    if (!bad) std::puts("ice clone ok");
    return bad;
}
