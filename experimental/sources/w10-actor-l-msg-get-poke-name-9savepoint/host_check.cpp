// Host contract for msg_getPokeName__9SavepointPUsi; not an EE input. Globals and callees are
// host stand-ins; words holding addresses are offsets from a host base. Covers the early exit,
// the player scan (first match, none, null entry pointer, zero pointee word), both key choices,
// every network/age/byte combination and the buffer being written only on the copy paths.
#define W10_ACTOR_L_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
unsigned char* w10_actor_l_host_base;
unsigned w10_actor_l_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls; static int busy, nozone;
extern "C" {
unsigned char player[23984 * 4 + 64];
int nmPlayers, nmPlayersAlive, MC_CheckPointAge; signed char netNetworkMode;
}
static unsigned short text_a[4], text_b[4];
extern "C" int MC_Busy__Fv() { calls += "busy;"; return busy; }
extern "C" int gameCheckNoSaveZone__Fv() { calls += "zone;"; return nozone; }
extern "C" unsigned short* textGetString__FPc(char* k) { calls += std::string("text(") + k + ");"; return text_a; }
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* t, int a, int b, char c) { calls += "hud(" + std::to_string(t == text_a) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ");"; }
extern "C" void ustrcpy__FPUsT0(unsigned short* d, unsigned short* s) { calls += "cpy(" + std::to_string(s == text_a) + ");"; d[0] = 0x1234; }
#include "msg_get_poke_name.cpp"
alignas(16) static unsigned char arena[0x2000];
int main() {
    w10_actor_l_host_base = arena; int bad = 0;
    unsigned char* const o = arena + 0x100;
    unsigned short out[4];
    // early exit
    calls.clear(); busy = 1; std::memset(out, 0, sizeof out);
    if (msg_getPokeName__9SavepointPUsi(o, out, 5) != 0 || calls != "busy;" || out[0]) bad = 1;
    busy = 0;
    // scan
    for (int n : {0, 1, 3, 4}) for (int match : {-1, 0, 1, 2, 3}) for (int alive : {0, 1, 2, 3, 4}) for (int nullptrcase : {0, 1}) {
        std::memset(player, 0, sizeof player); std::memset(arena, 0, sizeof arena);
        nmPlayers = n; nmPlayersAlive = alive; nozone = 1; calls.clear(); std::memset(out, 0, sizeof out);
        for (int i = 0; i < 4; ++i) {  // entries: pointer set for all, pointee word nonzero only at `match`
            const unsigned p = 0x1000 + 0x100 * i;
            if (!(nullptrcase && i == 0)) std::memcpy(player + i * 23984 + 21872, &p, 4);
            const int w = (i == match) ? 7 : 0; std::memcpy(arena + p + 1328, &w, 4);
        }
        int idx = n;
        for (int i = 0; i < n; ++i) { if (nullptrcase && i == 0) continue; if (i == match) { idx = i; break; } }
        const int r = msg_getPokeName__9SavepointPUsi(o, out, 0);
        const std::string want = std::string("busy;zone;text(") + (idx == alive ? "SaveInactive" : "SaveVehicle") + ");hud(1,180,-1,0);";
        if (r != 0 || calls != want || out[0]) bad = 1;
    }
    // no save zone not set
    for (int net : {0, 1, -1}) for (int age : {0, 1800, 1801, 99999}) for (int b : {-1, 0, 1, 255}) {
        nozone = 0; netNetworkMode = static_cast<signed char>(net); MC_CheckPointAge = age; calls.clear(); std::memset(out, 0, sizeof out);
        std::memset(arena, 0, sizeof arena); o[199] = static_cast<unsigned char>(b);
        const int r = msg_getPokeName__9SavepointPUsi(o, out, 0);
        const char* key = !net ? "SaveGame" : ((age >= 1801 && static_cast<signed char>(b) == -1) ? "SaveGameNetPlayersBusy" : "SaveGameNet");
        if (r != 1 || calls != std::string("busy;zone;text(") + key + ");cpy(1);" || out[0] != 0x1234) bad = 1;
    }
    return bad;
}
