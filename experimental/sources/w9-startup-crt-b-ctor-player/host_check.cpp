// Host check only; not an EE build input. Build with -DNORRATH_HOST_CHECK (the
// source then keeps 32-bit handles in its pointer-sized object fields). Callees
// are recorders. The same stores, call order and arguments were compared with the
// original in a private interpreter run: 400 random scenarios (index, flag,
// player count, level flags, race and animation codes, random object and table
// contents) gave identical call logs and identical changed bytes, covering every
// instruction of the original except padding no-ops.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#define NORRATH_HOST_CHECK 1
#define memset recorded_memset  // the source declares and calls these two library
#define sprintf recorded_sprintf  // functions; keep the host ones out of the way
#include "player.cpp"
#undef memset
#undef sprintf

// ---- handle registry -------------------------------------------------------
static std::vector<const void*> registry{nullptr};
extern "C" unsigned norrath_host_handle(const void* pointer) {
    if (!pointer) return 0;
    for (unsigned i = 1; i < registry.size(); ++i) if (registry[i] == pointer) return i;
    registry.push_back(pointer);
    return static_cast<unsigned>(registry.size() - 1);
}
extern "C" void* norrath_host_pointer(unsigned handle) { return const_cast<void*>(registry.at(handle)); }

// ---- data the source imports ----------------------------------------------
extern "C" {
unsigned char player[4 * 23984];
char* racePrefix[10];
void* typeToResource[10];
char* selectLumpName;
unsigned levelFlags;
int nmPlayers;
char thereIsACutPlayer;
char coreForceSmallCharacters;
void* world;
unsigned char _vt$6Player[4];
float norrath_w9_startup_crt_b_view_4801f8[6];
char norrath_w9_startup_crt_b_view_464ab8[4], norrath_w9_startup_crt_b_view_4644c0[4], norrath_w9_startup_crt_b_view_464ac8[4],
    norrath_w9_startup_crt_b_view_464ad8[4], norrath_w9_startup_crt_b_view_464ae8[4], norrath_w9_startup_crt_b_view_464af8[4],
    norrath_w9_startup_crt_b_view_464b10[4], norrath_w9_startup_crt_b_view_464b20[4], norrath_w9_startup_crt_b_view_464b30[4],
    norrath_w9_startup_crt_b_view_464b48[4], norrath_w9_startup_crt_b_view_464b58[4], norrath_w9_startup_crt_b_view_464b68[4],
    norrath_w9_startup_crt_b_view_464b78[4], norrath_w9_startup_crt_b_view_464b88[4], norrath_w9_startup_crt_b_view_464b98[4];
}

static std::string calls;
static unsigned char object[1600];
static unsigned char table[2048];
static unsigned char blocks[12][64];
static char race_text[4], cut_text[4];
static unsigned char owner[64], holder[16];
static char holder_name[4];
static char lump_tokens[16];
static int lump_count;
static char buffer_seen[16][32];

static const char* fmt_name(const char* f) {
    if (f == norrath_w9_startup_crt_b_view_464ac8) return "ac8";
    if (f == norrath_w9_startup_crt_b_view_464ad8) return "ad8";
    if (f == norrath_w9_startup_crt_b_view_464ae8) return "ae8";
    if (f == norrath_w9_startup_crt_b_view_464af8) return "af8";
    if (f == norrath_w9_startup_crt_b_view_464b10) return "b10";
    if (f == norrath_w9_startup_crt_b_view_464b20) return "b20";
    return "?";
}
static const char* lump_name(const char* s) {
    if (s == norrath_w9_startup_crt_b_view_464b48) return "b48";
    if (s == norrath_w9_startup_crt_b_view_464b58) return "b58";
    if (s == norrath_w9_startup_crt_b_view_464b68) return "b68";
    if (s == norrath_w9_startup_crt_b_view_464b78) return "b78";
    if (s == norrath_w9_startup_crt_b_view_464b88) return "b88";
    if (s == norrath_w9_startup_crt_b_view_464b98) return "b98";
    return "buf";
}

extern "C" {
unsigned __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc(void* self, Point3 p, short t, void* res, float scale, char** args) {
    char text[120];
    std::snprintf(text, sizeof text, "creature(%d,%g,%g,%g,%d,%d,%g,%d);", self == object, p.x, p.y, p.z, t,
                  res == static_cast<void*>(&typeToResource[4]), scale, args == nullptr);
    calls += text;
    return 0;
}
void lightAdd__FP5Light(void* l) { calls += l == object + 1208 ? "lightAdd;" : "lightAdd?;"; }
void lightDeactivate__FP5Light(void* l) { calls += l == object + 1208 ? "lightOff;" : "lightOff?;"; }
int recorded_sprintf(char* buffer, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    char* prefix = va_arg(ap, char*);
    char text[100];
    int a = -1, b = -1;
    if (format != norrath_w9_startup_crt_b_view_464b10) a = va_arg(ap, int);
    if (format == norrath_w9_startup_crt_b_view_464ad8 || format == norrath_w9_startup_crt_b_view_464af8) b = va_arg(ap, int);
    va_end(ap);
    std::snprintf(text, sizeof text, "sprintf(%s,%d,%d,%d);", fmt_name(format), prefix == race_text, a, b);
    calls += text;
    std::snprintf(buffer, 32, "%s", fmt_name(format));
    return 1;
}
void* lumpFindResource__FPcT0(char* lump, char* name) {
    char text[100];
    std::snprintf(text, sizeof text, "lump(%s,%s);", lump == norrath_w9_startup_crt_b_view_4644c0 ? "small" : lump == norrath_w9_startup_crt_b_view_464ab8 ? "pair" : lump == holder_name ? "holder" : "?", lump_name(name));
    calls += text;
    return &lump_tokens[lump_count++];
}
int scePrintf(const char* f, ...) { calls += f == norrath_w9_startup_crt_b_view_464b30 ? "sce;" : "sce?;"; return 0; }
void animInitState__FP14AnimationState(void* s) { calls += s == object + 288 ? "animInit;" : "animInit?;"; }
void resetPlayerAnimations__Fi(int i) { calls += "reset(" + std::to_string(i) + ");"; }
unsigned objectAddToRunList__FP10GameObject(void* o) { calls += o == object ? "run;" : "run?;"; return 0; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, Point3 p, float h) {
    char text[100];
    std::snprintf(text, sizeof text, "height(%d,%g,%g,%g,%g);", w == world, p.x, p.y, p.z, h);
    calls += text;
    return 7.5f;
}
void playerRecomputeAbilityModifiers__Fv() { calls += "mods;"; }
void* recorded_memset(void* d, int v, unsigned n) {
    char text[80];
    std::snprintf(text, sizeof text, "memset(%ld,%d,%u);", static_cast<unsigned char*>(d) - player, v, n);
    calls += text;
    return d;
}
void enumerateAlivePlayers__Fv() { calls += "alive;"; }
}

static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\ncalls: %s\n", what, calls.c_str()); ++failures; } }
static unsigned word(int off) { unsigned v; std::memcpy(&v, object + off, 4); return v; }
static unsigned half(int off) { unsigned short v; std::memcpy(&v, object + off, 2); return v; }
static float flt(int off) { float v; std::memcpy(&v, object + off, 4); return v; }
static unsigned float_bits(float f) { unsigned v; std::memcpy(&v, &f, 4); return v; }
static void put_word(unsigned char* base, int off, unsigned v) { std::memcpy(base + off, &v, 4); }

static void prepare(int index, signed char c0, signed char c1, signed char c2, signed char c3, int kind) {
    std::memset(object, 0x33, sizeof object);
    std::memset(player, 0x44, sizeof player);
    std::memset(table, 0, sizeof table);
    std::memset(blocks, 0, sizeof blocks);
    calls.clear();
    lump_count = 0;
    unsigned char* record = player + index * 23984;
    record[32] = static_cast<unsigned char>(c0);
    record[33] = static_cast<unsigned char>(c1);
    record[34] = static_cast<unsigned char>(c2);
    record[35] = static_cast<unsigned char>(c3);
    put_word(record, 84, 3);
    record[23582] = static_cast<unsigned char>(kind);
    racePrefix[3] = race_text;
    typeToResource[3] = &typeToResource[4];
    put_word(table, 0, norrath_host_handle(holder_name));
    put_word(object, 36, norrath_host_handle(table));
    for (int k = 0; k < 6; ++k) {
        put_word(table, k * 104 + 76, norrath_host_handle(blocks[k * 2]));
        put_word(table, k * 104 + 92, norrath_host_handle(blocks[k * 2 + 1]));
    }
    put_word(record, 21872, norrath_host_handle(owner));
    put_word(owner, 36, norrath_host_handle(holder));
    put_word(holder, 0, norrath_host_handle(holder_name));
    const float position[4] = {4.0f, 5.0f, 6.0f, 99.0f};
    std::memcpy(object + 16, position, 16);
    for (int i = 0; i < 6; ++i) norrath_w9_startup_crt_b_view_4801f8[i] = 0.5f + i;
    thereIsACutPlayer = 0;
}

int main() {
    // Scenario A: index 2, flag 0, no race codes except the last, animation kind 1.
    prepare(2, -1, 0, -1, 4, 1);
    levelFlags = 0;
    nmPlayers = 1;
    coreForceSmallCharacters = 0;
    __6PlayerG6Point3sii(object, Point3{1.0f, 2.0f, 3.0f}, 0x1fffd, 2, 0);
    expect(calls ==
               "creature(1,1,2,3,-3,1,13,1);lightAdd;lightOff;sprintf(b10,1,-1,-1);lump(small,buf);"
               "sprintf(b20,1,5,-1);lump(small,buf);sce;animInit;reset(2);lump(holder,b48);run;"
               "height(1,4,5,6,12);mods;memset(71552,0,64);memset(71616,0,64);alive;",
           "scenario A call sequence");
    unsigned char* record = player + 2 * 23984;
    expect(selectLumpName == norrath_w9_startup_crt_b_view_4644c0, "small lump name chosen");
    expect(thereIsACutPlayer == 0, "no cut player without the flag");
    expect(word(72) == norrath_host_handle(_vt$6Player), "vtable word");
    expect(object[1106] == 0 && object[40] == 48 && static_cast<int>(word(1176)) == -1, "flag byte, 48 and -1");
    expect(word(1340) == 0 && word(1348) == 0, "second pair cleared");
    expect(word(1332) == norrath_host_handle(&lump_tokens[0]) && word(1344) == norrath_host_handle(&lump_tokens[1]), "plain lumps stored");
    expect(word(296) == norrath_host_handle(&lump_tokens[2]), "animation lump stored at 296");
    expect(*reinterpret_cast<unsigned*>(player + 2 * 23984 + 21872) == norrath_host_handle(object), "record points back at the object when the flag is clear");
    expect(word(12) == (0x33333333u | 0x1004040u) && word(960) == 2, "flags word and index");
    expect(half(1186) == 9000 && flt(1120) == 3.5f && object[1105] == 1 && word(1400) == 2, "index-based fields");
    expect(flt(168) == 2.5f && flt(28) == 12.0f && flt(24) == 7.5f, "floats at 168, 28, 24");
    expect(flt(304) == 1.0f && flt(312) == 1.0f && half(324) == 4613 && object[292] == 4, "animation constants");
    expect(half(356) == 4613 && half(388) == 4613 && half(420) == 4613, "halves of 4613");
    expect(flt(1236) == 300.0f, "light range stays 300.0 when the light is off");
    expect(word(88) == 0 && word(104) == 0 && word(108) == 0 && word(136) == 0 && word(896) == 0 && word(956) == 0, "cleared ranges");
    expect(word(84) == 0x33333333u && word(140) == 0x33333333u && word(892) == 0x33333333u, "cleared ranges stop at their ends");
    expect(half(1208 + 10) == 188 && half(1208 + 16) == 0xffc0u && flt(1208 + 28) == flt(1208 + 36), "light fields");
    expect(word(1256) == float_bits(1.0f) && word(1264) == float_bits(3.0f), "position copy at 1256");
    expect(object[1361] == 1 && word(1396) == 0 && object[1276] == 0 && object[1277] == 0, "bytes near 1361");
    for (int k = 0; k < 6; ++k) {
        unsigned char* a = blocks[k * 2];
        unsigned char* b = blocks[k * 2 + 1];
        float a28, a20, b28, b20;
        std::memcpy(&a28, a + 28, 4); std::memcpy(&a20, a + 20, 4); std::memcpy(&b28, b + 28, 4); std::memcpy(&b20, b + 20, 4);
        expect(a[24] == 1 && a[32] == 1 && b[24] == 4 && b[32] == 4, "block bytes");
        expect(a28 == 0.75f && a20 == 0.25f && b28 == 5.5f && b20 == 4.5f, "block floats");
    }
    expect(record[23968] == 0, "record byte 23968 cleared");
    expect(record[23967] == 0x44, "record byte 23967 untouched");

    // Scenario B: two players, level flags with the halving bit, race codes present, flag set.
    prepare(1, 0, 2, 1, 4, 3);
    levelFlags = 0x24800000u;
    nmPlayers = 2;
    coreForceSmallCharacters = 0;
    __6PlayerG6Point3sii(object, Point3{1.0f, 2.0f, 3.0f}, 7, 1, 1);
    expect(calls ==
               "creature(1,1,2,3,7,1,13,1);lightAdd;sprintf(ac8,1,1,-1);lump(pair,buf);sprintf(ad8,1,1,99);lump(pair,buf);"
               "sprintf(ae8,1,2,-1);lump(pair,buf);sprintf(af8,1,2,99);lump(pair,buf);sprintf(b10,1,-1,-1);lump(pair,buf);"
               "sprintf(b20,1,5,-1);lump(pair,buf);animInit;reset(1);lump(holder,b68);run;"
               "height(1,4,5,6,12);mods;memset(47568,0,64);memset(47632,0,64);alive;",
           "scenario B call sequence");
    expect(selectLumpName == norrath_w9_startup_crt_b_view_464ab8, "pair lump name for two players");
    expect(thereIsACutPlayer == 1 && object[1106] == 1, "cut player flag");
    expect(flt(1236) == 252.0f && flt(1240) == 1.5f, "floats set when the light stays on");
    expect(half(1218) == 94 && half(1220) == 94 && half(1222) == 94 && half(1224) == 0xffe0u, "light halves halved");
    expect(word(1336) == norrath_host_handle(&lump_tokens[0]) && word(1352) == norrath_host_handle(&lump_tokens[1]) &&
               word(1340) == norrath_host_handle(&lump_tokens[2]) && word(1348) == norrath_host_handle(&lump_tokens[3]),
           "four lump results at their offsets");
    expect(half(1186) == 1 * 2700 + 3600, "index-based half");

    // Scenario C: the level flag that takes the table row 0 and the word at table+36.
    prepare(0, -1, 0, -1, 0, 9);
    levelFlags = 0x10000000u | 0x24800000u;
    nmPlayers = 0;
    coreForceSmallCharacters = 1;
    put_word(table, 36, norrath_host_handle(&lump_tokens[9]));
    __6PlayerG6Point3sii(object, Point3{1.0f, 2.0f, 3.0f}, 7, 0, 1);
    expect(selectLumpName == norrath_w9_startup_crt_b_view_464ab8, "forced small characters pick the pair name");
    expect(word(296) == norrath_host_handle(&lump_tokens[9]), "word at table+36 stored when flag 0x10000000 is set");
    expect(calls.find("lump(holder") == std::string::npos, "no animation lookup with flag 0x10000000");
    std::printf("%s\n", failures ? "FAIL" : "ok");
    return failures != 0;
}
