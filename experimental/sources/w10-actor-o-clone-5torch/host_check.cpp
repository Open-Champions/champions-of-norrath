// Host contract for clone__5Torch; not an EE input. Includes the source.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_10GameObject[8], vt_5Torch[8];
static unsigned char arena[256] __attribute__((aligned(16)));
static unsigned char src[256] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static unsigned word72() { unsigned w; std::memcpy(&w, arena + 72, 4); return w; }
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
void ListHead_copy(void* d, const void*) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + (word72() == lo(vt_10GameObject) ? "g" : "x") + ";";
    std::memset(d, 0, 4);
}
void lightAdd__FP5Light(void* l) {
    unsigned w[2]; std::memcpy(w, l, 8);
    calls += "light" + std::to_string(static_cast<unsigned char*>(l) - arena) + (word72() == lo(vt_5Torch) ? "t" : "x") + (w[0] || w[1] ? "dirty" : "clear") + ";";
}
int main() {
    int bad = 0;
    for (int i = 0; i < 256; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    failNext = true;
    std::memset(arena, 0xa5, sizeof arena);
    if (clone__5Torch(src) != nullptr || calls != "alloc140;") { std::puts("fail: null allocation"); bad = 1; }
    failNext = false;
    for (int mode = 0; mode < 4; ++mode) {
        for (int i = 0; i < 256; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
        if (!(mode & 1)) std::memset(src + 76, 0, 4);
        if (!(mode & 2)) std::memset(src + 80, 0, 4);
        std::memset(arena, 0xa5, sizeof arena); calls.clear();
        void* r = clone__5Torch(src);
        std::string want = "alloc140;lh0g;lh4g;lh8g;";
        if (mode) want += "light76tclear;";
        if (r != arena || calls != want) { std::printf("fail: mode %d calls %s\n", mode, calls.c_str()); bad = 1; }
        if (word72() != lo(vt_5Torch)) { std::puts("fail: final table pointer"); bad = 1; }
        for (unsigned i = 0; i < 256; ++i) {
            bool head = i < 12;
            bool copied = (i >= 12 && i <= 41) || (i >= 44 && i <= 57) || (i >= 60 && i <= 71) || (i >= 76 && i <= 135) || (i >= 136 && i <= 138);
            bool cleared = mode && (i >= 76 && i < 84);
            if (i >= 72 && i < 76) continue;
            if (head) { if (arena[i] != 0) { std::printf("fail: head byte %u\n", i); bad = 1; } continue; }
            if (cleared) { if (arena[i] != 0) { std::printf("fail: light word byte %u not cleared\n", i); bad = 1; } continue; }
            if (copied && arena[i] != src[i]) { std::printf("fail: byte %u not copied\n", i); bad = 1; }
            if (!copied && arena[i] != 0xa5) { std::printf("fail: byte %u written outside the ranges\n", i); bad = 1; }
        }
    }
    return bad;
}
