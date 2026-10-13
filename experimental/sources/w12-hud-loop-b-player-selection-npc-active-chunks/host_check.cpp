// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -DW12_HUD_HOST host_check.cpp
// Pointer words in the target records are handles: 0x100 + i names a target string, 0x1000 + k names
// the k-th mask slot of masks[].
#include "npc_active_chunks.cpp"
#include <cstdio>
#include <cstring>

extern "C" {
signed char playerSelectInfo[41 * 4];
signed char hairStyleSetting[16];
signed char beardSetting[16];
alignas(8) unsigned char targets[11 * 636];  // record 10 only stands in for a failed match
}
static const char* const names[10] = {"bm", "bf", "wem", "wef", "hem", "hef", "em", "ef", "dem", "def"};
alignas(8) static unsigned long long masks[40];
static const char* nameTag = "";
static bool hasPnum;
static int pnumAnswers[3], pnumCalls, tagCalls;
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    if (word >= 0x1000u) return reinterpret_cast<__UINTPTR_TYPE__>(&masks[word - 0x1000u]);
    return reinterpret_cast<__UINTPTR_TYPE__>(names[word - 0x100u]);
}
extern "C" char* findTagString__10GameObjectPci(void*, const char* tag, int) {
    ++tagCalls;
    if (!std::strcmp(tag, "name")) return const_cast<char*>(nameTag);
    return hasPnum && !std::strcmp(tag, "pnum") ? const_cast<char*>("1") : nullptr;
}
extern "C" int findTagInt__10GameObjectPci(void*, const char* tag, int) {
    if (std::strcmp(tag, "pnum") != 0) return -99;
    return pnumAnswers[pnumCalls++ % 3];
}

static int bad;
static void setWord(int rec, int off, unsigned w) { std::memcpy(targets + rec * 636 + off, &w, 4); }
static void expect(const char* what, unsigned long long got, unsigned long long want) {
    if (got != want) { std::printf("FAIL %s: got %llx want %llx\n", what, got, want); bad = 1; }
}
static void reset() {
    std::memset(targets, 0, sizeof targets);
    std::memset(playerSelectInfo, 0, sizeof playerSelectInfo);
    std::memset(hairStyleSetting, -1, sizeof hairStyleSetting);
    std::memset(beardSetting, -1, sizeof beardSetting);
    for (int i = 0; i < 10; ++i) setWord(i, 0, 0x100u + i);
    nameTag = "";
    for (int k = 0; k < 40; ++k) masks[k] = 1ull << (k + 8);
    hasPnum = false; pnumCalls = 0;
}

int main() {
    char npc[8];
    // Nothing set: all ones. Each pointer slot clears exactly its own mask.
    reset(); nameTag = "ef";
    expect("none", playerSelectionGetNPCActiveChunks__FP3NPC(npc), ~0ull);
    const int offsets[] = {92, 96, 100, 104};
    for (int n = 0; n < 4; ++n) {
        reset(); nameTag = "ef"; setWord(7, offsets[n], 0x1000u + n);
        expect("single", playerSelectionGetNPCActiveChunks__FP3NPC(npc), ~masks[n]);
    }
    // Hair style and beard from the settings tables of the matched index.
    reset(); nameTag = "hem";
    hairStyleSetting[4] = 2; beardSetting[4] = 3;
    setWord(4, 28 + 2 * 4, 0x1010u); setWord(4, 60 + 3 * 4, 0x1011u);
    setWord(4, 28 + 3 * 4, 0x1012u); setWord(4, 60 + 2 * 4, 0x1013u);  // other styles stay unused
    expect("settings", playerSelectionGetNPCActiveChunks__FP3NPC(npc), ~(masks[16] | masks[17]));
    // A style of -1 skips the lookup even if a word sits at the -1 position.
    reset(); nameTag = "hem"; hairStyleSetting[4] = -1; beardSetting[4] = -1;
    setWord(4, 24, 0x1020u); setWord(4, 56, 0x1021u);
    expect("minus one", playerSelectionGetNPCActiveChunks__FP3NPC(npc), ~0ull);
    // pnum path: three calls; first picks hair from +33, third picks beard from +36, second ignored.
    reset(); nameTag = "bf"; hasPnum = true;
    playerSelectInfo[1 * 41 + 33] = 1; playerSelectInfo[2 * 41 + 36] = 2;
    playerSelectInfo[0 * 41 + 33] = 5; playerSelectInfo[3 * 41 + 36] = 5; playerSelectInfo[3 * 41 + 33] = 7;
    pnumAnswers[0] = 1; pnumAnswers[1] = 3; pnumAnswers[2] = 2;
    hairStyleSetting[1] = 0; beardSetting[1] = 0;  // not used on this path
    setWord(1, 28 + 1 * 4, 0x1000u + 20); setWord(1, 60 + 2 * 4, 0x1000u + 21);
    setWord(1, 28, 0x1000u + 22); setWord(1, 60, 0x1000u + 23);
    expect("pnum", playerSelectionGetNPCActiveChunks__FP3NPC(npc), ~(masks[20] | masks[21]));
    // Three pnum queries are made on this path (one for the name, one for the tag test, three ints).
    // A different second answer changes nothing.
    pnumCalls = 0; pnumAnswers[1] = 0;
    expect("pnum second ignored", playerSelectionGetNPCActiveChunks__FP3NPC(npc), ~(masks[20] | masks[21]));
    // No name match: record 10 is read through the settings (-1 here), so only fixed slots count.
    reset(); nameTag = "zz";
    expect("no match", playerSelectionGetNPCActiveChunks__FP3NPC(npc), ~0ull);
    // Overlapping masks: clearing is a union of bits, not a replacement.
    reset(); nameTag = "bm";
    setWord(0, 92, 0x1000u + 1); setWord(0, 96, 0x1000u + 1); setWord(0, 100, 0x1000u + 2);
    masks[1] = 0xf0ull << 8; masks[2] = 0x3ull;
    expect("union", playerSelectionGetNPCActiveChunks__FP3NPC(npc), ~(0xf0ull << 8 | 0x3ull));
    std::printf(bad ? "FAIL\n" : "ok: active chunks\n");
    return bad;
}
