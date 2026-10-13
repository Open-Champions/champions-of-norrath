// Host contract for initType__3Orcb; not an EE input. Includes the source in host mode (32-bit words are
// offsets from w11_actor_b_host_base) and logs the callees' arguments.
#define W11_ACTOR_B_HOST 1
#include "init_type.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char* w11_actor_b_host_base = nullptr;
unsigned char skills[252];
float worldDifficulty;
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static std::string calls;
static int amountResult, rollResult;
static unsigned long long flagsAtCall;
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
unsigned w11_actor_b_host_call(unsigned function_word, void* adjusted, int count, unsigned long a1, unsigned long a2) {
    calls += "skill(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted) - arena) + "," + std::to_string(count) + "," +
             std::to_string(a1) + "," + std::to_string(static_cast<int>(a2)) + ");";
    return static_cast<unsigned>(amountResult);
}
extern "C" {
int eRand__10GameObject(void*) { calls += "rand;"; return rollResult; }
void setParameters__8Creaturefffffi(void* s, float a, float b, float c, float d, float e, int flag) {
    std::memcpy(&flagsAtCall, arena + 160, 8);
    calls += "set(" + f(a) + "," + f(b) + "," + f(c) + "," + f(d) + "," + f(e) + "," + std::to_string(flag) + ");";
    (void)s;
}
}
static unsigned long long dword(unsigned o) { unsigned long long v; std::memcpy(&v, arena + o, 8); return v; }
static unsigned word(unsigned o) { unsigned v; std::memcpy(&v, arena + o, 4); return v; }
static short half(unsigned o) { short v; std::memcpy(&v, arena + o, 2); return v; }
static float real(unsigned o) { float v; std::memcpy(&v, arena + o, 4); return v; }
static void set_word(unsigned o, unsigned v) { std::memcpy(arena + o, &v, 4); }
static const unsigned OWNER = 0x2000, TABLE = 0x3000;

static void reset(unsigned t, unsigned u) {
    std::memset(arena, 0, sizeof arena); std::memset(skills, 0, sizeof skills);
    w11_actor_b_host_base = arena;
    set_word(1116, t); set_word(208, u);
    const unsigned long long all = ~0ull;
    std::memcpy(arena + 160, &all, 8);
    worldDifficulty = 2.5f; amountResult = 100; rollResult = 0;
    for (unsigned i = 0; i < 63; ++i) { unsigned w = OWNER + 0x40 * i; std::memcpy(skills + 4 * i, &w, 4); set_word(w + 96, TABLE + 0x20 * i); set_word(TABLE + 0x20 * i + 96 + 4, 5000 + i); }
    set_word(TABLE + 0x20 * 14 + 96, 8);   // adjustment halfword for one owner
    calls.clear();
}
#define CHECK(cond, msg) do { if (!(cond)) { std::printf("%s\n", calls.c_str()); std::puts("fail: " msg); return 1; } } while (0)
static const unsigned long long CLEAR = 0xfffffff1ffffffffull;

int main() {
    // first-part variants by type
    struct Row { unsigned t; const char* call; unsigned long long bit; int byte; } rows[] = {
        {0, "set(1.1,0.8,1.1,0.8,0.8,0);", 1ull << 33, 0}, {4, "set(1.1,0.8,1.1,0.8,0.8,0);", 1ull << 33, 0},
        {1, "set(2,2,1.3,2,2,0);", 1ull << 35, 0}, {2, "set(1.8,1.2,1.3,1.2,1.2,0);", 1ull << 35, 0},
        {3, "set(2.6,1.2,1.3,1.2,1.4,0);", 1ull << 35, 0}, {5, "set(1.3,1,1.3,1,1,0);", 1ull << 34, 6}};
    for (auto& row : rows) {
        reset(row.t, 9);
        arena[156] = 77;
        initType__3Orcb(arena, 1);
        CHECK(calls == row.call, "setParameters call");
        CHECK(dword(160) == ((~0ull & CLEAR) | row.bit), "doubleword bits");
        CHECK(flagsAtCall == (~0ull & CLEAR), "bits are cleared before the call");
        CHECK(arena[156] == row.byte, "byte 156");
    }
    reset(3, 9); initType__3Orcb(arena, 1);
    CHECK(real(28) == 22.0f && real(224) > 1.29f && real(224) < 1.31f, "type 3 floats");
    // any other type: only the clear
    reset(6, 9); arena[156] = 77; initType__3Orcb(arena, 1);
    CHECK(calls.empty() && dword(160) == (~0ull & CLEAR) && arena[156] == 0, "other type");
    // flag is inverted
    reset(0, 9); initType__3Orcb(arena, 0);
    CHECK(calls == "set(1.1,0.8,1.1,0.8,0.8,1);", "flag 0 passes 1");
    reset(0, 9); initType__3Orcb(arena, 2);
    CHECK(calls == "set(1.1,0.8,1.1,0.8,0.8,3);", "flag 2 passes 3");
    // slot halfwords reset to -1 and left alone when the selector is unknown
    reset(6, 9);
    for (unsigned o : {1076u, 1084u, 1092u, 1100u}) { short junk = 1234; std::memcpy(arena + o, &junk, 2); }
    initType__3Orcb(arena, 1);
    for (unsigned o : {1076u, 1084u, 1092u, 1100u}) CHECK(half(o) == -1, "slot reset");
    CHECK(half(1108) == 0 && half(1068) == 0, "slots limited to four");
    // selector rows
    struct Sel { unsigned u, t; int skill; short type; int byte; bool slot; } sels[] = {
        {1, 0, 12, 24, 1, true}, {1, 4, 0, 0, 3, false}, {2, 0, 14, 22, 3, true}, {2, 4, 0, 0, 2, false},
        {3, 0, 13, 21, 2, true}, {3, 4, 0, 0, 2, false}, {4, 0, 26, 23, 4, true}, {4, 4, 26, 23, 4, true}, {0, 0, 0, 0, 0, false}, {5, 0, 0, 0, 0, false}};
    for (auto& s : sels) {
        reset(s.t, s.u); rollResult = 65536; amountResult = 100; worldDifficulty = 2.9f;
        initType__3Orcb(arena, 1);
        CHECK(arena[156] == s.byte, "byte after the selector");
        if (!s.slot) { CHECK(calls.find("skill") == std::string::npos && half(1076) == -1, "no slot"); continue; }
        const std::string want = "skill(" + std::to_string(5000 + s.skill) + "," + std::to_string(OWNER + 0x40 * s.skill + (s.skill == 14 ? 8 : 0)) + ",2,0,2);rand;";
        CHECK(calls.find(want) != std::string::npos, "slot call");
        // amount 100, factor 0.2 (one ulp low), roll 65536 * 2^-16 = 1.0: total = 100 + (int)(19.999998) = 119
        CHECK(half(1076) == s.type && half(1078) == 100 && half(1080) == 119 && half(1082) == 0 && half(1084) == -1, "slot contents");
        CHECK(arena[156] == s.byte, "byte after the slot");
    }
    // negative amount and roll truncate toward zero
    reset(0, 1); rollResult = -65536; amountResult = -50;
    initType__3Orcb(arena, 1);
    CHECK(half(1078) == -50 && half(1080) == -50 + static_cast<int>(-50 * 0.19999998807907104f * -1.0f), "negative amount");
    return 0;
}
