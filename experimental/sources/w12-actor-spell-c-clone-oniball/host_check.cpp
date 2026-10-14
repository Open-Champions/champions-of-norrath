// Host contract for clone__7OniBall; not an EE input. Includes the source.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_10GameObject[8], vt_10Projectile[8], vt_7OniBall[8];
static unsigned char arena[256] __attribute__((aligned(16)));
static unsigned char src[256] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned wordAt72AtCall[8];
static int callCount;
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
void ListHead_copy(void* d, const void* s) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    std::memcpy(&wordAt72AtCall[callCount++], arena + 72, 4);
    std::memset(d, 0, 4);
}
static unsigned lightWordsAtCall[2];
void lightAdd__FP5Light(void* light) {
    calls += "light" + std::to_string(static_cast<unsigned char*>(light) - arena) + ";";
    std::memcpy(lightWordsAtCall, light, 8);
}
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }

static int run(unsigned w0, unsigned w4) {
    int bad = 0;
    for (int i = 0; i < 256; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    std::memcpy(src + 120, &w0, 4);
    std::memcpy(src + 124, &w4, 4);
    std::memset(arena, 0xa5, sizeof arena);
    calls.clear(); callCount = 0; lightWordsAtCall[0] = lightWordsAtCall[1] = 7;
    void* r = clone__7OniBall(src);
    bool light = w0 != 0 || w4 != 0;
    std::string want = "alloc184;lh0<0;lh4<4;lh8<8;lh76<76;";
    if (light) want += "light120;";
    if (r != arena) { std::puts("fail: returns the new object"); bad = 1; }
    if (calls != want) { std::puts(calls.c_str()); std::puts("fail: call order"); bad = 1; }
    if (wordAt72AtCall[0] != lo(vt_10GameObject) || wordAt72AtCall[1] != lo(vt_10GameObject) ||
        wordAt72AtCall[2] != lo(vt_10GameObject) || wordAt72AtCall[3] != lo(vt_10Projectile)) {
        std::puts("fail: table word at the list head calls"); bad = 1;
    }
    unsigned w72; std::memcpy(&w72, arena + 72, 4);
    if (w72 != lo(vt_7OniBall)) { std::puts("fail: final table pointer"); bad = 1; }
    if (light && (lightWordsAtCall[0] != 0 || lightWordsAtCall[1] != 0)) { std::puts("fail: light words not cleared before the call"); bad = 1; }
    static bool copied[256];
    std::memset(copied, 0, sizeof copied);
    const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{80,80},{84,116},{120,179},{180,183}};
    for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
    for (unsigned i = 0; i < 184; ++i) {
        bool head = (i < 12 && i / 4 < 3) || (i >= 76 && i < 80);
        bool cleared = light && i >= 120 && i < 128;
        if (i >= 72 && i < 76) continue;
        if (cleared) { if (arena[i] != 0) { std::printf("fail: byte %u not cleared\n", i); bad = 1; break; } continue; }
        if (copied[i] && arena[i] != src[i]) { std::printf("fail: byte %u not copied\n", i); bad = 1; break; }
        if (!copied[i] && !head && arena[i] != 0xa5) { std::printf("fail: byte %u written but not in a copy range\n", i); bad = 1; break; }
    }
    return bad;
}

int main() {
    int bad = 0;
    failNext = true; calls.clear();
    if (clone__7OniBall(src) != nullptr || calls != "alloc184;") { std::puts("fail: null allocation"); bad = 1; }
    failNext = false;
    bad |= run(5, 6); bad |= run(0, 6); bad |= run(5, 0); bad |= run(0, 0);
    return bad;
}
