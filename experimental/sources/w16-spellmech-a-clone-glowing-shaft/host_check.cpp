// Host contract for clone__12GlowingShaft; not an EE input. Stored 32-bit addresses are offsets into an arena
// (NORRATH_HOST_ARENA). Checks the allocation size, the null-allocation path, the three list-head copy constructions, every
// copied byte range against the source, the class table at +72, and that the gap bytes stay untouched.
#define NORRATH_HOST_ARENA 1
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x1000];
static const unsigned base = 0x100000u;
unsigned char vt_GameObject[8];
unsigned char vt_GlowingShaft[8];
static const unsigned baseAddress = 0x7f0010u, shaftAddress = 0x7f0020u;
static std::string calls; static int allocSize; static bool failAlloc;
static std::vector<std::pair<void*, const void*> > heads;
static unsigned baseWordAtFirstHead;

extern "C" void* blockAlloc__Fi(int size) { calls += "alloc;"; allocSize = size; return failAlloc ? nullptr : arena + 0x400; }
extern "C" void* __8ListHeadRC8ListHead(void* d, const void* s) {
    calls += "head;"; heads.push_back(std::make_pair(d, s));
    if (heads.size() == 1) std::memcpy(&baseWordAtFirstHead, static_cast<unsigned char*>(d) + 72, 4);
    return d;
}
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + (a - base); }
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_GameObject) return baseAddress;
    if (q == vt_GlowingShaft) return shaftAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

int main() {
    int bad = 0;
    unsigned char* const source = arena + 0x100;
    unsigned char* const dest = arena + 0x400;
    // [start, end) copied from the source
    const int ranges[][2] = {{12, 42}, {44, 58}, {60, 72}, {76, 90}};
    for (int pattern = 0; pattern < 6; ++pattern) for (int fail = 0; fail < 2; ++fail) {
        std::memset(arena, 0, sizeof arena);
        for (int i = 0; i < 128; ++i) source[i] = static_cast<unsigned char>(i * 37 + pattern * 11 + 5);
        if (pattern == 5) std::memset(source, 0xff, 128);    // NaN-looking float fields must be copied bit-exactly
        std::memset(dest, 0xa5, 128);
        calls.clear(); heads.clear(); failAlloc = fail != 0; allocSize = 0; baseWordAtFirstHead = 0;
        void* result = clone__12GlowingShaft(source);
        if (allocSize != 92) { std::puts("fail: alloc size"); bad = 1; }
        if (fail) {
            if (result != nullptr || calls != "alloc;") { std::puts("fail: null path"); bad = 1; }
            continue;
        }
        if (result != dest) { std::puts("fail: result"); bad = 1; }
        if (calls != "alloc;head;head;head;" || heads.size() != 3) { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; continue; }
        for (int i = 0; i < 3; ++i)
            if (heads[i].first != dest + 4 * i || heads[i].second != source + 4 * i) { std::puts("fail: head args"); bad = 1; }
        if (baseWordAtFirstHead != baseAddress) { std::puts("fail: base table before heads"); bad = 1; }
        for (const auto& r : ranges)
            for (int i = r[0]; i < r[1]; ++i) {
                if (i >= 72 && i < 76) continue;
                if (dest[i] != source[i]) { std::printf("fail: byte %d\n", i); bad = 1; }
            }
        if (word(dest + 72) != shaftAddress) { std::puts("fail: class table"); bad = 1; }
        for (int i = 0; i < 128; ++i) {
            const bool copied = (i >= 12 && i < 42) || (i >= 44 && i < 58) || (i >= 60 && i < 90);
            if (!copied && dest[i] != 0xa5) { std::printf("fail: gap byte %d touched\n", i); bad = 1; }
        }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
