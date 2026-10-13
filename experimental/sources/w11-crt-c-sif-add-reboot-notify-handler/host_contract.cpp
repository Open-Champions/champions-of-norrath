// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp sif_add_reboot_notify_handler.cpp
// Pointers are stored as their low 32 bits, the width of a target pointer.
#include <cstdint>
#include <cstring>
extern "C" {
void* g_pUserRebootBuf;
int g_iUserRebootSize;
unsigned char g_SysRebootBuf[512];
unsigned norrath_host_gp = 0x4879f0;
static int dintr_result, dintr_calls, eintr_calls, order, dintr_at, eintr_at;
static unsigned char* watched;
static unsigned word_at_dintr = 0xffffffffu;
int DIntr() { ++dintr_calls; dintr_at = ++order; if (watched) std::memcpy(&word_at_dintr, watched, 4); return dintr_result; }
int EIntr() { ++eintr_calls; eintr_at = ++order; return 0; }
int sceSifAddRebootNotifyHandler(unsigned identifier, void (*handler)(), void* data);
}
alignas(8) static unsigned char user[16 * 5];
static void fill() {
    std::memset(g_SysRebootBuf, 0xaa, sizeof g_SysRebootBuf);
    std::memset(user, 0xbb, sizeof user);
    dintr_calls = eintr_calls = order = 0;
}
static unsigned word(const unsigned char* base, unsigned entry, unsigned n) {
    unsigned v; std::memcpy(&v, base + entry * 16 + n * 4, 4); return v;
}
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static void handler() {}
static int token;
int main() {
    g_pUserRebootBuf = user; g_iUserRebootSize = 5;
    fill(); dintr_result = 1;
    if (sceSifAddRebootNotifyHandler(3, handler, &token) != 0) return 1;
    if (word(user, 3, 0) != low(reinterpret_cast<void*>(handler)) || word(user, 3, 1) != low(&token) || word(user, 3, 2) != 0x4879f0) return 2;
    for (unsigned i = 0; i < 80; ++i) if (i / 16 != 3 || i % 16 >= 12) if (user[i] != 0xbb) return 3;  // word 3 and others untouched
    for (unsigned char c : g_SysRebootBuf) if (c != 0xaa) return 4;
    if (dintr_calls != 1 || eintr_calls != 1 || dintr_at != 1 || eintr_at != 2) return 5;
    // DIntr zero: no EIntr
    fill(); dintr_result = 0;
    if (sceSifAddRebootNotifyHandler(4, handler, nullptr) != 0 || dintr_calls != 1 || eintr_calls != 0) return 6;
    if (word(user, 4, 1) != 0) return 7;
    // the stores happen after DIntr
    fill(); dintr_result = 1; watched = user + 16 * 2; word_at_dintr = 0;
    if (sceSifAddRebootNotifyHandler(2, handler, &token) != 0 || word_at_dintr != 0xbbbbbbbbu) return 8;
    watched = nullptr;
    // invalid index: marker, no interrupt calls, no change
    fill();
    if (sceSifAddRebootNotifyHandler(5, handler, &token) != static_cast<int>(0x80000069u) || dintr_calls != 0 || eintr_calls != 0) return 9;
    if (sceSifAddRebootNotifyHandler(0x7fffffff, handler, &token) != static_cast<int>(0x80000069u)) return 10;
    for (unsigned char c : user) if (c != 0xbb) return 11;
    // system table: top bit selects it and is dropped from the index
    fill(); dintr_result = 1;
    if (sceSifAddRebootNotifyHandler(0x80000005u, handler, &token) != 0 || word(g_SysRebootBuf, 5, 0) != low(reinterpret_cast<void*>(handler))) return 12;
    for (unsigned i = 0; i < 512; ++i) if (i / 16 != 5 || i % 16 >= 12) if (g_SysRebootBuf[i] != 0xaa) return 13;
    for (unsigned char c : user) if (c != 0xbb) return 14;
    fill();
    if (sceSifAddRebootNotifyHandler(0x8000001fu, handler, &token) != 0 || word(g_SysRebootBuf, 31, 1) != low(&token)) return 15;
    if (sceSifAddRebootNotifyHandler(0x80000020u, handler, &token) != static_cast<int>(0x80000069u) ||
        sceSifAddRebootNotifyHandler(0xffffffffu, handler, &token) != static_cast<int>(0x80000069u)) return 16;
    // the system count is fixed at 32 whatever the user size says
    g_iUserRebootSize = 100; fill();
    if (sceSifAddRebootNotifyHandler(0x80000020u, handler, &token) != static_cast<int>(0x80000069u) ||
        sceSifAddRebootNotifyHandler(0x80000001u, handler, &token) != 0) return 17;
    // empty user table refuses index zero; a negative size counts as huge (unsigned compare)
    g_iUserRebootSize = 0; fill();
    if (sceSifAddRebootNotifyHandler(0, handler, &token) != static_cast<int>(0x80000069u)) return 18;
    g_iUserRebootSize = -1; fill();
    if (sceSifAddRebootNotifyHandler(2, handler, &token) != 0 || word(user, 2, 1) != low(&token)) return 19;
    return 0;
}
