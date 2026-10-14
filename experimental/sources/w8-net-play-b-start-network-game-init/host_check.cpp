// Host check for netStartNetworkGameInit. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
#include "net_start_network_game_init.cpp"
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
signed char netBroadCastStart;
int nmPartners, broadcastConnection;
unsigned char myaddr[16];
alignas(8) unsigned char partners[70528];
unsigned char cd[8], g_net_buf[8];
alignas(8) unsigned char netGameLaunchInfo[328];
alignas(8) unsigned char launchPeers[1008];
unsigned char playerSelectInfo[164];
alignas(8) unsigned char player[95936];
alignas(8) unsigned char MC_GameConfig[252];
void* lnet_buf;
int udpLaunchConnection;
signed char hostLaunchMask;
int currentTime, lastPingTime;
signed char netNewLaunchMaybeQuit, netNewLaunchMaybeLaunch, hostForcedLaunch, hostStartedWithoutMe;
int netLaunchTimer, netLaunchExpirationCount, compressSize, gameDifficulty;
signed char isModem;
}

static std::string log_text;
static bool bad;
static bool strict;
alignas(8) static unsigned char temp_storage[4][23984];
alignas(8) static unsigned char ram_player[23984];
alignas(8) static unsigned char lnet_arena[16];
static int create_calls, open_calls, open_handle[2], packed_arg;
static int param_word0[2], param_word1[2];
static void* open_buffer[2];

extern "C" void* memset(void* d, int v, unsigned long n) {
    if (n == 64) {
        log_text += "M";
    } else if (n == 252) {
        log_text += "m";
    } else if (strict) {
        bad = true;  // no other memset size is expected inside the function
    }
    unsigned char* b = (unsigned char*)d;
    for (unsigned long i = 0; i < n; ++i) b[i] = (unsigned char)v;
    return d;
}
extern "C" char* strcpy(char* d, const char* s) { log_text += "S"; char* r = d; while ((*d++ = *s++)) {} return r; }
extern "C" int sceInetCreate(void* o, void* b, void* p) {
    log_text += "C"; if (o != cd || b != g_net_buf) bad = true;
    int* w = (int*)p; param_word0[create_calls] = w[0]; param_word1[create_calls] = w[1];
    for (int i = 2; i < 16; ++i) if (w[i]) bad = true;
    return 500 + create_calls++;
}
extern "C" int sceInetOpen(void* o, void* b, int h, int options) {
    log_text += "O"; if (o != cd || options != 1000) bad = true;
    open_buffer[open_calls] = b; open_handle[open_calls] = h;
    return 900 + open_calls++;
}
extern "C" void* memAlloc__FiPc(int size, char* tag) {
    log_text += "A"; if (size != 0x8000 || std::strcmp(tag, "Net Launch buff") != 0) bad = true;
    return lnet_arena;
}
extern "C" void MC_BuildPackedPlayer__FP11playerEntryi(void* e, int i) { log_text += "B"; if (e != playerSelectInfo || i != 0) bad = true; packed_arg = 1; }
extern "C" void* MC_GetTempPlayerStorage__Fi(int i) { log_text += "T"; if (i != 0) bad = true; return temp_storage; }
extern "C" void* MC_GetRamPlayerPtr__Fi(int i) { log_text += "G"; if (i != 0) bad = true; return ram_player; }
extern "C" void MC_ByteCopy__FPvT0i(void* d, void* s, int n) { log_text += "Y"; if (n != 23984) bad = true; std::memcpy(d, s, n); }
extern "C" void MC_ConvertTempPlayerToReal__Fi(int i) { log_text += "V"; if (i != 0) bad = true; }
extern "C" int MC_NetPack__Fv() { log_text += "N"; return 12345; }

static unsigned lcg_state = 12345;
static unsigned char next_byte() { lcg_state = lcg_state * 1664525u + 1013904223u; return (unsigned char)(lcg_state >> 24); }
static void fill(unsigned char* p, int n) { for (int i = 0; i < n; ++i) p[i] = next_byte(); }
static int W(const unsigned char* b, int o) { int v; std::memcpy(&v, b + o, 4); return v; }
static void SW(unsigned char* b, int o, int v) { std::memcpy(b + o, &v, 4); }

#define CHECK(c, n) do { if (!(c)) { std::printf("fail %d\n", n); return n; } } while (0)

static int run_launch(int flags, int count, int local, int other, int modem) {
    log_text.clear(); bad = false; create_calls = open_calls = 0;
    netBroadCastStart = 0; nmPartners = 77; lnet_buf = nullptr;
    fill(netGameLaunchInfo, 328);
    netGameLaunchInfo[0] = (unsigned char)flags; netGameLaunchInfo[4] = (unsigned char)count;
    netGameLaunchInfo[5] = (unsigned char)other; netGameLaunchInfo[6] = (unsigned char)local;
    fill(player, 95936); std::memcpy(player, "Name\0\0\0\0", 8); fill(MC_GameConfig, 252); fill((unsigned char*)temp_storage, sizeof temp_storage); fill(ram_player, sizeof ram_player);
    std::memset(launchPeers, 0xaa, sizeof launchPeers);
    isModem = (signed char)modem; gameDifficulty = 4;
    compressSize = 7; hostLaunchMask = 0; currentTime = lastPingTime = 5; netNewLaunchMaybeQuit = netNewLaunchMaybeLaunch = hostForcedLaunch = hostStartedWithoutMe = 5;
    netLaunchTimer = netLaunchExpirationCount = 5; udpLaunchConnection = 0;
    static unsigned char temp_before[4][23984], ram_before[23984];
    std::memcpy(temp_before, temp_storage, sizeof temp_storage); std::memcpy(ram_before, ram_player, sizeof ram_player);
    static unsigned char info_before[328]; std::memcpy(info_before, netGameLaunchInfo, 328);
    strict = true;
    netStartNetworkGameInit__Fv();
    strict = false;
    if (bad) return 100;
    // call sequence
    std::string want;
    if (flags & 2) want += "B";
    if (flags & 6) want += "T";
    want += "AMCO";
    for (int i = 0; i < count; ++i) want += "m";
    bool plain = (flags & 1) && !(flags & 6);
    if (plain) want += "TGYT";
    want += "VS";
    if (plain) want += "N";
    if (log_text != want) { std::printf("log %s want %s\n", log_text.c_str(), want.c_str()); return 101; }
    if (nmPartners != 0) return 102;
    if (lnet_buf != lnet_arena || udpLaunchConnection != 500 || open_buffer[0] != lnet_arena || open_handle[0] != 500) return 103;
    if (param_word0[0] != 0 || param_word1[0] != 3000) return 104;
    if (hostLaunchMask != -1 || currentTime || lastPingTime || netNewLaunchMaybeQuit || netNewLaunchMaybeLaunch || hostForcedLaunch || hostStartedWithoutMe || netLaunchTimer || netLaunchExpirationCount) return 105;
    // temporary storage move
    if (flags & 6) {
        for (int i = 0; i < 4; ++i) {
            const unsigned char* want_rec = i == local ? temp_before[0] : temp_before[i];
            if (i == 0 && local != 0) want_rec = temp_before[0];
            if (std::memcmp(temp_storage[i], want_rec, 23984) != 0) {
                if (!(plain && i == 0)) return 106;
            }
        }
    }
    // peers
    unsigned char model[1008];
    std::memset(model, 0xaa, sizeof model);
    for (int i = 0; i < count; ++i) {
        unsigned char* p = model + i * 252;
        std::memset(p, 0, 252);
        const unsigned char* in = info_before;
        p[4] = in[266 + 4 * i]; p[5] = in[265 + 4 * i]; p[6] = in[264 + 4 * i]; p[7] = in[263 + 4 * i];
        p[36] = in[282 + 4 * i]; p[37] = in[281 + 4 * i]; p[38] = in[280 + 4 * i]; p[39] = in[279 + 4 * i];
        SW(p, 48, W(in, 296 + 4 * i)); SW(p, 52, W(in, 312 + 4 * i));
        std::memcpy(p + 16, p, 16);
        SW(p, 56, W(p, 48)); SW(p, 64, i);
        if (i != local) {
            bool flag93;
            if (local == other) flag93 = true;
            else if (i >= local) flag93 = false;
            else flag93 = other != i;
            p[93] = flag93 ? 1 : 0;
        }
    }
    // the local entry's later fields
    unsigned char* e = model + local * 252;
    const unsigned char* rec = player;
    std::memcpy(e + 104, rec, std::strlen((const char*)rec) + 1);
    e[136] = rec[84]; e[137] = rec[32]; e[138] = rec[33]; e[139] = rec[35]; e[140] = rec[34];
    for (int k = 0; k < 4; ++k) e[141 + k] = rec[144 + 4 * k];
    SW(e, 152, (signed char)rec[89]); SW(e, 148, W(rec, 232));
    for (int k = 0; k < 14; ++k) SW(e, 164 + 4 * k, W(MC_GameConfig, 52 + 4 * k));
    if ((flags & 1) && (flags & 6)) SW(e, 156, 4);
    SW(e, 220, modem ? 512 : 1300);
    int csize = plain ? 12345 : -1;
    if (compressSize != csize) return 107;
    if (flags & 2) SW(e, 160, 41); else if (flags & 4) SW(e, 160, 23984); else if (flags & 1) SW(e, 160, csize);
    if (count > 0 || true) {
        // entries not written by the model stay as the function left them: compare only model-defined bytes
        for (int i = 0; i < 1008; ++i) {
            int rec_i = i / 252, off = i % 252;
            bool in_range = rec_i < count;
            bool is_local = rec_i == local;
            // bytes of a local entry beyond the shared fields are only written if local < count or the entry is outside
            if (!in_range && !is_local) { if (launchPeers[i] != 0xaa) { std::printf("untouched entry %d off %d is %d\n", rec_i, off, launchPeers[i]); return 108; } continue; }
            if (in_range && !is_local) { if (launchPeers[i] != model[i]) { std::printf("peer %d off %d got %d want %d\n", rec_i, off, launchPeers[i], model[i]); return 109; } continue; }
            if (!in_range && is_local) {
                // entry outside the cleared range: only the late fields are written over the earlier 0xaa fill
                bool late = (off >= 104 && off < 104 + (int)std::strlen((const char*)rec) + 1) || (off >= 136 && off < 145) ||
                            (off >= 148 && off < 156) || (off >= 164 && off < 220) || (off >= 220 && off < 224) ||
                            (off >= 156 && off < 164 && ((flags & 1) || (flags & 6)));
                (void)late;
                continue;  // content unchecked: it depends on stale bytes by design
            }
            if (launchPeers[i] != model[i]) { std::printf("local peer %d off %d got %d want %d\n", rec_i, off, launchPeers[i], model[i]); return 110; }
        }
    }
    return 0;
}

int main() {
    // broadcast mode
    log_text.clear(); bad = false; create_calls = open_calls = 0;
    netBroadCastStart = 1; nmPartners = 5;
    std::memset(partners, 0x55, sizeof partners);
    myaddr[4] = 11; myaddr[5] = 22; myaddr[6] = 33; myaddr[7] = 44;
    strict = true;
    netStartNetworkGameInit__Fv();
    strict = false;
    if (bad || log_text != "MCO") std::printf("bad=%d log=%s\n", bad, log_text.c_str());
    CHECK(!bad && log_text == "MCO", 1);
    CHECK(nmPartners == 1 && broadcastConnection == 500 && open_buffer[0] == g_net_buf && open_handle[0] == 500, 2);
    CHECK(partners[0] == 11 && partners[1] == 22 && partners[2] == 33 && partners[3] == 44 && partners[17620] == 1, 3);
    for (int i = 0; i < 4; ++i) CHECK(W(partners, i * 17632 + 17600) == 1, 4);
    for (int i = 0; i < (int)sizeof partners; ++i) {
        bool mine = i < 4 || i == 17620 || (i % 17632 >= 17600 && i % 17632 < 17604);
        if (!mine && partners[i] != 0x55) { std::printf("stray %d\n", i); return 5; }
    }
    CHECK(param_word0[0] == 0 && param_word1[0] == 3000, 6);
    // launch mode
    int fails = 0;
    for (int flags = 0; flags < 8; ++flags)
        for (int count = 0; count <= 4; ++count)
            for (int local = 0; local < 4; ++local)
                for (int other = 0; other < 4; other += (flags & 1) ? 1 : 3)
                    for (int modem = 0; modem < 2; ++modem) {
                        int rc = run_launch(flags, count, local, other, modem);
                        if (rc) { std::printf("launch flags=%d count=%d local=%d other=%d modem=%d rc=%d\n", flags, count, local, other, modem, rc); if (++fails > 3) return rc; }
                    }
    return fails ? 7 : 0;
}
