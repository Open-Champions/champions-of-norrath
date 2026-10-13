// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "specific_init.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cctype>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
char levelName[128];
static int cmp(const char* a, const char* b) {
    for (;; ++a, ++b) {
        const int x = std::tolower(static_cast<unsigned char>(*a)), y = std::tolower(static_cast<unsigned char>(*b));
        if (x != y) return x < y ? -1 : 1;
        if (x == 0) return 0;
    }
}
static std::string log_text;
static int roll_value = 0, oneshot = 0, path_tag = 0, slot_arg = -1;
static char kitten_tag[16] = "";
static char path_string[8] = "path";
static unsigned char heap[8];
extern "C" int strcasecmp(const char* a, const char* b) { return cmp(a, b); }
extern "C" int strticmp__FPCcT0(const char* a, const char* b) { return cmp(a, b); }
extern "C" int sprintf(char* d, const char* f, ...) {
    if (std::strcmp(f, "kitten%d") != 0) return 0;
    static int next = 0; (void)next;
    __builtin_va_list ap; __builtin_va_start(ap, f);
    const int n = __builtin_va_arg(ap, int); __builtin_va_end(ap);
    d[0] = 'k'; d[1] = 'i'; d[2] = 't'; d[3] = 't'; d[4] = 'e'; d[5] = 'n'; d[6] = static_cast<char>('0' + n); d[7] = 0;
    return 7;
}
extern "C" int eRand__10GameObject(void*) { log_text += "r"; return roll_value; }
extern "C" char* findTagString__10GameObjectPci(void*, char* tag, int) {
    log_text += "f";
    if (std::strcmp(tag, "path") == 0) return path_tag ? path_string : nullptr;
    return kitten_tag[0] ? kitten_tag : nullptr;
}
extern "C" int findTagInt__10GameObjectPci(void*, char* tag, int) { log_text += "i"; return std::strcmp(tag, "oneshot") == 0 ? oneshot : -1; }
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    log_text += "[" + std::string(lump) + ":" + name + "]";
    return heap + (name[0] & 3);
}
static void slot(void*, int arg) { log_text += "s"; slot_arg = arg; }
static unsigned char object[1300];
static unsigned char table[256];
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned long long d(unsigned o) { return rtti_c::read_u64(a() + o); }
static int bad = 0;
static void expect(bool ok, const char* msg) { if (!ok) { std::printf("fail: %s\n", msg); ++bad; } }
static void run(const char* species) {
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    rtti_c::host_install_slot(object, table, 184u, 0, reinterpret_cast<void*>(slot));
    rtti_c::host_note(heap); rtti_c::host_note(heap + 1); rtti_c::host_note(heap + 2); rtti_c::host_note(heap + 3);
    log_text.clear(); slot_arg = -1;
    char name[32], resource[] = "res";
    std::strcpy(name, species);
    specificInit__7CritterPcT1(object, name, resource);
}
int main() {
    std::strcpy(levelName + 0x51, "other");
    run("bat"); expect(w(12) == 1u && d(160) == 0x100000ull && log_text.empty(), "bat");
    run("BAT"); expect(w(12) == 1u, "case-insensitive");
    roll_value = 3; std::strcpy(kitten_tag, "Kitten2");
    run("kitten"); expect(w(1076) == 0u && w(1024) == 180u && w(1028) == 120u && w(1020) == 180u && w(244) == 4u && log_text == "f[res:jump_xy.anm]" && w(1048) == rtti_c::address_bits(heap + ('j' & 3)), "kitten tagged");
    kitten_tag[0] = 0; run("kitten"); expect(w(244) == 0u && log_text == "f[res:jump_xy.anm]", "kitten untagged");
    std::strcpy(kitten_tag, "nothing"); run("kitten"); expect(w(244) == 0u, "kitten nonmatching tag");
    run("pengo"); expect(w(1024) == 360u && w(1028) == 120u && w(1020) == 480u && log_text == "[res:pengo_cluck.anm][res:pengo_flap.anm]" && w(1052) == rtti_c::address_bits(heap + ('p' & 3)), "pengo");
    run("shark"); expect(object[40] == 40 && w(12) == 0u, "shark");
    run("orca"); expect(object[40] == 120, "orca");
    roll_value = 65536; run("crab"); expect(w(12) == 1u && w(1020) == 180u && w(1024) == 180u && w(224) == 0x40200000u && log_text == "r", "crab (1.0 + 1.5 = 2.5)");
    std::strcpy(levelName + 0x51, "kelsel"); run("raven");
    expect(d(160) == 0x2100000ull && w(1076) == 1u && w(1084) == 200u && w(1028) == 240u && log_text == "[res:hop.anm][res:flutter.anm]", "raven kelsel");
    std::strcpy(levelName + 0x51, "INTRO"); run("raven"); expect(d(160) == 0x2100000ull, "raven intro");
    std::strcpy(levelName + 0x51, "other"); run("raven"); expect(d(160) == 0x100000ull, "raven other");
    roll_value = 131072; run("fish"); expect(w(12) == 1u && w(1080) == 0x42f00000u && log_text == "r", "fish (2.0 * 60 = 120.0)");
    oneshot = 1; run("seagull");
    expect(slot_arg == 14 && log_text == "is[res:stretch.anm][res:preen.anm]" && d(160) == 0x100000ull && w(1084) == 200u && w(12) == 1u && w(1028) == 360u && w(1076) == 1u, "seagull oneshot");
    oneshot = 0; run("seagull"); expect(slot_arg == 15, "seagull without oneshot");
    run("rat"); expect(w(12) == 1u && w(224) == 0x3f999999u, "rat");
    run("bullfrog"); expect(w(12) == 1u && w(1020) == 360u && w(1024) == 360u && w(224) == 0x3f400000u, "bullfrog");
    run("catfish"); expect(w(1076) == 4u && w(12) == 1u && w(168) == 0x3e4ccccc, "catfish");
    run("fishdead"); expect(w(1076) == 4u && w(12) == 0u && w(1080) == 0x41000000u && w(168) == 0x3e4ccccc, "fishdead");
    path_tag = 1; run("drake"); expect(log_text == "f" && w(12) == 0u && d(160) == 0ull, "drake with a path");
    path_tag = 0; run("drake");
    expect(slot_arg == 15 && log_text == "fs[res:idlea.anm][res:idleb.anm]" && d(160) == 0x100000ull && w(12) == 1u && w(1076) == 1u && w(1084) == 200u && w(1028) == 360u, "drake");
    run("bflya"); expect(w(12) == 1u, "bflya");
    run("bflyb"); expect(w(12) == 1u, "bflyb");
    run("unknown"); expect(w(12) == 0u && d(160) == 0ull && object[40] == 0 && log_text.empty(), "unknown species");
    std::printf(bad ? "failed\n" : "specificInit__7CritterPcT1 passed\n");
    return bad != 0;
}
