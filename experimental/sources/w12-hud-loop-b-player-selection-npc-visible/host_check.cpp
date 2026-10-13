// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -DW12_HUD_HOST host_check.cpp
// The first word of each target record is a 32-bit string handle that hostWordAddress maps.
#include "npc_visible.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>

extern "C" {
int selectMode, curPlayer, src, dst;
float delta;
signed char playerSelectInfo[41 * 4];
alignas(8) unsigned char targets[10 * 636];
}
static const char* const names[10] = {"bm", "bf", "wem", "wef", "hem", "hef", "em", "ef", "dem", "def"};
static const char* tagValue = "";
static int tagCalls;
__UINTPTR_TYPE__ hostWordAddress(unsigned word) { return reinterpret_cast<__UINTPTR_TYPE__>(names[word - 0x100u]); }
extern "C" char* findTagString__10GameObjectPci(void*, const char* tag, int fallback) {
    ++tagCalls;
    if (std::strcmp(tag, "name") != 0 || fallback != 0) return nullptr;
    return const_cast<char*>(tagValue);
}

static int bad;
struct Npc { alignas(8) unsigned char b[200]; };
static unsigned long long flagsOf(const Npc& n) { unsigned long long v; std::memcpy(&v, n.b + 160, 8); return v; }
static void expect(const char* what, int got, int want) {
    if (got != want) { std::printf("FAIL %s: got %d want %d\n", what, got, want); bad = 1; }
}

int main() {
    for (int i = 0; i < 10; ++i) { unsigned w = 0x100u + i; std::memcpy(targets + i * 636, &w, 4); }
    Npc n;
    // Modes 7 and 8 return 1 without touching the flags or asking for the tag.
    for (int mode : {7, 8}) {
        selectMode = mode; tagCalls = 0;
        std::memset(n.b, 0xff, sizeof n.b);
        expect("mode early", playerSelectionNPCVisible__FP3NPC(n.b), 1);
        expect("mode no tag", tagCalls, 0);
        expect("mode flags", flagsOf(n) == ~0ull, 1);
    }
    for (int mode : {0, 6, 9, -1}) {
        selectMode = mode;
        for (int i = 0; i <= 10; ++i) {
            tagValue = i < 10 ? names[i] : "zz";
            for (int sel = -2; sel <= 12; ++sel) {
                curPlayer = 2; std::memset(playerSelectInfo, 0, sizeof playerSelectInfo);
                playerSelectInfo[2 * 41 + 32] = static_cast<signed char>(sel);
                playerSelectInfo[1 * 41 + 32] = 5;
                std::memset(n.b, 0, sizeof n.b);
                n.b[160 + 2] = 0x20; n.b[160 + 6] = 0x5a;  // bit 0x200000 and an unrelated high bit
                const int want = i == 10 ? 1 : (static_cast<unsigned>(i - sel + 1) < 3u);
                expect("result", playerSelectionNPCVisible__FP3NPC(n.b), want);
                unsigned long long f = flagsOf(n);
                const bool set8 = i == 8 && sel < 8;
                expect("flag bit", (f & 0x200000ull) != 0, set8);
                expect("other bits", (f >> 48) == 0x5aull, 1);
            }
        }
    }
    // delta/src/dst/9 sets the bit; changing any one condition leaves it clear.
    selectMode = 0; curPlayer = 0; playerSelectInfo[32] = 20; tagValue = names[9];
    struct C { float d; int s, t; } cs[] = {{1, 7, 8}, {0, 7, 8}, {1, 6, 8}, {1, 7, 9}, {-0.5f, 7, 8}};
    for (auto& c : cs) {
        delta = c.d; src = c.s; dst = c.t;
        std::memset(n.b, 0, sizeof n.b);
        playerSelectionNPCVisible__FP3NPC(n.b);
        expect("delta case", (flagsOf(n) & 0x200000ull) != 0, c.d != 0 && c.s == 7 && c.t == 8);
    }
    tagValue = names[8]; delta = 1; src = 7; dst = 8;  // the same inputs with index 8 and selection 20: no bit
    std::memset(n.b, 0, sizeof n.b);
    playerSelectionNPCVisible__FP3NPC(n.b);
    expect("index 8 high selection", flagsOf(n) != 0, 0);
    std::printf(bad ? "FAIL\n" : "ok: npc visible\n");
    return bad;
}
