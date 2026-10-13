// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// Every callee is a stub that appends to a trace.
#include "inventory_start.cpp"
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
signed char netNetworkMode, interfaceOn, objectDrawEnable;
int gameNoDraw, gamePause, invMsgHandle = 77, inventoryDrawDelay;
unsigned char floorDrawEnable;
void* gameCam = reinterpret_cast<void*>(0x1234);
alignas(8) unsigned char player[4 * 23984];
alignas(8) unsigned char inventory[464];
alignas(8) unsigned char statsScreen[232];
}
static std::string trace;
static int localAnswer, mutexAnswer;
extern "C" {
int netPlayerIsLocal__Fi(int p) { trace += "local" + std::to_string(p) + ";"; return localAnswer; }
int DLG_SetMutex__Fv() { trace += "mutex;"; return mutexAnswer; }
void engineSetBlur__Fi(int a) { trace += "blur" + std::to_string(a) + ";"; }
void hudClearInfoMessage__Fv() { trace += "clear;"; }
void P_Push__Fv() { trace += "push;"; }
void lightPushSceneLights__FG6Point3iiiiiiiii(Point3 d, int a, int b, int c, int e, int f, int g, int h, int i, int j) {
    trace += "light(" + std::to_string(d.x) + "," + std::to_string(d.y) + "," + std::to_string(d.z) + ")";
    for (int v : {a, b, c, e, f, g, h, i, j}) trace += "," + std::to_string(v);
    trace += ";";
}
void SetMode__6Camerai(void* c, int m) { trace += std::string("mode") + (c == gameCam ? "" : "?") + std::to_string(m) + ";"; }
void msgHudSetPlayerBusy__Fcc(char p, char b) { trace += "busy" + std::to_string(p) + "," + std::to_string(b) + ";"; }
void inventoryUnpack__Fiib(int p, int a, bool b) { trace += "unpack" + std::to_string(p) + "," + std::to_string(a) + "," + std::to_string(b) + ";"; }
void inventoryFrameRun__Fv() {}
void inventoryFrameDraw__Fv() {}
void engineAddTask__FPFv_viPci(void (*t)(), int pr, char* n, int f) {
    trace += std::string("task:") + (t == inventoryFrameRun__Fv ? "run" : t == inventoryFrameDraw__Fv ? "draw" : "?") + "," + std::to_string(pr) + "," + n + "," +
             std::to_string(f) + ";";
}
void* lumpFindResource__FPcT0(char* l, char* n) { trace += std::string("find:") + l + "," + n + ";"; return &trace; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* s, void* o, float v) {
    trace += std::string("seq") + (s == &trace ? "" : "?") + (o == nullptr ? "" : "?") + std::to_string(v) + ";";
}
int netSendMessage__FiPUci(int h, unsigned char* b, int n) {
    trace += "send" + std::to_string(h) + ":" + std::to_string(b[0]) + "," + std::to_string(b[1]) + "," + std::to_string(b[2]) + "," + std::to_string(n) + ";";
    return 0;
}
}

static int bad;
static void expect(const char* what, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, got.c_str(), want.c_str()); bad = 1; }
}
static int word(const unsigned char* base, int off) { int v; std::memcpy(&v, base + off, 4); return v; }
static void prime() {
    std::memset(inventory, 0x11, sizeof inventory);
    std::memset(statsScreen, 0x22, sizeof statsScreen);
    std::memset(player, 0x33, sizeof player);
    player[2 * 23984 + 89] = 0xee;
    interfaceOn = objectDrawEnable = 7; floorDrawEnable = 7; gameNoDraw = gamePause = inventoryDrawDelay = 0;
    trace.clear();
}
static const char* kCommon = "blur128;clear;push;light(-0.500000,-0.500000,0.500000),101,107,78,0,38,40,48,0,128;";

int main() {
    // Not local: nothing at all happens.
    prime(); localAnswer = 0; netNetworkMode = 0;
    inventoryStart__Fii(1, 1);
    expect("not local", trace, "local1;");
    expect("untouched", std::to_string(interfaceOn) + std::to_string(gamePause), "70");
    // Local, offline, mutex held: stops after the mutex.
    prime(); localAnswer = 1; mutexAnswer = 1;
    inventoryStart__Fii(1, 1);
    expect("mutex held", trace, "local1;mutex;");
    // Local, offline, no flag.
    prime(); mutexAnswer = 0;
    inventoryStart__Fii(2, 0);
    expect("offline trace", trace, std::string("local2;mutex;") + kCommon + "mode0;busy2,1;unpack2,0,1;task:run,50,inventoryRun,1;task:draw,50,inventoryDraw,1;");
    expect("globals", std::to_string(interfaceOn) + "," + std::to_string(gameNoDraw) + "," + std::to_string(floorDrawEnable) + "," +
                      std::to_string(objectDrawEnable) + "," + std::to_string(gamePause) + "," + std::to_string(inventoryDrawDelay),
           "1,1,0,0,1,2");
    expect("player word", std::to_string(word(player + 2 * 23984, 23536)) + "," + std::to_string(word(player + 1 * 23984, 23536)), "0,858993459");
    for (int r = 0; r < 4; ++r) {
        const unsigned char* rec = inventory + r * 116;
        if (r < 2) {
            expect("record", std::to_string(word(rec, 24)) + std::to_string(word(rec, 28)) + std::to_string(word(rec, 32)) + std::to_string(word(rec, 36)) +
                             std::to_string(word(rec, 40)) + "," + std::to_string(word(rec, 52)) + "," + std::to_string(word(rec, 56)) + "," +
                             std::to_string(word(rec, 76)),
                   "00000,-1,2,2");
            expect("record neighbours", std::to_string(word(rec, 20)) + "," + std::to_string(word(rec, 44)) + "," + std::to_string(word(rec, 80)),
                   "286331153,286331153,286331153");
        } else {
            expect("record untouched", std::to_string(word(rec, 52)), "286331153");
        }
    }
    // Network game: one record, no mutex, no pause, flag set: sound, message and the extra stores.
    prime(); netNetworkMode = 1;
    inventoryStart__Fii(2, 3);
    expect("network trace", trace,
           std::string("local2;") + kCommon +
               "mode0;busy2,1;unpack2,0,1;task:run,50,inventoryRun,1;task:draw,50,inventoryDraw,1;"
               "find:fx.lmp,game_lvl_complet.anm;seq1.000000;send77:9,2,238,3;");
    expect("network globals", std::to_string(gamePause) + "," + std::to_string(word(inventory, 80)) + "," + std::to_string(statsScreen[16]) + "," +
                                  std::to_string(player[2 * 23984 + 241]) + "," + std::to_string(player[2 * 23984 + 242]) + "," +
                                  std::to_string(player[2 * 23984 + 240]),
           "0,5,1,2,0,51");
    expect("network record 1 untouched", std::to_string(word(inventory + 116, 52)), "286331153");
    expect("network record 0", std::to_string(word(inventory, 52)) + "," + std::to_string(word(inventory, 56)), "-1,2");
    std::printf(bad ? "FAIL\n" : "ok: inventory start\n");
    return bad;
}
