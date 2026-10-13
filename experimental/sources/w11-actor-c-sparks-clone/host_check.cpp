// Host contract for clone__6Sparks; not an EE input. Includes the source.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_GameObject[8], vt_Projectile[8], vt_Sparks[8];
static unsigned char arena[256] __attribute__((aligned(16)));
static unsigned char src[256] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned wordAt72AtCall[16];
static int callCount;
static unsigned lightWordsAtCall[2], wordAt72AtLight;
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
void ListHead_copy(void* d, const void* s) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    std::memcpy(&wordAt72AtCall[callCount++], arena + 72, 4); std::memset(d, 0, 4);
}
void lightAdd__FP5Light(void* l) {
    calls += "light" + std::to_string(static_cast<unsigned char*>(l) - arena) + ";";
    std::memcpy(lightWordsAtCall, l, 8); std::memcpy(&wordAt72AtLight, arena + 72, 4);
}
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static int run(const char* label, bool zeroWords, int wordToSet) {
    int bad = 0;
    for (int i = 0; i < 256; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    if (zeroWords) { std::memset(src + 100, 0, 8); if (wordToSet >= 0) src[100 + wordToSet * 4 + 1] = 9; }
    std::memset(arena, 0xa5, sizeof arena); calls.clear(); callCount = 0;
    lightWordsAtCall[0] = lightWordsAtCall[1] = 7;
    void* r = clone__6Sparks(src);
    if (r != arena) { std::printf("fail %s: returns the new object\n", label); bad = 1; }
    const bool wantLight = !zeroWords || wordToSet >= 0;
    std::string want = "alloc184;lh0<0;lh4<4;lh8<8;lh76<76;" + std::string(wantLight ? "light100;" : "");
    if (calls != want) { std::printf("fail %s: calls %s\n", label, calls.c_str()); bad = 1; }
    for (int i = 0; i < 3; ++i) if (wordAt72AtCall[i] != lo(vt_GameObject)) { std::printf("fail %s: table word before heads\n", label); bad = 1; }
    if (wordAt72AtCall[3] != lo(vt_Projectile)) { std::printf("fail %s: table word at the +76 head\n", label); bad = 1; }
    if (wantLight && wordAt72AtLight != lo(vt_Sparks)) { std::printf("fail %s: table word at the light call\n", label); bad = 1; }
    if (wantLight && (lightWordsAtCall[0] != 0 || lightWordsAtCall[1] != 0)) { std::printf("fail %s: light words not cleared at the call\n", label); bad = 1; }
    unsigned w72; std::memcpy(&w72, arena + 72, 4);
    if (w72 != lo(vt_Sparks)) { std::printf("fail %s: final table pointer\n", label); bad = 1; }
    const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{80,80},{84,87},{88,99},{108,159},{160,183}};
    static bool copied[256];
    std::memset(copied, 0, sizeof copied);
    for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
    for (unsigned i = 0; i < 256; ++i) {
        bool head = i < 12 || (i >= 76 && i < 80) || (i >= 100 && i < 108);
        if (i >= 72 && i < 76) continue;
        if (i >= 184) { if (arena[i] != 0xa5) { std::printf("fail %s: wrote past the end\n", label); bad = 1; } continue; }
        if (copied[i] && arena[i] != src[i]) { std::printf("fail %s: byte %u not copied\n", label, i); bad = 1; break; }
        if (!copied[i] && !head && arena[i] != 0xa5) { std::printf("fail %s: byte %u written but not in a copy range\n", label, i); bad = 1; break; }
    }
    if (wantLight) { for (int i = 100; i < 108; ++i) if (arena[i]) { std::printf("fail %s: light words\n", label); bad = 1; } }
    else for (int i = 100; i < 108; ++i) if (arena[i] != src[i]) { std::printf("fail %s: untouched light words\n", label); bad = 1; }
    return bad;
}
int main() {
    int bad = 0;
    failNext = true;
    if (clone__6Sparks(src) != nullptr || calls != "alloc184;") { std::puts("fail: null allocation"); bad = 1; }
    failNext = false;
    bad |= run("pattern", false, -1);
    bad |= run("both-zero", true, -1);
    bad |= run("first-only", true, 0);
    bad |= run("second-only", true, 1);
    if (!bad) std::puts("clone Sparks ok");
    return bad;
}
