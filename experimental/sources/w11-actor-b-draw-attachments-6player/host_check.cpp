// Host contract for drawAttachments__6PlayerP11_drawRecordf; not an EE input. Build with the host compiler
// and -fno-builtin (strstr and strlen are called by name). Words are offsets from w11_actor_b_host_base.
#define W11_ACTOR_B_HOST 1
#include "draw_attachments.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char* w11_actor_b_host_base = nullptr;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
int charAnimTest, levelFlags, gamePlayersPresent;
signed char interfaceOn;
unsigned char armorCache[3520];
unsigned char player[4 * 23984];
unsigned char globalItem[0x6a0];
static unsigned char arena[0x10000] __attribute__((aligned(16)));
static std::string calls;
static int proxyIds[8], proxyIndex;
static unsigned long long chunksResult = 0x1122334455667788ull;
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string off(const void* p) { return p ? std::to_string(static_cast<const unsigned char*>(p) - arena) : std::string("null"); }
extern "C" {
char* find_text(const char* h, const char* n) __asm__("strstr");
char* find_text(const char* h, const char* n) { calls += std::string("find(") + h + "," + n + ");"; return const_cast<char*>(std::strstr(h, n)); }
unsigned text_length(const char* t) __asm__("strlen");
unsigned text_length(const char* t) { return static_cast<unsigned>(std::strlen(t)); }
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int s) { calls += "chunks(" + off(m) + "," + std::to_string(s) + ");"; return chunksResult; }
void getPicName__4itemPciPQ24item11description(char* b, int v, void* d) { std::strcpy(b, "armorpiece.xyz"); calls += "pic(" + std::to_string(v) + "," + off(d) + ");"; }
void cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(char* n, void* e, void** m, void** t) {
    calls += std::string("cache(") + n + "," + std::to_string(static_cast<unsigned char*>(e) - armorCache) + ");";
    *m = arena + 0x8000; *t = arena + 0x8100;
}
int getProxyMeshId__4itemPQ24item11description(void* d) { calls += "proxy(" + off(d) + ");"; return proxyIds[proxyIndex++ & 7]; }
void setupHalo__4itemPQ24item11descriptionf(void* d, float s) { calls += "setup(" + off(d) + "," + f(s) + ");"; }
void modelSetAttachmentFlip__Fv() { calls += "flip;"; }
void modelDrawAttachment(void* m, void* t, int a, void* body, void* rec, unsigned long long chunks, int flip, int d) {
    calls += "att(" + off(m) + "," + off(t) + "," + std::to_string(a) + "," + off(body) + "," + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(rec)) + "," + std::to_string(chunks) + "," + std::to_string(flip) + "," + std::to_string(d) + ");";
}
void modelResetOptionalParams__Fv() { calls += "reset;"; }
void modelSetHalo__FP7HaloDefi(void* t, int c) { calls += "halo(" + off(t) + "," + std::to_string(c) + ");"; }
}
static void set_word(unsigned o, unsigned v) { std::memcpy(arena + o, &v, 4); }
static void set_half(unsigned o, short v) { std::memcpy(arena + o, &v, 2); }
static void set_player_word(int i, unsigned o, unsigned v) { std::memcpy(player + i * 23984 + o, &v, 4); }
static unsigned long long get_dword(unsigned o) { unsigned long long v; std::memcpy(&v, arena + o, 8); return v; }
static const unsigned DESC = 0x1000, DEF = 0x2000, STRINGS = 0x3000, MODELS = 0x4000, EQUIP = 0x5000, EQUIPDEF = 0x5100;
static unsigned str(const char* s, unsigned slot) { std::strcpy(reinterpret_cast<char*>(arena + STRINGS + 0x40 * slot), s); return STRINGS + 0x40 * slot; }

static void reset() {
    std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player); std::memset(armorCache, 0, sizeof armorCache); std::memset(globalItem, 0, sizeof globalItem);
    w11_actor_b_host_base = arena;
    charAnimTest = 0; levelFlags = 0; gamePlayersPresent = 1; interfaceOn = 0;
    std::memset(proxyIds, 0, sizeof proxyIds); proxyIndex = 0;
    set_word(960, 2); set_word(1332, 0x6000);
    calls.clear();
}
// slot i (0..15) holds description number n (0..3); def[28] = category
static void give(int slot, unsigned n, int category, unsigned nameWord, unsigned word48, short half52 = 0) {
    set_player_word(2, 21896 + 4 * slot, DESC + 0x20 * n);
    set_word(DESC + 0x20 * n, DEF + 0x80 * n);
    arena[DEF + 0x80 * n + 28] = static_cast<unsigned char>(category);
    set_word(DEF + 0x80 * n, nameWord); set_word(DEF + 0x80 * n + 48, word48); set_half(DEF + 0x80 * n + 52, half52);
}
#define RUN() drawAttachments__6PlayerP11_drawRecordf(arena, reinterpret_cast<void*>(0x777), 2.5f)
#define CHECK(cond, msg) do { if (!(cond)) { std::puts(calls.c_str()); std::puts("fail: " msg); return 1; } } while (0)

int main() {
    // nothing equipped
    reset(); RUN();
    CHECK(calls == "halo(null,0);reset;" && get_dword(552) == ~0ull, "empty");
    reset(); charAnimTest = 273; RUN();
    CHECK(calls == "chunks(24576,1);halo(null,0);reset;" && get_dword(552) == chunksResult, "chunks when charAnimTest >= 273");
    reset(); charAnimTest = 272; RUN();
    CHECK(calls.find("chunks") == std::string::npos, "charAnimTest 272");
    // slot 4: equipment cache (offset 0 + 88 * 2), then the draw and the bit clearing
    reset(); give(4, 1, 1, str("plain", 0), str("whatever", 1)); std::memset(arena + 0x8000, 0xff, 8);
    RUN();
    CHECK(calls.find("pic(0,4128);cache(armorpiece,176);setup(4128,2.5);att(32768,33024,1,24576,1911,18446744073709551615,-1,0);") != std::string::npos, "cache path");
    CHECK(get_dword(552) == 0, "clearing from the model's first doubleword");
    // slot 4 without def+48 leaves model null: nothing is drawn
    reset(); give(4, 1, 1, str("plain", 0), 0); RUN();
    CHECK(calls == "halo(null,0);reset;", "no def+48");
    // skip categories
    for (int c : {5, 6, 7, 8, 14}) { reset(); give(4, 1, c, str("plain", 0), str("x", 1)); RUN(); CHECK(calls == "halo(null,0);reset;", "skipped category"); }
    reset(); levelFlags = 0x10000000; give(4, 1, 2, str("plain", 0), str("x", 1)); RUN();
    CHECK(calls == "halo(null,0);reset;", "category 2 skipped with level bit");
    reset(); levelFlags = 0x10000000; interfaceOn = 1; give(4, 1, 2, str("plain", 0), str("x", 1)); RUN();
    CHECK(calls.find("att(") != std::string::npos, "interfaceOn keeps category 2");
    // weapon check for slot 3
    reset(); give(3, 1, 1, str("plain", 0), str("x", 1)); gamePlayersPresent = 0; RUN();
    CHECK(calls == "halo(null,0);reset;", "slot 3 with no players");
    reset(); give(3, 1, 1, str("plain", 0), str("x", 1));
    set_player_word(2, 21960, DESC + 0x20 * 2); set_word(DESC + 0x40, EQUIPDEF); arena[EQUIPDEF + 30] = 4; RUN();
    CHECK(calls.find("att(") == std::string::npos, "weapon with byte 4 hides slot 3");
    reset(); give(3, 1, 1, str("plain", 0), str("x", 1));
    set_player_word(2, 21960, DESC + 0x20 * 2); set_word(DESC + 0x40, EQUIPDEF); arena[EQUIPDEF + 30] = 3; RUN();
    CHECK(calls.find("att(") != std::string::npos && calls.find("cache(armorpiece,3344)") != std::string::npos, "weapon with another byte");
    // slot 0: category-B checks and the global item table
    reset(); give(0, 1, 1, str("plain", 0), 0, 2);
    { unsigned m = 0x4000, t = 0x4100; std::memcpy(globalItem + 8 + 32, &m, 4); std::memcpy(globalItem + 12 + 32, &t, 4); }
    RUN();
    CHECK(calls.find("setup(4128,2.5);att(16384,16640,1,24576,1911,18446744073709551615,-1,0);") != std::string::npos, "global item table");
    reset(); give(0, 1, 1, str("plain", 0), 0, 2); arena[964] = 1; RUN();
    CHECK(calls == "halo(null,0);reset;", "+964 blocks slot 0");
    reset(); give(0, 1, 1, str("plain", 0), 0, 2); set_word(1328, 1); RUN();
    CHECK(calls == "halo(null,0);reset;", "+1328 blocks slot 0");
    reset(); give(3, 1, 1, str("plain", 0), 0, 0); arena[964] = 1; { unsigned m = 0x4200; std::memcpy(globalItem + 8, &m, 4); unsigned t = 0x4300; std::memcpy(globalItem + 12, &t, 4); } RUN();
    CHECK(calls.find("att(16896,17152,") != std::string::npos, "slot 3 ignores +964");
    reset(); give(1, 1, 1, str("plain", 0), str("x", 1)); gamePlayersPresent = 0; RUN();
    CHECK(calls == "halo(null,0);reset;", "slot 1 with no players");
    // slot 1 flips: flip value 1, and the flip call when kind is 2
    reset(); give(1, 1, 1, str("plain", 0), str("x", 1)); set_player_word(2, 84, 2); RUN();
    CHECK(calls.find("flip;att(32768,33024,1,24576,1911,18446744073709551615,1,0);reset;") != std::string::npos, "flip");
    reset(); give(1, 1, 1, str("plain", 0), str("x", 1)); set_player_word(2, 84, 3); RUN();
    CHECK(calls.find("flip;") == std::string::npos && calls.find("att(32768,33024,1,24576,1911,18446744073709551615,1,0);reset;") != std::string::npos, "no flip for other kinds");
    // default (proxy) path: slots 5..8
    reset(); give(5, 1, 1, str("plain", 0), 0); proxyIds[0] = 1; proxyIds[1] = 2;
    { unsigned m = 0x4400, t = 0x4500; std::memcpy(globalItem + 8 + 16, &m, 4); std::memcpy(globalItem + 12 + 32, &t, 4); }
    RUN();
    CHECK(calls.find("proxy(4128);proxy(4128);setup(4128,2.5);att(17408,17664,") != std::string::npos, "proxy path");
    reset(); give(5, 1, 1, str("plain", 0), 0); arena[964] = 1; proxyIds[0] = 1;
    { unsigned m = 0x4400, t = 0x4500; std::memcpy(globalItem + 8 + 16, &m, 4); std::memcpy(globalItem + 12 + 16, &t, 4); }
    RUN();
    CHECK(calls.find("proxy(") == std::string::npos, "proxy path blocked by +964");
    reset(); give(5, 1, 1, str("plain", 0), 0); arena[964] = 1; interfaceOn = 1; proxyIds[0] = 1; proxyIds[1] = 1;
    { unsigned m = 0x4400, t = 0x4500; std::memcpy(globalItem + 8 + 16, &m, 4); std::memcpy(globalItem + 12 + 16, &t, 4); }
    RUN();
    CHECK(calls.find("proxy(4128);proxy(4128);") != std::string::npos, "proxy path with the interface on");
    // type 3 selects chunks: shift table
    const int shifts[10] = {1, 0, 9, 8, 7, 6, 5, 4, 3, 2};
    for (unsigned kind = 0; kind < 11; ++kind) {
        reset(); give(4, 1, 3, str("plain", 0), str("x", 1)); set_player_word(2, 84, kind); RUN();
        const int s = kind < 10 ? shifts[kind] : 0;
        CHECK(calls.find("chunks(32768," + std::to_string(1 << s) + ");") != std::string::npos, "chunk shift");
        CHECK(calls.find("," + std::to_string(chunksResult) + ",-1,0);") != std::string::npos, "chunks passed on");
    }
    // hide rules and the extra pieces
    reset(); give(4, 1, 11, str("helmet", 0), str("x", 1)); set_word(1336, 0x7000); set_word(1352, 0x7100); set_word(1340, 0x7200); set_word(1348, 0x7300);
    RUN();   // kind 0: category 11 without "circlet" in the name: hide = 1
    CHECK(calls.find("halo(") == std::string::npos && calls.find("att(28672,") == std::string::npos && calls.find("att(29184,") == std::string::npos, "hide skips the extras");
    reset(); give(4, 1, 11, str("my circlet", 0), str("x", 1)); set_word(1336, 0x7000); set_word(1352, 0x7100); set_word(1340, 0x7200); set_word(1348, 0x7300);
    RUN();
    CHECK(calls.find("halo(null,0);att(28672,28928,1,24576,1911,18446744073709551615,-1,0);att(29184,29440,1,24576,1911,18446744073709551615,-1,0);reset;") != std::string::npos, "extras drawn");
    reset(); give(4, 1, 4, str("plain", 0), str("cloth3body", 1)); set_player_word(2, 84, 7); set_word(1336, 0x7000); set_word(1352, 0x7100);
    RUN();
    CHECK(calls.find("halo") == std::string::npos, "kind 7 with cloth3body hides");
    reset(); give(4, 1, 4, str("chain mail", 0), str("x", 1)); set_player_word(2, 84, 9); set_word(1336, 0x7000); set_word(1352, 0x7100);
    RUN();
    CHECK(calls.find("halo") == std::string::npos, "kind 9 chain hides");
    reset(); give(4, 1, 1, str("chain mail", 0), str("x", 1)); set_player_word(2, 84, 9); set_word(1336, 0x7000); set_word(1352, 0x7100);
    RUN();
    CHECK(calls.find("halo(null,0)") != std::string::npos, "kind 9 chain without byte 4 does not hide");
    // tint: leather helmet at R+21940 re-enables the second extra when hide is set
    reset(); give(4, 1, 11, str("helmet", 0), str("x", 1)); set_word(1340, 0x7200); set_word(1348, 0x7300);
    set_player_word(2, 21940, DESC + 0x20 * 2); set_word(DESC + 0x40, EQUIPDEF); set_word(EQUIPDEF, str("leatherhelmet cap", 5));
    RUN();
    CHECK(calls.find("att(29184,29440,") != std::string::npos, "leatherhelmet draws the second extra even when hidden");
    reset(); give(4, 1, 11, str("helmet", 0), str("x", 1)); set_word(1340, 0x7200); set_word(1348, 0x7300);
    set_player_word(2, 21940, DESC + 0x20 * 2); set_word(DESC + 0x40, EQUIPDEF); set_word(EQUIPDEF, str("other", 5)); set_player_word(2, 84, 6);
    RUN();
    CHECK(calls.find("att(29184,29440,") != std::string::npos, "kind 6 re-enables the second extra");
    reset(); give(4, 1, 11, str("helmet", 0), str("x", 1)); set_word(1340, 0x7200); set_word(1348, 0x7300);
    set_player_word(2, 21940, DESC + 0x20 * 2); set_word(DESC + 0x40, EQUIPDEF); set_word(EQUIPDEF, str("other", 5));
    RUN();
    CHECK(calls.find("att(29184,29440,") == std::string::npos, "no re-enable");
    return 0;
}
