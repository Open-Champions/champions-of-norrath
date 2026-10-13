// Host contract for setState__14WoodElfSoldieri; not an EE input. Pointer words are offsets from a host base,
// every callee is a recording stub and the virtual slot is routed through w11_actor_c_host_slots. The expectations
// follow the table in the comment of the source, written out per state and per variant V (the word at +964).
#define W11_ACTOR_C_HOST 1
#include "set_state.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
static std::string calls;
static int procResult, edgeResult, commanderResult;
static unsigned rollValue;
static void* slot0(void* adjusted) { calls += "slot(" + std::to_string(static_cast<unsigned char*>(adjusted) - w11_actor_c_host_base) + ");"; return nullptr; }
W11ActorCHostSlot w11_actor_c_host_slots[] = {slot0};
static std::string hdr(void* h) { return std::to_string(static_cast<unsigned char*>(h) - w11_actor_c_host_base); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* state) { calls += "proc(" + std::to_string(*state) + ");"; return procResult; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* a, void* h, float rate, int flags) { calls += "T(" + hdr(h) + "," + std::to_string(rate) + "," + std::to_string(flags) + ");"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* a, void* h, float x, float y, int flags) { calls += "S(" + hdr(h) + "," + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(flags) + ");"; }
void animGlobalSync__FP14AnimationState(void* a) { calls += "sync(" + std::to_string(static_cast<unsigned char*>(a) - w11_actor_c_host_base) + ");"; }
int eRand__10GameObject(void*) { calls += "rand;"; return static_cast<int>(rollValue); }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(float* r, void*, int range) { calls += "smart(" + std::to_string(range) + ");"; r[0] = 11.0f; r[1] = 22.0f; r[2] = 33.0f; }
int bNextToEdge__14WoodElfSoldier(void*) { calls += "edge;"; return edgeResult; }
int commanderValid__14WoodElfSoldier(void*) { calls += "valid;"; return commanderResult; }
void leaveCommander__14WoodElfSoldier(void*) { calls += "leave;"; }
void dropItem__14WoodElfSoldieri(void*, int kind) { calls += "item(" + std::to_string(kind) + ");"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const float* p, int h, float a, float b, float c, float d, float e, float f, char* name) {
    char buf[200]; std::snprintf(buf, sizeof buf, "treasure(%g,%g,%g,%d,%g,%g,%.2f,%.2f,%.2f,%g,%d);", p[0], p[1], p[2], h, a, b, c, d, e, f, name != nullptr); calls += buf; }
}
static int failures;
static unsigned char* self;
int main() {
    static unsigned char base[8192];
    w11_actor_c_host_base = base;
    self = base + 64;
    auto setup = [&](int current, unsigned half294, unsigned v, unsigned long long flags64, unsigned flags12) {
        std::memset(base, 0, sizeof base);
        const unsigned res = 1024; std::memcpy(self + 36, &res, 4);
        for (int o = 0; o < 400; o += 4) { const unsigned w = 3000 + o; std::memcpy(base + 1024 + o, &w, 4); }
        const short c = static_cast<short>(current); std::memcpy(self + 572, &c, 2);
        std::memcpy(self + 294, &half294, 2); std::memcpy(self + 964, &v, 4); std::memcpy(self + 160, &flags64, 8); std::memcpy(self + 12, &flags12, 4);
        const unsigned u = 1000; std::memcpy(self + 808, &u, 4);
        const unsigned table = 2000; std::memcpy(self + 72, &table, 4);
        const short adj = 4; std::memcpy(base + 2000 + 192, &adj, 2);
        const unsigned fn = 0; std::memcpy(base + 2000 + 196, &fn, 4);
        const float pos[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(self + 16, pos, 12);
        const short heading = 777; std::memcpy(self + 56, &heading, 2);
        calls.clear();
    };
    auto stateAfter = [&]() { unsigned short s; std::memcpy(&s, self + 572, 2); return static_cast<int>(s); };
    auto w256 = [&]() { unsigned v; std::memcpy(&v, self + 256, 4); return v; };
    auto y176 = [&]() { float v; std::memcpy(&v, self + 176, 4); return v; };
    auto flags160 = [&]() { unsigned long long v; std::memcpy(&v, self + 160, 8); return v; };
    const char* slow = "0.300000"; const char* fast = "0.100000";
    auto T = [&](int off, const char* r, int f) { return "T(" + std::to_string(3000 + off) + "," + r + "," + std::to_string(f) + ");"; };
    auto S = [&](int off, const char* r, int f) { return "S(" + std::to_string(3000 + off) + "," + r + "," + r + "," + std::to_string(f) + ");"; };
    const std::string A = "sync(" + std::to_string(64 + 288) + ");";
    auto check = [&](const char* label, int state, unsigned v, const std::string& want, int wantState) {
        const std::string full = "proc(" + std::to_string(state) + ");" + want;
        if (calls != full || stateAfter() != wantState) { std::fprintf(stderr, "%s V=%u:\n got  %s (state %d)\n want %s (state %d)\n", label, v, calls.c_str(), stateAfter(), full.c_str(), wantState); ++failures; }
    };
    rollValue = 1234567u; procResult = 0; edgeResult = 0; commanderResult = 0;
    const unsigned r120 = rollValue % 120u, r60 = rollValue % 60u, r10 = rollValue % 10u;
    const int stand[4] = {148, 172, 244, 252}, walk[4] = {164, 212, 164, 268}, run5[4] = {148, 172, 244, 260};
    for (unsigned v = 0; v < 6; ++v) {
        auto pick = [&](const int* t) { return v < 4 ? T(t[v], slow, 3) : std::string(); };
        // 0 and 9
        setup(7, 0, v, 0x200ull | 0xf0ull, 0);
        edgeResult = 0; setState__14WoodElfSoldieri(self, 0);
        check("0", 0, v, "rand;" + std::string(v == 1 ? "edge;" : "") + pick(stand), 0);
        if (w256() != 1000 + r120 + 120 || flags160() != 0xf0ull) { std::fprintf(stderr, "0 fields V=%u\n", v); ++failures; }
        if (v == 1) { setup(7, 0, v, 0, 0); edgeResult = 1; setState__14WoodElfSoldieri(self, 0); check("0 edge", 0, v, "rand;edge;rand;" + pick(stand), 0); if (w256() != 1000 + r60) { std::fprintf(stderr, "0 edge field\n"); ++failures; } edgeResult = 0; }
        setup(7, 0, v, 0, 0); setState__14WoodElfSoldieri(self, 9); check("9", 9, v, pick(stand), 9);
        // 1
        setup(7, 0, v, 0, 0); setState__14WoodElfSoldieri(self, 1);
        { std::string w = "reset;smart(300);"; if (v == 0) w += T(164, slow, 3); if (v == 1) w += T(212, slow, 3); if (v == 2) w += T(164, slow, 3); if (v == 3) w += T(268, slow, 3); w += A + "rand;"; check("1", 1, v, w, 1);
          if (w256() != 1000 + r120 + 300 || y176() != (v == 3 ? 150.0f : 100.0f)) { std::fprintf(stderr, "1 fields V=%u\n", v); ++failures; } }
        // 2
        setup(7, 0, v, 0, 0); setState__14WoodElfSoldieri(self, 2);
        { std::string w = "reset;"; if (v == 0) w += T(164, slow, 3); if (v == 1) w += T(212, slow, 3); if (v == 2) w += T(164, slow, 3); if (v == 3) w += T(300, slow, 3); check("2", 2, v, w + A, 2); if (y176() != 100.0f) ++failures; }
        // 7
        setup(7, 0, v, 0, 0); setState__14WoodElfSoldieri(self, 7);
        { std::string w = "reset;"; if (v == 0) w += T(156, slow, 3); if (v == 1) w += T(188, slow, 3); if (v == 2) w += T(156, slow, 3); if (v == 3) w += T(268, slow, 3); check("7", 7, v, w + A, 7); if (y176() != 150.0f || w256() != 1120) { std::fprintf(stderr, "7 fields\n"); ++failures; } }
        // 3
        setup(7, 0, v, 0, 0); setState__14WoodElfSoldieri(self, 3);
        { std::string w = "reset;"; if (v == 0) w += T(156, slow, 3); if (v == 1) w += T(212, slow, 3); if (v == 2) w += T(156, slow, 3); if (v == 3) w += T(268, slow, 3); check("3", 3, v, w + A, 3); if (v < 4 && y176() != (v == 1 ? 100.0f : 150.0f)) { std::fprintf(stderr, "3 field V=%u\n", v); ++failures; } }
        // 5
        setup(7, 0, v, 0, 0); setState__14WoodElfSoldieri(self, 5);
        { std::string w; if (v < 4) w = T(run5[v], fast, 3); check("5", 5, v, w, 5); }
        // 13
        setup(7, 0, v, 0, 0); setState__14WoodElfSoldieri(self, 13);
        check("13", 13, v, T(236, slow, 3), 13);
        if (w256() != 1060 || flags160() != 0x200ull) { std::fprintf(stderr, "13 fields\n"); ++failures; }
        // 6
        setup(7, 0, v, 0, 0); setState__14WoodElfSoldieri(self, 6);
        { std::string w; if (v == 0) w = T(148, fast, 3) + S(140, fast, 8); if (v == 2) w = T(244, fast, 3) + S(220, fast, 8); if (v == 3) w = T(260, fast, 3) + S(284, fast, 8); check("6", 6, v, w + "rand;", 6); if (w256() != r10) { std::fprintf(stderr, "6 field\n"); ++failures; } }
        // 15
        setup(7, 0, v, 0, 0); setState__14WoodElfSoldieri(self, 15);
        { std::string w; if (v == 1) w = T(172, fast, 3) + S(196, fast, 8); if (v == 3) w = T(260, fast, 3) + S(196, fast, 8); check("15", 15, v, w + "rand;", 15); if (w256() != r10 + 10) { std::fprintf(stderr, "15 field\n"); ++failures; } }
        // 14
        setup(7, 0, v, 0, 0); setState__14WoodElfSoldieri(self, 14);
        { std::string w; if (v == 1) w = T(172, fast, 3) + S(204, fast, 8); check("14", 14, v, w + "rand;", 14); if (w256() != r60 + 60) { std::fprintf(stderr, "14 field\n"); ++failures; } }
        // 8
        for (int cur : {9, 6, 13, 7}) for (unsigned f : {0u, 8u}) {
            setup(cur, f, v, 0, 0); setState__14WoodElfSoldieri(self, 8);
            std::string w; int want;
            if (cur == 9) want = 9; else if (cur == 6 && (f & 8)) want = 6; else if (v == 2) { w = S(228, fast, 16); want = 8; } else want = cur;
            check("8", 8, v, w, want);
        }
        // 10
        for (int valid : {0, 1}) for (unsigned f12 : {0u, 2u, 3u}) {
            setup(7, 0, v, 0, f12); commanderResult = valid; setState__14WoodElfSoldieri(self, 10);
            std::string w = "valid;"; if (valid) w += "leave;"; if ((f12 & 2) == 0) w += "item(1);"; w += T(308, slow, 6) + "treasure(1,2,3,777,36,2,1.60,0.60,1.60,0.25,0);slot(68);";
            check("10", 10, v, w, 10);
        }
        commanderResult = 0;
        // store only
        for (int s : {4, 11, 12, 16, 17, 70000, -1}) { setup(7, 0, v, 0x200ull, 0); setState__14WoodElfSoldieri(self, s); check("store", s, v, "", s & 0xffff); if (flags160() != 0) { std::fprintf(stderr, "bit 0x200 not cleared\n"); ++failures; } }
    }
    setup(7, 0, 0, 0xf00ull, 0); procResult = 1; setState__14WoodElfSoldieri(self, 13);
    if (calls != "proc(13);" || stateAfter() != 7 || flags160() != 0xf00ull) { std::fprintf(stderr, "processing early return\n"); ++failures; }
    if (!failures) std::puts("WoodElfSoldier setState ok");
    return failures != 0;
}
