#define NORRATH_W8_HOST 1
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x20000];
extern "C" void* norrath_w8_word_to_pointer(unsigned word) { return arena + word; }
static void vfunction_a(void* object, void* target);
static const char* vfunction_b(void* object);
void (*norrath_w8_word_to_function(unsigned word))(void*) {
    return word == 1 ? reinterpret_cast<void (*)(void*)>(reinterpret_cast<void*>(&vfunction_a)) : reinterpret_cast<void (*)(void*)>(reinterpret_cast<void*>(&vfunction_b));
}

static std::vector<std::string> log_lines;
static void note(const char* format, ...) {
    char buffer[256];
    va_list list;
    va_start(list, format);
    std::vsnprintf(buffer, sizeof buffer, format, list);
    va_end(list);
    log_lines.push_back(buffer);
}
static bool has(const std::string& text) {
    for (const auto& line : log_lines) if (line == text) return true;
    return false;
}
static int count_prefix(const char* prefix) {
    int n = 0;
    for (const auto& line : log_lines) if (line.rfind(prefix, 0) == 0) ++n;
    return n;
}

struct Point3;
#include "creature_every_frame.cpp"

extern "C" {
int frameNm;
int nmPlayers;
int levelFlags;
int localNmPlayersAlive;
int localPlayersAlive[4];
void* world;
signed char netNetworkMode;
int bossMeterCurrent;
int bossMeterMax;
Point3 worldViewCenter;
unsigned char player[4 * 23984];
char bossMeterName[128];
char levelName[400];
}
static int rand_value, closest_result, local_object_is_self, boss_result, activation_result;
static float hit_fractions[40];
static int hit_index;
static char tag_text[16];
static bool tag_present;
static int vertex_returns[8];
static int vertex_index;

extern "C" {
int getClosestLivingPlayer__FG6Point3f(Point3* position, float range) { note("closest %g,%g,%g range=%g", position->x, position->y, position->z, range); return closest_result; }
void gameLineInitPathState__FP17gameLinePathStatePcP6Point3(void*, char* name, Point3* position) { note("path init %s %g", name, position->x); }
void* gameLineGet__FPc(char*) { return arena + 0x1F000; }
void initPosition__FP17gameLinePathState(void* state) { *reinterpret_cast<int*>(static_cast<unsigned char*>(state) + 32) = 4; }
float* gameLineGetVert__Fii(void*, int index) {
    note("vertex %d", index);
    const unsigned where = vertex_returns[vertex_index++ % 8];
    return where ? reinterpret_cast<float*>(arena + where) : nullptr;
}
void objectUpdateInGrid__FP10GameObject(void*) { note("grid"); }
void resetRouting__8Creature(void*) { note("routing"); }
void addEffect__FiG6Point3(int kind, Point3* position) { note("effect %d %g,%g,%g", kind, position->x, position->y, position->z); }
int eRand__10GameObject(void*) { return rand_value; }
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void*, void*, float a, float b, int c, int d, int e, int f, void* description, bool g) {
    if (description != nullptr) {
        unsigned char* bytes = static_cast<unsigned char*>(description);
        short kind, flag_a, flag_b, zero;
        std::memcpy(&kind, bytes + 36, 2); std::memcpy(&flag_a, bytes + 38, 2); std::memcpy(&flag_b, bytes + 40, 2); std::memcpy(&zero, bytes + 42, 2);
        note("attack %g,%g %d,%d,%d,0x%x desc kind=%d %d %d %d b32=%d,%d g=%d", a, b, c, d, e, f, kind, flag_a, flag_b, zero, bytes[32], bytes[33], g);
    } else {
        note("attack %g,%g %d,%d,%d,0x%x none g=%d", a, b, c, d, e, f, g);
    }
}
int iatan2__Fff(float y, float x) { note("atan2 %g %g", y, x); return 0x1000; }
float icos__Fi(int angle) { return 0.5f; }
float isin__Fi(int angle) { return 0.25f; }
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void*, Point3* start, Point3* direction, float length, int mode) {
    note("hitscan start=%g,%g,%g dir=%g,%g,%g length=%g mode=%d", start->x, start->y, start->z, direction->x, direction->y, direction->z, length, mode);
    return hit_fractions[hit_index++];
}
int netCheckActivation__FG6Point3f(Point3* position, float range) { note("activation %g range=%g", position->x, range); return activation_result; }
void* netGetLocalObject__FP8Creature(void* object) { return local_object_is_self ? object : nullptr; }
int isBoss__FP10GameObject(void*) { return boss_result; }
void fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(void*, void*, Point3* position, short yaw, void*, float a, float b, float c, float d) {
    note("fire %g,%g,%g yaw=%d %g %g %g %g", position->x, position->y, position->z, yaw, a, b, c, d);
}
void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(void*, void*, Point3* position, short yaw, void*, float a, float b, float c, unsigned color) {
    note("shock yaw=%d %g %g %g 0x%x", yaw, a, b, c, color);
}
int strcasecmp(const char* a, const char* b) {
    std::string x = a, y = b;
    for (auto& ch : x) ch = static_cast<char>(std::tolower(ch));
    for (auto& ch : y) ch = static_cast<char>(std::tolower(ch));
    return x.compare(y);
}
char* findTagString__10GameObjectPci(void*, const char* tag, int zero) { note("tag %s", tag); return tag_present ? tag_text : nullptr; }
}
static void vfunction_a(void* object, void* target) {
    const Point3* p = static_cast<const Point3*>(target);
    note("vfunc A at %d target=%g,%g,%g", static_cast<int>(static_cast<unsigned char*>(object) - arena), p->x, p->y, p->z);
}
static const char* vfunction_b(void* object) { note("vfunc B at %d", static_cast<int>(static_cast<unsigned char*>(object) - arena)); return "Boss"; }

static int failures;
static void expect(bool ok, const char* m) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", m);
        for (const auto& line : log_lines) std::fprintf(stderr, "    log: %s\n", line.c_str());
        ++failures;
    }
}
static const unsigned self_offset = 0x2000;
static void* const self_pointer = arena + self_offset;
static void setf(unsigned offset, float value) { std::memcpy(arena + self_offset + offset, &value, 4); }
static float getf(unsigned offset) { float v; std::memcpy(&v, arena + self_offset + offset, 4); return v; }
static void setw(unsigned offset, unsigned value) { std::memcpy(arena + self_offset + offset, &value, 4); }
static unsigned getw(unsigned offset) { unsigned v; std::memcpy(&v, arena + self_offset + offset, 4); return v; }
static void absw(unsigned address, unsigned value) { std::memcpy(arena + address, &value, 4); }
static void seth(unsigned offset, short value) { std::memcpy(arena + self_offset + offset, &value, 2); }
static short geth(unsigned offset) { short v; std::memcpy(&v, arena + self_offset + offset, 2); return v; }
static void setq(unsigned offset, unsigned long long value) { std::memcpy(arena + self_offset + offset, &value, 8); }
static unsigned long long getq(unsigned offset) { unsigned long long v; std::memcpy(&v, arena + self_offset + offset, 8); return v; }
static void setup() {
    std::memset(arena, 0, sizeof arena);
    log_lines.clear();
    frameNm = 1; nmPlayers = 0; levelFlags = 0; localNmPlayersAlive = 0; netNetworkMode = 0;
    bossMeterCurrent = bossMeterMax = 0; std::memset(bossMeterName, 0, sizeof bossMeterName);
    std::memset(player, 0, sizeof player);
    std::strcpy(levelName + 81, "other");
    rand_value = 0; closest_result = 0; local_object_is_self = 0; boss_result = 0; activation_result = 1;
    hit_index = 0; vertex_index = 0; tag_present = false;
    for (float& f : hit_fractions) f = 0.0f;
    for (int& v : vertex_returns) v = 0;
    setw(12, 0x40);                    // skip the vtable call by default
    setw(72, 0x6000);                  // vtable at 0x6000
    absw(0x6000 + 136, 0); absw(0x6000 + 140, 1);
    absw(0x6000 + 152, 0); absw(0x6000 + 156, 2);
    setf(32, 2.0f); setf(16, 10.0f); setf(20, 20.0f); setf(24, 30.0f);
    setf(60, 30.0f);                   // spring at rest
    setf(788, 0.0f);
    setw(0, 0);
    std::memcpy(arena + self_offset + 800, "\x3c", 1);   // byte 60: no +808 increment
}
static int run() { return everyFrame__8CreatureP12_modelHeader(self_pointer, arena + 0x1800); }

int main() {
    // Plain object with the rest state: nothing but the spring and the return value.
    setup();
    expect(run() == 0, "returns 0");
    expect(getf(64) == 0.0f && getf(60) == 30.0f, "spring at rest stays");
    expect(log_lines.empty(), "no calls");

    // Spring: v = f64 + 3 * (f24 - f60), damped by sqrt(0.09...) and added to f60 / 60.
    setup(); setf(24, 36.0f); setf(60, 30.0f); setf(64, 1.0f);
    run();
    const float spring = 1.0f + (36.0f - 30.0f) * 3.0f;
    const float damped = spring - __builtin_sqrtf(__builtin_bit_cast(float, 0x3db851ebu)) * spring;
    expect(getf(64) == damped && getf(60) == 30.0f + damped * __builtin_bit_cast(float, 0x3c888888u), "spring arithmetic");

    // Bit 0x40 clear: virtual entry at +136 with the raised point; the point's x gets 0 added.
    setup(); setw(12, 0); absw(0x6000 + 136, 8); setf(32, 4.0f);
    run();
    expect(has("vfunc A at 8200 target=10,20,32"), "vtable call: this + 8, target (x, y, z + f32 / 2 added to z=30)");

    // Decay of the two halfword groups.
    setup(); seth(140, 5); seth(142, -5); seth(144, 1); seth(146, -1); seth(148, 2); seth(150, -2); seth(152, 0); seth(154, 100);
    run();
    expect(geth(140) == 3 && geth(142) == -3 && geth(144) == 0 && geth(146) == 0 && geth(148) == 0 && geth(150) == 0 && geth(152) == 0 && geth(154) == 98,
           "halfwords move two toward zero and stop at zero");

    // Effects: frame mask 3, counters.
    setup(); setw(808, 1); setw(804, 10); frameNm = 8; seth(796, 3);
    run();
    expect(has("effect 16 10,20,32") && has("effect 18 10,20,32"), "effects 16 and 18 at (x, y, z + f32)");
    expect(geth(796) == 2, "796 counts down");
    setup(); setw(808, 1); setw(804, 10); frameNm = 9; seth(796, 3); seth(794, 1);
    run();
    expect(count_prefix("effect") == 0 && geth(794) == 0, "frame & 3 != 0: no effects; 794 counts down");

    // 808 increments unless the byte at 800 is 60.
    setup(); *(arena + self_offset + 800) = 59; setw(808, 7);
    run();
    expect(getw(808) == 8, "808 increments");

    // 876 glow.
    setup(); setf(876, 2.0f);
    run();
    expect(geth(140) == 128 && geth(144) == 128 && geth(148) == 128 && geth(152) == 128 && geth(146) == 32, "glow level min(1, f) * 128 and 32");
    expect(getf(876) == 2.0f - __builtin_bit_cast(float, 0x3c888888u), "glow decreases by 1/60");

    // Fire and shock for the local creature, then the decay of their timers.
    setup(); setf(856, 1.0f); setf(864, 0.0f); local_object_is_self = 1; setw(12, 0); setf(224, 1.5f); setf(228, 2.5f); *(arena + self_offset + 40) = 7;
    seth(56, 99);
    run();
    expect(has("fire 10,20,30 yaw=99 1 1.5 2.5 7"), "fire effect arguments");
    expect(getf(856) == 1.0f - __builtin_bit_cast(float, 0x3c888888u), "fire timer decreases by 1/60");
    setup(); setf(864, 0.01f); local_object_is_self = 1; setw(12, 0); setw(836, 1); setw(844, 2); setw(852, 3); setw(840, 9);
    seth(56, 5);
    run();
    expect(has("shock yaw=5 0.01 0 0 0x5e808080"), "shock effect arguments: timer, f224, f228, colour");
    expect(getw(836) == 0 && getw(844) == 0 && getw(852) == 0 && getw(840) == 9, "expired shock clears 836, 844, 852");
    setup(); setf(856, 1.0f); local_object_is_self = 0; frameNm = 32;
    run();
    expect(count_prefix("fire") == 0, "no fire effect unless local");
    expect(has("attack 0,0 100000,0,0,0x1180c040 desc kind=21 1 1 0 b32=255,255 g=0"), "every 32 frames the fire damage is resolved with the 21 description");

    // Burn and poison style timers with randomized damage.
    setup(); setf(868, 1.0f); frameNm = 64; rand_value = 7; seth(880, 2); seth(882, 5);
    run();
    expect(has("attack 0.2,0.3 100000,0,0,0x80c040 none g=0") || count_prefix("attack 0.2") == 1, "868: low * 0.1, (rand % range) * 0.1");
    setup(); setf(872, 1.0f); frameNm = 64; rand_value = 7; seth(884, 1); seth(886, 3);
    run();
    expect(count_prefix("attack 0.1,0.1 ") == 1, "872 damage: low 1, 7 % 3 = 1");

    // Slow: byte at 800 follows the timer; boss limit 15, normal 30; flag 8 clears it.
    setup(); setf(860, 100.0f); setw(12, 0x40); *(arena + self_offset + 800) = 0;
    run();
    expect(static_cast<signed char>(arena[self_offset + 800]) == 30, "slow with flag 0x40: capped at 30");
    setup(); setf(860, 100.0f); setw(12, 0); boss_result = 1;
    run();
    expect(static_cast<signed char>(arena[self_offset + 800]) == 15, "boss cap 15");
    setup(); setf(860, 100.0f); setw(12, 0); boss_result = 0;
    run();
    expect(static_cast<signed char>(arena[self_offset + 800]) == 30, "normal cap 30");
    setup(); setf(860, 100.0f); setw(12, 8);
    run();
    expect(static_cast<signed char>(arena[self_offset + 800]) == 0, "flag 8 clears the byte");

    // Stun timer at 924.
    setup(); setw(924, 5); frameNm = 100; setw(252, 90);
    run();
    expect(static_cast<signed char>(arena[self_offset + 800]) == 20, "elapsed 10: byte 20");
    setup(); setw(924, 200); frameNm = 100; setw(252, 0);
    run();
    expect(static_cast<signed char>(arena[self_offset + 800]) == 60, "elapsed above 30 and timer above 30: 60");
    setup(); setw(924, 7); frameNm = 100; setw(252, 0);
    run();
    expect(static_cast<signed char>(arena[self_offset + 800]) == 7, "elapsed above 30 and timer 7: the timer byte");
    setup(); setw(924, -3); frameNm = 100; setw(252, 0);
    run();
    expect(getw(924) == 0 && static_cast<signed char>(arena[self_offset + 800]) == 0, "negative timer resets");

    // Level flag minimum.
    setup(); levelFlags = 0x100; *(arena + self_offset + 800) = 59; seth(0, 0); arena[self_offset + 800] = 5;
    run();
    expect(static_cast<signed char>(arena[self_offset + 800]) == 20, "levelFlags 0x100 raises the byte to 20");

    // Aggro: a local player in range sets bit 31, calls the +152 entry and sets the meters; out of range clears them.
    setup(); setq(160, 1); setw(236, 1); setf(788, 100.0f); setf(560, 12.9f); setf(564, 34.2f);
    localNmPlayersAlive = 1; localPlayersAlive[0] = 2;
    std::memcpy(player + 2 * 23984 + 21872, "\x00\x30\x00\x00", 4);
    { float v = 11.0f; std::memcpy(arena + 0x3000 + 16, &v, 4); v = 20.0f; std::memcpy(arena + 0x3000 + 20, &v, 4); v = 30.0f; std::memcpy(arena + 0x3000 + 24, &v, 4); }
    run();
    expect((getq(160) & 0x80000000ull) != 0, "bit 31 set");
    expect(bossMeterCurrent == 12 && bossMeterMax == 34 && std::strcmp(bossMeterName, "Boss") == 0, "meters and name");
    expect(has("vfunc B at 8192"), "entry +152 called on this + adjustment");
    setup(); setq(160, 0x80000001ull); setw(236, 1); setf(788, 1.0f); bossMeterCurrent = 5; bossMeterMax = 6;
    localNmPlayersAlive = 1; localPlayersAlive[0] = 2;
    std::memcpy(player + 2 * 23984 + 21872, "\x00\x30\x00\x00", 4);
    { float v = 100.0f; std::memcpy(arena + 0x3000 + 16, &v, 4); }
    run();
    expect((getq(160) & 0x80000000ull) == 0 && bossMeterCurrent == 0 && bossMeterMax == 0, "out of range: bit 31 cleared and meters zeroed");

    // Pursuit counter 792: counts down, clears bits at zero; picks a player when the tracked one is invalid.
    setup(); setq(160, 0x400000); seth(792, 1); nmPlayers = 2; arena[self_offset + 85] = 0xff; closest_result = 3;
    run();
    expect((getq(160) & 0xc00000ull) == 0 && geth(792) == 0, "792 reaches zero and clears bits 22 and 23");
    setup(); setq(160, 0x400000); seth(792, 5); nmPlayers = 2; arena[self_offset + 85] = 0xff; closest_result = 3; setw(236, 1);
    run();
    expect(arena[self_offset + 85] == 3 && has("closest 10,20,30 range=500") && getf(788) == 500.0f, "tracking re-picked: closest player, +788 = 500");
    setup(); setq(160, 0x800000); seth(792, -1); nmPlayers = 2; arena[self_offset + 85] = 1; setw(236, 1);
    std::memcpy(player + 1 * 23984 + 21872, "\x00\x30\x00\x00", 4);
    { float v = 7.0f; std::memcpy(arena + 0x3000 + 16, &v, 4); v = 8.0f; std::memcpy(arena + 0x3000 + 20, &v, 4); v = 9.0f; std::memcpy(arena + 0x3000 + 24, &v, 4); }
    run();
    expect(geth(792) == -1 && getf(788) == 100.0f, "-1 never counts down; no bit 0x400000: 100");
    expect(getf(776) == 7.0f - 15.0f && getf(780) == 8.0f - 7.5f, "target point moved back by 30 * (cos, sin)");

    // Probing for a spot: the first accepted direction moves the object.
    setup(); setq(160, 0x800000); seth(792, 5); nmPlayers = 1; arena[self_offset + 85] = 0; setw(236, 0);
    std::memcpy(player + 0 * 23984 + 21872, "\x00\x30\x00\x00", 4);
    { float v = 7.0f; std::memcpy(arena + 0x3000 + 16, &v, 4); v = 8.0f; std::memcpy(arena + 0x3000 + 20, &v, 4); v = 9.0f; std::memcpy(arena + 0x3000 + 24, &v, 4); }
    worldViewCenter = {1000.0f, 1000.0f, 1000.0f};
    hit_fractions[0] = 0.0f; hit_fractions[1] = 1.0f; hit_fractions[2] = 0.5f;
    run();
    expect(count_prefix("hitscan") == 3, "three directions probed until one with a fraction in (0, 1) is accepted");
    expect(count_prefix("grid") == 1 && count_prefix("routing") == 1, "accepted: grid update and routing reset");
    expect(getf(16) == getf(776) + 800.0f * 0.5f * 0.5f && getf(20) == getf(780) + 800.0f * 0.25f * 0.5f && getf(24) == getf(784), "position set from the hit: hold + direction * 800 * fraction");
    expect(getf(272) == getf(776), "+272 follows +776");
    setup(); setq(160, 0x800000); seth(792, 5); nmPlayers = 0; setw(236, 0);
    std::strcpy(levelName + 81, "Kelethin");
    run();
    expect(count_prefix("hitscan") == 0, "kelethin never probes");
    // Path following: PathName tag, vertex table. Near the next vertex the one after it becomes the target.
    setup(); setq(160, 0x08000000); tag_present = true; std::strcpy(tag_text, "route"); setw(808, 40);
    { const float near_vertex[3] = {12.0f, 21.0f, 5.0f}; const float after_vertex[3] = {100.0f, 200.0f, 300.0f};
      std::memcpy(arena + 0x1A000, near_vertex, 12); arena[0x1A000 + 60] = 9;
      std::memcpy(arena + 0x1A040, after_vertex, 12); arena[0x1A040 + 60] = 4; }
    vertex_returns[0] = 0x1A000; vertex_returns[1] = 0x1A040;
    run();
    expect(has("tag PathName") && has("path init route 10") && has("vertex 5") && has("vertex 6"), "next vertex is index + 1, then index + 2");
    expect(getf(788) == 50.0f && getf(776) == 100.0f && getf(780) == 200.0f && getf(784) == 300.0f && getf(272) == 100.0f, "near the vertex: +776 and +272 take the vertex after");
    expect(getw(80) == 640 && arena[self_offset + 84] == 4, "+80 = +808 + 600, byte 84 = vertex byte");
    // Far from the next vertex: the point becomes that vertex; a matching byte with +80 < +808 moves the object.
    setup(); setq(160, 0x08000000); tag_present = true; setw(808, 700); setw(80, 650); arena[self_offset + 84] = 9;
    { const float far_vertex[3] = {500.0f, 600.0f, 7.0f}; std::memcpy(arena + 0x1A000, far_vertex, 12); arena[0x1A000 + 60] = 9; }
    vertex_returns[0] = 0x1A000;
    run();
    expect(getf(776) == 500.0f && getf(16) == 500.0f && getf(20) == 600.0f && count_prefix("grid") == 1, "far from the vertex: the object moves to it");
    setup(); setq(160, 0x08000000); tag_present = true; setw(808, 700); setw(80, 650); arena[self_offset + 84] = 3;
    { const float far_vertex[3] = {500.0f, 600.0f, 7.0f}; std::memcpy(arena + 0x1A000, far_vertex, 12); arena[0x1A000 + 60] = 9; }
    vertex_returns[0] = 0x1A000;
    run();
    expect(getw(80) == 1300 && arena[self_offset + 84] == 9 && count_prefix("grid") == 0 && getf(16) == 10.0f, "different vertex byte: only the timer and byte update");
    return failures;
}
