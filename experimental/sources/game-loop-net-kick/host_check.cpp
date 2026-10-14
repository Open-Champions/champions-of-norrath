// Host contract for gameNetKick. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "net_kick.cpp"

#include <cstring>
#include <string>

extern "C" {
signed char netNetworkMode = 0;
int nmPlayersAlive = 0;
int playersAlive[4];
unsigned char player[23984 * 3];
unsigned char* norrath_game_loop_host_base = nullptr;
}

static std::string log_text;
static int local_result = 0;
static int kick_offsets[4];
static int kicks = 0;

static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

extern "C" int netGetLocalPlayerNm__Fi(int player) {
    log_text += "L";
    if (player != 0) log_text += "?";
    return local_result;
}
extern "C" void msgHudSetPlayerBusy__Fcc(int player, int second) {
    log_text += "B";
    log_text += std::to_string(player);
    log_text += ":";
    log_text += std::to_string(second);
}
extern "C" void debugClearStartMenu__Fv() { log_text += "D"; }
extern "C" void invShutDown__Fi(int value) {
    log_text += "I";
    if (value != 0) log_text += "?";
}
extern "C" void hudKick__Fv() { log_text += "H"; }
extern "C" void MC_Kick__Fv() { log_text += "M"; }
extern "C" void shopKick__Fv() { log_text += "S"; }
extern "C" void Kick__7Vehicle(unsigned char* vehicle) {
    log_text += "K";
    if (kicks < 4) kick_offsets[kicks] = static_cast<int>(vehicle - norrath_game_loop_host_base);
    ++kicks;
    if (kicks == 1) nmPlayersAlive = 1;
}

int main() {
    alignas(16) unsigned char heap[4096];
    std::memset(heap, 0, sizeof heap);
    norrath_game_loop_host_base = heap;
    gameNetKick__Fi(0);
    if (!log_text.empty() || kicks != 0) return 1;

    netNetworkMode = 1;
    local_result = 0x15a;
    gameNetKick__Fi(4);
    if (log_text != "LB90:0DIHMS" || kicks != 0) return 2;

    log_text.clear();
    local_result = 0x1ff;
    nmPlayersAlive = 0;
    gameNetKick__Fi(0);
    if (log_text != "LB-1:0DIHMS" || kicks != 0) return 3;

    log_text.clear();
    kicks = 0;
    local_result = 1;
    nmPlayersAlive = 2;
    playersAlive[0] = 1;
    playersAlive[1] = 0;
    store_u32(player + 23984 + 21872, 100u);
    store_u32(heap + 100 + 1328, 0u);
    store_u32(player + 21872, 200u);
    store_u32(heap + 200 + 1328, 300u);
    gameNetKick__Fi(0);
    if (kicks != 1 || kick_offsets[0] != 300) return 4;
    if (log_text != "LB1:0DIHMSK") return 5;

    log_text.clear();
    kicks = 0;
    nmPlayersAlive = 2;
    playersAlive[0] = 0;
    playersAlive[1] = 1;
    store_u32(heap + 100 + 1328, 400u);
    gameNetKick__Fi(0);
    if (kicks != 1 || kick_offsets[0] != 300) return 6;
    return 0;
}
