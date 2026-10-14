#define NORRATH_W8_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static char buffers[4][512];
extern "C" unsigned norrath_w8_pointer_to_word(const void* pointer);
extern "C" unsigned norrath_w8_pointer_to_word(const void* pointer) {
    for (unsigned i = 0; i < 4; ++i) if (pointer == buffers[i]) return 0x1000u + i;
    return 0xdead0000u;
}
#include "lump_unload.cpp"
LumpRecord lumps[128];
int nmLumps;
static std::vector<std::string> calls;
extern "C" void freeLump__FPv(void* lump) { calls.push_back("freeLump " + std::to_string((char*)lump - buffers[0])); }
extern "C" void memFree__FPv(void* block) { calls.push_back("memFree " + std::to_string((char*)block - buffers[0])); }
extern "C" void textureFreeTexture__FP8_texture(void* p) { calls.push_back("tex " + std::to_string((char*)p - buffers[0])); }
extern "C" void worldFreeWorld__FP12_worldHeader(void* p) { calls.push_back("world " + std::to_string((char*)p - buffers[0])); }
extern "C" void textRemoveText__FPv(void* p) { calls.push_back("text " + std::to_string((char*)p - buffers[0])); }
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static void setup() {
    std::memset(lumps, 0, sizeof lumps);
    std::memset(buffers, 0, sizeof buffers);
    for (unsigned i = 0; i < 4; ++i) { lumps[i].word[4] = 0x1000u + i; lumps[i].word[0] = 0x100u + i; }
    nmLumps = 4;
    calls.clear();
}
int main() {
    // Plain record in the middle: freeLump, compaction, count decreases.
    setup();
    lumpUnload__FPv(buffers[1]);
    expect(calls.size() == 1 && calls[0] == "freeLump 512", "plain record calls freeLump only");
    expect(nmLumps == 3, "count decreases");
    expect(lumps[0].word[0] == 0x100u && lumps[1].word[0] == 0x102u && lumps[2].word[0] == 0x103u, "later records move down");
    expect(lumps[3].word[0] == 0x103u, "last slot is left as it was");

    // Offset 36 word set: memFree after the release call.
    setup();
    lumps[0].word[9] = 5;
    lumpUnload__FPv(buffers[0]);
    expect(calls.size() == 2 && calls[1] == "memFree 0", "word 36 nonzero calls memFree last");

    // Single-file records pick the call from the extension of the name.
    const char* names[] = {"a.tex", "b.world", "c.uni", "d.bin"};
    const char* want[] = {"tex 256", "world 768", "text 1280", ""};
    for (int i = 0; i < 4; ++i) {
        setup();
        std::strcpy(buffers[i], names[i]);
        lumps[i].word[5] = 1;
        lumpUnload__FPv(buffers[i]);
        const std::string expected = i == 3 ? "" : std::string(want[i]).substr(0, std::string(want[i]).find(' ')) + " " + std::to_string(512 * i + 256);
        expect(i == 3 ? calls.empty() : (calls.size() == 1 && calls[0] == expected), names[i]);
        expect(nmLumps == 3, "flagged record is removed too");
    }
    setup();
    std::strcpy(buffers[2], "noextension");
    lumps[2].word[5] = 1;
    lumpUnload__FPv(buffers[2]);
    expect(calls.empty() && nmLumps == 3, "flagged name without a dot releases nothing but still removes");

    // Unknown pointer: nothing happens.
    setup();
    char other[8];
    lumpUnload__FPv(other);
    expect(calls.empty() && nmLumps == 4, "unknown pointer is ignored");

    // Last record: no move, count decreases.
    setup();
    lumpUnload__FPv(buffers[3]);
    expect(nmLumps == 3 && lumps[3].word[0] == 0x103u, "last record only lowers the count");
    return failures;
}
