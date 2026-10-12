#include <cstring>
#include <vector>
#include <string>
int shopRunning, gameNoDraw, gameControlPaused, gamePause, shopMouseDecay[2], norrath_shop_sell_mode_words[4];
unsigned char objectDrawEnable, floorDrawEnable;
signed char interfaceOn, netNetworkMode;
void* gameCam;
static std::vector<std::string> calls;
static void* popped;
extern "C" void shopFrame__Fv() {}
extern "C" void Pop__6Camera(void* c) { calls.push_back("pop"); popped = c; }
extern "C" void inventoryUnload__Fv() { calls.push_back("unload"); }
static void (*removed)();
extern "C" void engineRemoveTask__FPFv_v(void (*t)()) { calls.push_back("remove"); removed = t; }
extern "C" void hudNetWaitMsgReady__Fv() { calls.push_back("net"); }
extern "C" void shopKick__Fv();

static void prime() {
    calls.clear();
    shopRunning = 1; gameNoDraw = 7; gameControlPaused = 0; gamePause = 5;
    shopMouseDecay[0] = shopMouseDecay[1] = 9;
    for (int& w : norrath_shop_sell_mode_words) w = 9;
    objectDrawEnable = floorDrawEnable = 0; interfaceOn = 1;
    static int cam; gameCam = &cam; popped = 0; removed = 0;
}

int main() {
    prime(); shopRunning = 0; netNetworkMode = 0;
    shopKick__Fv();
    if (!calls.empty() || gamePause != 5 || gameNoDraw != 7) return 1;
    prime(); netNetworkMode = 0;
    shopKick__Fv();
    if (shopRunning || gameNoDraw || gameControlPaused != 60 || gamePause != 0) return 2;
    if (objectDrawEnable != 1 || floorDrawEnable != 1 || interfaceOn != 0) return 3;
    if (shopMouseDecay[0] || shopMouseDecay[1]) return 4;
    for (int w : norrath_shop_sell_mode_words) if (w) return 5;
    if (popped != gameCam || removed != shopFrame__Fv) return 6;
    if (calls != std::vector<std::string>{"pop", "unload", "remove"}) return 7;
    prime(); netNetworkMode = 2;
    shopKick__Fv();
    if (gamePause != 5) return 8;
    if (calls != std::vector<std::string>{"pop", "unload", "remove", "net"}) return 9;
    return 0;
}
