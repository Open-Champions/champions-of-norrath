// Host check for playerCheckRecallHotKey: every skip condition on its own, evaluation order, and the pad test.
// Includes the source (build the file alone).
#define NORRATH_INVENTORY_HOST 1
#include "player_check_recall_hot_key.cpp"
#include <cstring>
#include <string>
#include <vector>
unsigned char player[2 * 23984];
int gameControlPaused, localNmPlayersAlive, localPlayersAlive[4];
signed char recallOn, hudNetWaiting;
unsigned actionButton[4];
unsigned char netLocalPad[4 * 88];
static const void* table[8]; static unsigned used = 0;
static unsigned intern(const void* p) { table[used] = p; return 0x1000 + 0x10 * used++; }
const void* inventory_items::host_resolve(unsigned w) { return table[(w - 0x1000) / 0x10]; }
static std::vector<std::string> calls;
static int paused, dialog, camera[4], jump, pause_menu;
extern "C" int gameIsPaused__Fv() { calls.push_back("paused"); return paused; }
extern "C" int DLG_Active__Fv() { calls.push_back("dialog"); return dialog; }
extern "C" int netGetCameraCustomControl__Fi(int p) { calls.push_back("camera"); return camera[p]; }
extern "C" int engineTaskActive__FPc(char* n) { calls.push_back(n); return std::strcmp(n, "jumpgate") == 0 ? jump : pause_menu; }
extern "C" void msgHudRecall__Fc(char p) { calls.push_back("recall" + std::to_string(int(p))); }
static unsigned char object[1400];
static void reset() {
    std::memset(object, 0, sizeof object); std::memset(netLocalPad, 0, sizeof netLocalPad);
    gameControlPaused = 0; recallOn = hudNetWaiting = 0; paused = dialog = jump = pause_menu = 0;
    for (int& c : camera) c = 0;
    for (unsigned& a : actionButton) a = 0x10;
    localNmPlayersAlive = 1; localPlayersAlive[0] = 1; calls.clear();
    static const unsigned w = intern(object);
    std::memcpy(player + 23984 + 21872, &w, 4);
    unsigned v = 0x10; std::memcpy(netLocalPad + 88 + 8, &v, 4);
    v = 0x100; std::memcpy(netLocalPad + 88 + 12, &v, 4);
}
static int run() { calls.clear(); playerCheckRecallHotKey__Fv(); return int(calls.size()); }
int main() {
    reset();
    run();
    const std::vector<std::string> full = {"paused", "dialog", "camera", "jumpgate", "pausemenu", "recall1"};
    if (calls != full) return 1;
    int rc = 10;
    auto expect_skip = [&](const char* last) { ++rc; run(); if (calls.empty() || calls.back() != last) return false; return true; };
    reset(); gameControlPaused = 3; run(); if (!calls.empty()) return 2;
    reset(); paused = 1; if (!expect_skip("paused")) return 3;
    reset(); dialog = 1; if (!expect_skip("dialog")) return 4;
    reset(); *reinterpret_cast<short*>(object + 1188) = 1; if (!expect_skip("dialog")) return 5;
    reset(); recallOn = 1; if (!expect_skip("dialog")) return 6;
    reset(); hudNetWaiting = 2; if (!expect_skip("dialog")) return 7;
    reset(); camera[1] = 1; if (!expect_skip("camera")) return 8;
    reset(); jump = 1; if (!expect_skip("jumpgate")) return 9;
    reset(); pause_menu = 1; if (!expect_skip("pausemenu")) return 10;
    reset(); *reinterpret_cast<int*>(object + 1328) = 1; if (!expect_skip("pausemenu")) return 11;
    reset(); *reinterpret_cast<unsigned*>(object + 12) = 0x400000; if (!expect_skip("pausemenu")) return 12;
    reset(); *reinterpret_cast<unsigned*>(object + 12) = 0xffbfffffu; run(); if (calls.back() != "recall1") return 13;
    reset(); actionButton[1] = 0x20; run(); if (calls.back() == "recall1") return 14;      // pad bit does not overlap
    reset(); { unsigned v = 0xff; std::memcpy(netLocalPad + 88 + 12, &v, 4); } run(); if (calls.back() == "recall1") return 15;
    // two players: the loop re-reads the count and visits both
    reset(); localNmPlayersAlive = 2; localPlayersAlive[1] = 1; run();
    if (calls.size() != 2 * 5 + 1 + 1) return 16;      // the first entry (player 1 twice) recalls twice
    return 0;
}
