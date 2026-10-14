// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -DW12_HUD_HOST host_check.cpp
// Attachment and texture words in the target records are plain handles; the stub records them.
#include "npc_draw.cpp"
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
signed char playerSelectInfo[41 * 4];
signed char hairStyleSetting[16], hairColorSetting[16], beardSetting[16];
alignas(8) unsigned char targets[11 * 636];
}
static const char* const names[10] = {"bm", "bf", "wem", "wef", "hem", "hef", "em", "ef", "dem", "def"};
static const char* nameTag = "";
static bool hasPnum;
static int pnumAnswers[3], pnumCalls;
static std::string trace;
__UINTPTR_TYPE__ hostWordAddress(unsigned word) { return reinterpret_cast<__UINTPTR_TYPE__>(names[word - 0x100u]); }
extern "C" char* findTagString__10GameObjectPci(void*, const char* tag, int) {
    if (!std::strcmp(tag, "name")) return const_cast<char*>(nameTag);
    return hasPnum && !std::strcmp(tag, "pnum") ? const_cast<char*>("1") : nullptr;
}
extern "C" int findTagInt__10GameObjectPci(void*, const char*, int) { return pnumAnswers[pnumCalls++ % 3]; }
extern "C" void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* a, void* t, int one, void* m, void* r,
                                                                                    unsigned long mask, int x, int y) {
    trace += "[" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(a)) + "," + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(t)) + "," +
             std::to_string(one) + "," + (m == reinterpret_cast<void*>(0x77) ? "M" : "?") + (r == reinterpret_cast<void*>(0x88) ? "R" : "?") + "," +
             std::to_string(static_cast<unsigned>(mask)) + "," + std::to_string(x) + "," + std::to_string(y) + "]";
}

static int bad;
static void setWord(int rec, int off, unsigned w) { std::memcpy(targets + rec * 636 + off, &w, 4); }
static void reset() {
    std::memset(targets, 0, sizeof targets);
    std::memset(playerSelectInfo, 0, sizeof playerSelectInfo);
    std::memset(hairStyleSetting, -1, sizeof hairStyleSetting);
    std::memset(beardSetting, -1, sizeof beardSetting);
    std::memset(hairColorSetting, 0, sizeof hairColorSetting);
    for (int i = 0; i < 10; ++i) setWord(i, 0, 0x100u + i);
    hasPnum = false; pnumCalls = 0; trace.clear();
}
static std::string part(unsigned a, unsigned t) {
    return "[" + std::to_string(a) + "," + std::to_string(t) + ",1,MR,4294967295,-1,0]";
}
static void run(const char* what, const char* want) {
    trace.clear();
    playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord(nullptr, reinterpret_cast<void*>(0x77), reinterpret_cast<void*>(0x88));
    if (trace != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, trace.c_str(), want); bad = 1; }
}

int main() {
    reset(); nameTag = "ef";
    run("nothing", "");
    // The four fixed attachments are drawn in the order 104, 92, 100, 96 with textures 632, 620, 628, 624.
    setWord(7, 104, 1001); setWord(7, 632, 2001); setWord(7, 92, 1002); setWord(7, 620, 2002);
    setWord(7, 100, 1003); setWord(7, 628, 2003); setWord(7, 96, 1004); setWord(7, 624, 2004);
    run("fixed", (part(1001, 2001) + part(1002, 2002) + part(1003, 2003) + part(1004, 2004)).c_str());
    // Hair and beard from the settings: style 2, colour 3, beard 1 on record 7; they sit between 104 and 92.
    hairStyleSetting[7] = 2; hairColorSetting[7] = 3; beardSetting[7] = 1;
    setWord(7, 28 + 2 * 4, 3001); setWord(7, 108 + (2 * 8 + 3) * 4, 4001);
    setWord(7, 60 + 1 * 4, 3002); setWord(7, 364 + (1 * 8 + 3) * 4, 4002);
    run("settings", (part(1001, 2001) + part(3001, 4001) + part(3002, 4002) + part(1002, 2002) + part(1003, 2003) + part(1004, 2004)).c_str());
    // A zero attachment word skips its draw even when its texture word is set.
    setWord(7, 28 + 2 * 4, 0);
    run("zero attachment", (part(1001, 2001) + part(3002, 4002) + part(1002, 2002) + part(1003, 2003) + part(1004, 2004)).c_str());
    // Style and beard of -1 skip even when words sit at the -1 position.
    hairStyleSetting[7] = -1; beardSetting[7] = -1;
    setWord(7, 24, 5001); setWord(7, 56, 5002);
    run("minus one", (part(1001, 2001) + part(1002, 2002) + part(1003, 2003) + part(1004, 2004)).c_str());
    // pnum path: byte 33 of record 1, byte 34 of record 3, byte 36 of record 2; other bytes are decoys.
    reset(); nameTag = "bm"; hasPnum = true;
    playerSelectInfo[1 * 41 + 33] = 1; playerSelectInfo[3 * 41 + 34] = 2; playerSelectInfo[2 * 41 + 36] = 3;
    playerSelectInfo[0 * 41 + 33] = 0; playerSelectInfo[0 * 41 + 34] = 7; playerSelectInfo[0 * 41 + 36] = 0;
    pnumAnswers[0] = 1; pnumAnswers[1] = 3; pnumAnswers[2] = 2;
    hairStyleSetting[0] = 0; beardSetting[0] = 0; hairColorSetting[0] = 5;
    setWord(0, 28 + 1 * 4, 6001); setWord(0, 108 + (1 * 8 + 2) * 4, 7001);
    setWord(0, 60 + 3 * 4, 6002); setWord(0, 364 + (3 * 8 + 2) * 4, 7002);
    run("pnum", (part(6001, 7001) + part(6002, 7002)).c_str());
    // No match: record 10 is used, only its own words count.
    reset(); nameTag = "zz";
    setWord(10, 104, 8001); setWord(10, 632, 8002);
    run("no match", part(8001, 8002).c_str());
    std::printf(bad ? "FAIL\n" : "ok: npc draw\n");
    return bad;
}
