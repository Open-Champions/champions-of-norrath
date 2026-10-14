// Host contract for clone__8HolyBolt; not an EE input. Stored 32-bit addresses are offsets into an arena
// (NORRATH_HOST_ARENA). Checks the allocation size, the null path, the four list-head copy constructions and their order,
// the three table stores, every copied range against the source, untouched gap bytes, and the light re-registration (only
// when one of the first two light words is non-zero; both zeroed before lightAdd, which receives the copy's +116).
#define NORRATH_HOST_ARENA 1
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x1000];
static const unsigned base = 0x100000u;
unsigned char vt_GameObject[8];
unsigned char vt_Projectile[8];
unsigned char vt_HolyBolt[8];
static const unsigned goAddress = 0x7f0010u, projAddress = 0x7f0020u, boltAddress = 0x7f0030u;
static std::string calls; static int allocSize; static bool failAlloc;
static std::vector<std::pair<void*, const void*> > heads;
static std::vector<unsigned> tableAtHead;
static void* lightSeen; static unsigned lightWord0, lightWord4; static unsigned char lightCopy[60];

extern "C" void* blockAlloc__Fi(int size) { calls += "alloc;"; allocSize = size; return failAlloc ? nullptr : arena + 0x400; }
extern "C" void* __8ListHeadRC8ListHead(void* d, const void* s) {
    calls += "head;"; heads.push_back(std::make_pair(d, s));
    unsigned w; std::memcpy(&w, static_cast<unsigned char*>(d) - (heads.size() == 4 ? 76 : 0) + 72, 4); tableAtHead.push_back(w);
    return d;
}
extern "C" void lightAdd__FP5Light(void* light) {
    calls += "light;"; lightSeen = light; std::memcpy(lightCopy, light, 60);
    std::memcpy(&lightWord0, light, 4); std::memcpy(&lightWord4, static_cast<unsigned char*>(light) + 4, 4);
}
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + (a - base); }
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_GameObject) return goAddress;
    if (q == vt_Projectile) return projAddress;
    if (q == vt_HolyBolt) return boltAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
// Bytes the clone copies from the source (list-head regions at 0..11 and 76..79 belong to the list constructor).
static bool copiedByte(int i) {
    return (i >= 12 && i < 42) || (i >= 44 && i < 58) || (i >= 60 && i < 72) || i == 80 || (i >= 84 && i < 192);
}

int main() {
    int bad = 0;
    unsigned char* const source = arena + 0x100;
    unsigned char* const dest = arena + 0x400;
    for (int pattern = 0; pattern < 8; ++pattern) for (int fail = 0; fail < 2; ++fail) {
        std::memset(arena, 0, sizeof arena);
        for (int i = 0; i < 200; ++i) source[i] = static_cast<unsigned char>(i * 37 + pattern * 11 + 5);
        if (pattern == 5) std::memset(source, 0xff, 200);
        if (pattern & 1) std::memset(source + 116, 0, 4);            // first light word zero
        if (pattern & 2) std::memset(source + 120, 0, 4);            // second light word zero
        std::memset(dest, 0xa5, 200);
        calls.clear(); heads.clear(); tableAtHead.clear(); failAlloc = fail != 0; allocSize = 0; lightSeen = nullptr;
        void* result = clone__8HolyBolt(source);
        if (allocSize != 192) { std::puts("fail: alloc size"); bad = 1; }
        if (fail) {
            if (result != nullptr || calls != "alloc;") { std::puts("fail: null path"); bad = 1; }
            continue;
        }
        const bool wantLight = word(source + 116) != 0 || word(source + 120) != 0;
        const std::string want = std::string("alloc;head;head;head;head;") + (wantLight ? "light;" : "");
        if (result != dest) { std::puts("fail: result"); bad = 1; }
        if (calls != want || heads.size() != 4) { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; continue; }
        const int offsets[] = {0, 4, 8, 76};
        for (int i = 0; i < 4; ++i)
            if (heads[i].first != dest + offsets[i] || heads[i].second != source + offsets[i]) { std::puts("fail: head args"); bad = 1; }
        if (tableAtHead[0] != goAddress || tableAtHead[3] != projAddress) { std::puts("fail: tables at heads"); bad = 1; }
        if (word(dest + 72) != boltAddress) { std::puts("fail: class table"); bad = 1; }
        for (int i = 0; i < 200; ++i) {
            if (i >= 72 && i < 76) continue;
            if (i >= 116 && i < 124 && wantLight) continue;
            if (i >= 192) { if (dest[i] != 0xa5) { std::puts("fail: past the end touched"); bad = 1; } continue; }
            const bool copied = copiedByte(i);
            if (copied && dest[i] != source[i]) { std::printf("fail: byte %d\n", i); bad = 1; }
            if (!copied && !(i < 12) && !(i >= 76 && i < 80) && dest[i] != 0xa5) { std::printf("fail: gap byte %d touched\n", i); bad = 1; }
        }
        if (wantLight) {
            if (lightSeen != dest + 116 || lightWord0 != 0 || lightWord4 != 0 || word(dest + 116) != 0 || word(dest + 120) != 0) { std::puts("fail: light call"); bad = 1; }
            if (std::memcmp(lightCopy + 8, source + 124, 52) != 0) { std::puts("fail: light body"); bad = 1; }
        } else if (std::memcmp(dest + 116, source + 116, 60) != 0) { std::puts("fail: light copy"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
