// Host contract for addItemToInventory__FiPQ24item11descriptioni. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
static unsigned char arena[512] __attribute__((aligned(16)));
unsigned char* norrath_host_base = reinterpret_cast<unsigned char*>(reinterpret_cast<std::uintptr_t>(arena) & ~std::uintptr_t(0xffffffffu));
#include "add_item.cpp"
static unsigned W(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
extern "C" {
unsigned char player[2 * 23984 + 4 * 4616 + 4096] __attribute__((aligned(16)));
int gameDifficulty;
}
static std::vector<std::string> calls;
static int gameState; static unsigned short textA[2], textB[2];
extern "C" {
int scriptGetGameState__FPc(char* n) { calls.push_back(std::string("get:") + n); return gameState; }
void scriptSetGameState__FPci(char* n, int v) { calls.push_back(std::string("set:") + n + "=" + std::to_string(v)); gameState = v; }
unsigned short* textGetString__FPc(char* k) { calls.push_back(std::string("text:") + k); return std::strcmp(k, "HelpfulHint") == 0 ? textA : textB; }
void hudAddHelpMessage__FiPUsT1(int i, unsigned short* a, unsigned short* b) {
    calls.push_back("help:" + std::to_string(i) + (a == textA ? ":A" : ":?") + (b == textB ? ":B" : ":?"));
}
void playerRecomputeWeight__Fi(int i) { calls.push_back("weight:" + std::to_string(i)); }
int playerGetPotionsLeft__Fii(int i, int w) { calls.push_back("potions:" + std::to_string(i) + "," + std::to_string(w)); return 0; }
}
// Items: 0x40 apart in the arena; word 0 -> name, byte 29 -> kind.
static const char* names[] = {"sword", "heart", "recallpotion", "shell"};
static unsigned itemAt(int n) { return W(arena + n * 0x20); }
static unsigned nameAt(int n) { return W(arena + 0x100 + n * 0x20); }
static void setup() {
    std::memset(arena, 0, sizeof arena);
    for (int n = 0; n < 4; ++n) {
        std::strcpy(reinterpret_cast<char*>(arena + 0x100 + n * 0x20), names[n]);
        unsigned nm = nameAt(n); std::memcpy(arena + n * 0x20, &nm, 4);
        arena[n * 0x20 + 29] = 2;   // kind 2
    }
    std::memset(player, 0, sizeof player);
    calls.clear(); gameState = 0; gameDifficulty = 0;
}
static unsigned char* slots(int index, int kind) { return player + index * 23984 + 328 + kind * 4616; }
static int& countOf(int index) { return *reinterpret_cast<int*>(slots(index, 2) + 4608); }
static void makeDesc(unsigned char* d, int item, unsigned long long flags, unsigned short qty) {
    std::memset(d, 0, 72);
    unsigned w = itemAt(item); std::memcpy(d, &w, 4);
    std::memcpy(d + 4, &qty, 2); std::memcpy(d + 8, &flags, 8);
    d[32] = 0xff; d[33] = 0xff; d[40] = 0x77; d[71] = 0x55;
}
static unsigned short hw(unsigned char* p) { unsigned short v; std::memcpy(&v, p, 2); return v; }
int main() {
    unsigned char d[72];
    // Append into an empty array.
    setup(); makeDesc(d, 0, 0, 1);
    if (addItemToInventory__FiPQ24item11descriptioni(1, d, 1) != 0) return 1;
    if (countOf(1) != 1 || std::memcmp(slots(1, 2), d, 34) != 0 || std::memcmp(slots(1, 2) + 36, d + 36, 36) != 0) return 2;
    if (hw(slots(1, 2) + 34) != 0) return 3;
    if (calls != std::vector<std::string>{"potions:1,0", "potions:1,1"}) return 4;
    // Second append gets the next index and its own number at byte 34.
    calls.clear(); makeDesc(d, 3, 0, 1);
    if (addItemToInventory__FiPQ24item11descriptioni(1, d, 0) != 1 || countOf(1) != 2) return 5;
    if (hw(slots(1, 2) + 72 + 34) != 1) return 6;
    // Stack merge into slot 1 (stackable flag).
    setup(); makeDesc(d, 0, 0x100, 2);
    for (int i = 0; i < 3; ++i) { addItemToInventory__FiPQ24item11descriptioni(0, d, 0); }
    // Each call stacks into slot 0 after the first.
    if (countOf(0) != 1 || hw(slots(0, 2) + 4) != 6) return 7;
    // Different bytes 32/33 do not merge.
    calls.clear(); d[33] = 0; if (addItemToInventory__FiPQ24item11descriptioni(0, d, 0) != 1) return 8;
    if (calls.size() != 2) return 9;
    // Merge calls recompute weight, then both potion queries, and return the slot.
    calls.clear(); d[33] = 0xff;
    if (addItemToInventory__FiPQ24item11descriptioni(0, d, 0) != 0) return 10;
    if (calls != std::vector<std::string>{"weight:0", "potions:0,0", "potions:0,1"}) return 11;
    // Non-stackable items never merge, even when identical.
    setup(); makeDesc(d, 0, 0, 1);
    addItemToInventory__FiPQ24item11descriptioni(0, d, 0);
    if (addItemToInventory__FiPQ24item11descriptioni(0, d, 0) != 1 || countOf(0) != 2) return 12;
    // Unique names refuse a second copy; others do not.
    setup(); makeDesc(d, 1, 0, 1);
    if (addItemToInventory__FiPQ24item11descriptioni(0, d, 0) != 0) return 13;
    if (addItemToInventory__FiPQ24item11descriptioni(0, d, 0) != -1 || countOf(0) != 1) return 14;
    makeDesc(d, 3, 0, 1);
    if (addItemToInventory__FiPQ24item11descriptioni(0, d, 0) != 1) return 15;
    if (addItemToInventory__FiPQ24item11descriptioni(0, d, 0) != -1) return 16;
    // The heart in a different player's array is unaffected.
    makeDesc(d, 1, 0, 1);
    if (addItemToInventory__FiPQ24item11descriptioni(1, d, 0) != 0) return 17;
    // Full array: the count word is read as the slot word and is nonzero.
    setup(); makeDesc(d, 0, 0, 1);
    countOf(0) = 64;
    if (addItemToInventory__FiPQ24item11descriptioni(0, d, 0) != -1 || countOf(0) != 64) return 18;
    // A used slot at the count position is refused.
    setup(); countOf(0) = 0; unsigned w = itemAt(0); std::memcpy(slots(0, 2), &w, 4);
    if (addItemToInventory__FiPQ24item11descriptioni(0, d, 0) != -1) return 19;
    // Recall potion hint on append: needs the announce flag and a clear state.
    setup(); makeDesc(d, 2, 0, 1);
    addItemToInventory__FiPQ24item11descriptioni(0, d, 0);
    if (!calls.empty() && calls[0] != "potions:0,0") return 20;
    setup(); makeDesc(d, 2, 0, 1);
    addItemToInventory__FiPQ24item11descriptioni(0, d, 1);
    if (calls != std::vector<std::string>{"get:PLAY_firstRecall", "set:PLAY_firstRecall=1", "text:HelpfulHint",
                                        "text:TutRecallUseBody", "help:0:A:B", "potions:0,0", "potions:0,1"}) return 21;
    calls.clear(); makeDesc(d, 2, 0, 1);
    addItemToInventory__FiPQ24item11descriptioni(0, d, 1);   // state already set
    if (calls != std::vector<std::string>{"get:PLAY_firstRecall", "potions:0,0", "potions:0,1"}) return 22;
    // Recall potion hint on merge also needs gameDifficulty == 1.
    setup(); makeDesc(d, 2, 0x100, 1);
    addItemToInventory__FiPQ24item11descriptioni(0, d, 0);
    calls.clear(); addItemToInventory__FiPQ24item11descriptioni(0, d, 1);
    if (calls != std::vector<std::string>{"weight:0", "potions:0,0", "potions:0,1"}) return 23;
    gameDifficulty = 1; calls.clear();
    addItemToInventory__FiPQ24item11descriptioni(0, d, 1);
    if (calls.size() != 8 || calls[0] != "get:PLAY_firstRecall" || calls[4] != "help:0:A:B" || calls[5] != "weight:0") return 24;
    if (hw(slots(0, 2) + 4) != 3) return 25;
    return 0;
}
