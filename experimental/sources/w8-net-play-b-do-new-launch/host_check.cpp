// Host check for netDoNewLaunch. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
// Pointer-valued words in the entries are offsets from norrath_launch_host_base.
#define NORRATH_HOST_CONTRACT
#include "net_do_new_launch.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[1 << 20];
extern "C" unsigned char* norrath_launch_host_base = arena;
extern "C" {
alignas(8) unsigned char netGameLaunchInfo[328];
alignas(8) unsigned char launchPeers[1008];
unsigned char playerSpecStorage[164], playerSelectInfo[164];
unsigned char cd[8], g_net_buf[8];
unsigned char pad[0x160];
unsigned char player[95936];
alignas(8) unsigned char partners[70528];
void* lnet_buf;
int udpLaunchConnection, broadcastConnection, netLaunchTimer, netLaunchExpirationCount, compressSize, gameDifficulty, nmPartners;
signed char netNewLaunchMaybeQuit, netNewLaunchMaybeLaunch, hostForcedLaunch, hostLaunchMask, hostStartedWithoutMe, MC_ProtectRamSlot;
float currentTime, lastPingTime;
unsigned attackButton[4], spellButton1[4], spellButton2[4], actionButton[4], blockButton[4];
unsigned healthPotionButton[4], energyPotionButton[4], automapButton[4], weaponToggleButton[4];
}

static std::string log_text;
struct Sent { int type, parameter, sender; int limit; unsigned char block[16]; unsigned char payload[136]; };
static std::vector<Sent> sent;
static int answers[2], answer_index, recv_size;
static unsigned char recv_packet[1300], recv_from[16];
static int recv_from_length;
static std::vector<int> chunk_sizes;

extern "C" void* memset(void* d, int v, unsigned long n) {
    unsigned char* b = (unsigned char*)d; for (unsigned long i = 0; i < n; ++i) b[i] = (unsigned char)v; return d;
}
extern "C" void* memcpy(void* d, const void* s, unsigned long n) {
    unsigned char* b = (unsigned char*)d; const unsigned char* c = (const unsigned char*)s;
    for (unsigned long i = 0; i < n; ++i) b[i] = c[i]; return d;
}
extern "C" int sceInetRecvFrom(void*, void*, int, void* data, int, int*, void* address, void* length_out, int) {
    if (recv_size >= 0) {
        std::memcpy(data, recv_packet, 1300);
        std::memcpy(address, recv_from, 16);
        *static_cast<int*>(length_out) = recv_from_length;
    }
    log_text += "R";
    return recv_size;
}
extern "C" int sceInetSendTo(void*, void*, int, void* data, int length, int* flags, void* address, int limit, int last) {
    Sent s{};
    std::memcpy(&s.sender, data, 4); std::memcpy(&s.type, (unsigned char*)data + 4, 4); std::memcpy(&s.parameter, (unsigned char*)data + 8, 4);
    std::memcpy(s.payload, (unsigned char*)data + 12, 136);
    std::memcpy(s.block, address, 16);
    s.limit = limit;
    if (length != 512 || *flags != 0 || last != 100) s.type = -999;
    sent.push_back(s);
    log_text += "S";
    return 0;
}
extern "C" int sceInetClose(void*, void*, int, int) { log_text += "C"; return 0; }
extern "C" int sceInetCreate(void*, void*, void*) { log_text += "c"; return 500; }
extern "C" int sceInetOpen(void*, void*, int, int) { log_text += "o"; return 900; }
extern "C" unsigned short* textGetString__FPc(char*) { log_text += "T"; return nullptr; }
extern "C" int hudRunYesNo__FPUsiiii(unsigned short*, int, int, int, int) { log_text += "Y"; return answers[answer_index++ & 1]; }
extern "C" void showNetPlayerStatus__Fv() { log_text += "N"; }
extern "C" int peerSendDataMsg__FPciiP14sceInetAddressi(char*, int size, int, void*, int) { chunk_sizes.push_back(size); log_text += "D"; return 77; }
extern "C" void* MC_GetTempPlayerStorage__Fi(int) { return arena + 0x10000; }
extern "C" void* MC_GetRamSlot__FPi(int*) { return arena + 0x20000; }
extern "C" void* MC_GetRamPlayerPtr__Fi(int) { return arena + 0x30000; }
extern "C" void MC_BuildPackedPlayer__FP11playerEntryi(void*, int) {}
extern "C" void MC_ConvertTempPlayerToReal__Fi(int) {}
extern "C" void MC_ByteCopy__FPvT0i(void*, void*, int) {}
extern "C" void MC_NetUnPack__Fv() { log_text += "U"; }
extern "C" void MC_SetRamNmPlayers__Fi(int) {}
extern "C" void MC_ReplicateFirstPlayersImportantThings__Fv() {}
extern "C" void dumpMem__FPvi(void*, int) {}
extern "C" void memFree__FPv(void*) { log_text += "F"; }

static unsigned char* peer(int i) { return launchPeers + i * 252; }
static int& W(unsigned char* b, int o) { return *reinterpret_cast<int*>(b + o); }
static void reset(int count, int local) {
    std::memset(netGameLaunchInfo, 0, 328); std::memset(launchPeers, 0, 1008);
    netGameLaunchInfo[4] = (unsigned char)count; netGameLaunchInfo[5] = (unsigned char)local; netGameLaunchInfo[6] = (unsigned char)local;
    for (int i = 0; i < 4; ++i) W(peer(i), 64) = i;
    netNewLaunchMaybeQuit = netNewLaunchMaybeLaunch = hostForcedLaunch = hostStartedWithoutMe = 0; hostLaunchMask = 0;
    netLaunchTimer = 0; netLaunchExpirationCount = 0; currentTime = 10.0f; lastPingTime = 10.0f;
    log_text.clear(); sent.clear(); recv_size = -1; answer_index = 0; answers[0] = answers[1] = 0; chunk_sizes.clear();
    std::memset(pad, 0, sizeof pad); std::memset(player, 0xaa, sizeof player);
    for (int i = 0; i < 4; ++i) { W(peer(i), 56) = 3000; }
}
static void packet(int sender, int type, int parameter) {
    std::memset(recv_packet, 0, sizeof recv_packet);
    std::memcpy(recv_packet, &sender, 4); std::memcpy(recv_packet + 4, &type, 4); std::memcpy(recv_packet + 8, &parameter, 4);
    recv_size = 40;
    std::memset(recv_from, 7, 16); recv_from_length = 3000;
}
#define CHECK(c, n) do { if (!(c)) { std::printf("fail %d\n", n); return n; } } while (0)

int main() {
    // prompts
    reset(2, 0); netNewLaunchMaybeQuit = 1; answers[0] = 2;
    CHECK(netDoNewLaunch__Fv() == -1 && log_text == "TYCF", 1);  // cancel: close, clear players, free
    for (int i = 0; i < 4; ++i) for (int k = 0; k < 23984; ++k) if (player[i * 23984 + k] != 0) return 2;
    reset(2, 0); netNewLaunchMaybeQuit = 1; answers[0] = 1; netLaunchExpirationCount = 3000;
    CHECK(netDoNewLaunch__Fv() == 0 && netNewLaunchMaybeQuit == 0, 3);
    reset(2, 0); netNewLaunchMaybeLaunch = 1; answers[0] = 1;
    CHECK(netDoNewLaunch__Fv() == 0 && netNewLaunchMaybeLaunch == 0 && log_text == "TY", 4);
    reset(3, 0); netNewLaunchMaybeLaunch = 1; answers[0] = 2; peer(1)[92] = 8; peer(2)[92] = 3;
    CHECK(netDoNewLaunch__Fv() == 0 && hostForcedLaunch == 1 && hostLaunchMask == 3 && netNewLaunchMaybeLaunch == 0, 5);
    CHECK(log_text == "TYN", 6);

    // type 7 raises the ready flag when the parameter is the peer count minus one
    reset(3, 0); peer(1)[92] = 8; packet(1, 7, 2);
    netDoNewLaunch__Fv();
    CHECK(peer(1)[93] == 8, 7);
    reset(3, 0); peer(1)[92] = 8; packet(1, 7, 1);
    netDoNewLaunch__Fv();
    CHECK(peer(1)[93] == 0, 8);
    // type 3 moves state 2 to 3; the same state then sends a chunk
    reset(2, 0); peer(1)[92] = 2; peer(1)[100] = 1; W(peer(1), 60) = 5000; W(peer(1), 220) = 512; packet(1, 3, 0);
    netDoNewLaunch__Fv();
    CHECK(peer(1)[92] == 4 && chunk_sizes.size() == 1 && chunk_sizes[0] == 500, 9);  // 512 - 12 bytes, smaller than 5000
    CHECK(W(peer(1), 72) == 77 && W(peer(1), 76) == 500, 10);
    // a packet from the local index is ignored
    reset(2, 0); peer(1)[92] = 2; *reinterpret_cast<float*>(peer(1) + 88) = 10.0f; packet(0, 3, 0);
    netDoNewLaunch__Fv();
    CHECK(peer(1)[92] == 2, 11);
    // a join (type 0) from a peer in state 0 stores its block, answers with type 1 and the local block
    reset(2, 1); peer(0)[92] = 0; W(peer(0), 160) = 5; W(peer(1), 160) = 7;
    packet(0, 0, 123);
    std::memset(recv_packet + 12, 0x5a, 136);
    netDoNewLaunch__Fv();
    CHECK(peer(0)[92] == 5 && peer(0)[100] == 1 && peer(0)[104] == 0x5a, 12);
    CHECK(sent.size() >= 1 && sent[0].type == 1 && sent[0].parameter == 123 && sent[0].sender == 1 && sent[0].limit == 3000, 13);
    // state 1 sends the size (type 2) and moves to state 2
    reset(2, 0); peer(1)[92] = 1; netGameLaunchInfo[0] = 2; currentTime = 10.0f;
    netDoNewLaunch__Fv();
    CHECK(peer(1)[92] == 2 && W(peer(1), 60) == 41 && sent.size() == 1 && sent[0].type == 2 && sent[0].parameter == 41, 14);
    // everybody ready, timer reaches 120: positive result (launch)
    reset(2, 0); peer(1)[92] = 8; peer(1)[93] = 8; netLaunchTimer = 119; hostLaunchMask = 3;
    std::memset(netGameLaunchInfo + 263, 0, 60);
    int r = netDoNewLaunch__Fv();
    CHECK(r == 0 || r == 1 || r == 2, 15);
    reset(2, 0); peer(1)[92] = 8; peer(1)[93] = 8; netLaunchTimer = 120; hostLaunchMask = 3; netGameLaunchInfo[0] = 1;
    r = netDoNewLaunch__Fv();
    CHECK(r == 1 || r == 2, 16);
    CHECK(log_text.find('F') != std::string::npos && nmPartners == 2, 17);
    return 0;
}
