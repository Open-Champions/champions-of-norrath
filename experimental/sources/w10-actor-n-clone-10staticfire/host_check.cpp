// Host contract for clone__10StaticFire; not an EE input. Includes the source.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_GameObject[8], vt_StaticFire[8];
static unsigned char arena[1280] __attribute__((aligned(16)));
static unsigned char src[1280] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned wordAt72AtCall[16];
static int callCount;
static unsigned lightWordsAtCall[2];
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
void ListHead_copy(void* d, const void* s) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    std::memcpy(&wordAt72AtCall[callCount++], arena + 72, 4); std::memset(d, 0, 4);
}
void lightAdd__FP5Light(void* l) {
    calls += "light" + std::to_string(static_cast<unsigned char*>(l) - arena) + ";";
    std::memcpy(lightWordsAtCall, l, 8);
}
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static int run(const char* label, bool zeroWords, int wordToSet) {
    int bad = 0;
    for (int i = 0; i < 1280; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    if (zeroWords) { std::memset(src + 128, 0, 8); if (wordToSet >= 0) src[128 + wordToSet * 4 + 1] = 9; }
    std::memset(arena, 0xa5, sizeof arena); calls.clear(); callCount = 0;
    lightWordsAtCall[0] = lightWordsAtCall[1] = 7;
    void* r = clone__10StaticFire(src);
    if (r != arena) { std::printf("fail %s: returns the new object\n", label); bad = 1; }
    const bool wantLight = !zeroWords || wordToSet >= 0;
    std::string want = "alloc1192;lh0<0;lh4<4;lh8<8;" + std::string(wantLight ? "light128;" : "");
    if (calls != want) { std::printf("fail %s: calls %s\n", label, calls.c_str()); bad = 1; }
    for (int i = 0; i < 3; ++i) if (wordAt72AtCall[i] != lo(vt_GameObject)) { std::printf("fail %s: table word before heads\n", label); bad = 1; }
    if (wantLight && (lightWordsAtCall[0] != 0 || lightWordsAtCall[1] != 0)) { std::printf("fail %s: light words not cleared at the call\n", label); bad = 1; }
    unsigned w72; std::memcpy(&w72, arena + 72, 4);
    if (w72 != lo(vt_StaticFire)) { std::printf("fail %s: final table pointer\n", label); bad = 1; }
    const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{76,112},{114,127},{136,187},{188,1187},{1188,1191}};
    static bool copied[1280];
    std::memset(copied, 0, sizeof copied);
    for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
    for (unsigned i = 0; i < 1280; ++i) {
        bool head = i < 12 || (i >= 128 && i < 136);
        if (i >= 72 && i < 76) continue;
        if (copied[i] && arena[i] != src[i]) { std::printf("fail %s: byte %u not copied\n", label, i); bad = 1; break; }
        if (!copied[i] && !head && arena[i] != 0xa5) { std::printf("fail %s: byte %u written but not in a copy range\n", label, i); bad = 1; break; }
    }
    if (wantLight) {
        if (arena[128] || arena[129] || arena[130] || arena[131] || arena[132] || arena[133] || arena[134] || arena[135]) { std::printf("fail %s: light words\n", label); bad = 1; }
    }
    return bad;
}
int main() {
    int bad = 0;
    failNext = true;
    if (clone__10StaticFire(src) != nullptr || calls != "alloc1192;") { std::puts("fail: null allocation"); bad = 1; }
    failNext = false;
    bad |= run("pattern", false, -1);
    bad |= run("both-zero", true, -1);
    bad |= run("first-only", true, 0);
    bad |= run("second-only", true, 1);
    return bad;
}
