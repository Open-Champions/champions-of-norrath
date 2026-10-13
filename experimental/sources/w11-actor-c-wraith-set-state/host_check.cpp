// Host contract for setState__6Wraithi; not an EE input. Pointer words are offsets from a host base, every callee
// is a recording stub, and the virtual slot is routed through w11_actor_c_host_slots.
#define W11_ACTOR_C_HOST 1
#include "set_state.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
static std::string calls;
static int procResult;
static unsigned rollValue;
static void* slot0(void* adjusted) { calls += "slot(" + std::to_string(static_cast<unsigned char*>(adjusted) - w11_actor_c_host_base) + ");"; return nullptr; }
W11ActorCHostSlot w11_actor_c_host_slots[] = {slot0};
static std::string hdr(void* h) { return std::to_string(static_cast<unsigned char*>(h) - w11_actor_c_host_base); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* state) { calls += "proc(" + std::to_string(*state) + ");"; return procResult; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* a, void* h, float rate, int flags) { calls += "trans(" + std::to_string(static_cast<unsigned char*>(a) - w11_actor_c_host_base) + "," + hdr(h) + "," + std::to_string(rate) + "," + std::to_string(flags) + ");"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* a, void* h, float x, float y, int flags) { calls += "shot(" + std::to_string(static_cast<unsigned char*>(a) - w11_actor_c_host_base) + "," + hdr(h) + "," + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(flags) + ");"; }
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
int eRand__10GameObject(void*) { calls += "rand;"; return static_cast<int>(rollValue); }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(float* r, void*, int range) { calls += "smart(" + std::to_string(range) + ");"; r[0] = 11.0f; r[1] = 22.0f; r[2] = 33.0f; }
void listDestroy__FP8ListHead(void* head) { calls += "list(" + std::to_string(static_cast<unsigned char*>(head) - w11_actor_c_host_base) + ");"; }
void dropTreasure__4itemG6Point3iffffffPci(const float* p, int h, float a, float b, float c, float d, float e, float f, char* name, int value) {
    char buf[200]; std::snprintf(buf, sizeof buf, "treasure(%g,%g,%g,%d,%g,%g,%.2f,%g,%g,%g,%d,%d);", p[0], p[1], p[2], h, a, b, c, d, e, f, name != nullptr, value); calls += buf;
}
}
int main() {
    static unsigned char base[8192];
    w11_actor_c_host_base = base;
    int failures = 0;
    unsigned char* self = base + 64;
    const int self_off = 64;
    auto setup = [&](int current, unsigned half294) {
        std::memset(base, 0, sizeof base);
        const unsigned res = 1024; std::memcpy(self + 36, &res, 4);
        for (int o = 0; o < 100; o += 4) { const unsigned w = 3000 + o; std::memcpy(base + 1024 + o, &w, 4); }
        const short c = static_cast<short>(current); std::memcpy(self + 572, &c, 2);
        std::memcpy(self + 294, &half294, 2);
        const unsigned u = 1000; std::memcpy(self + 808, &u, 4);
        const unsigned table = 2000; std::memcpy(self + 72, &table, 4);
        const short adj = 4; std::memcpy(base + 2000 + 192, &adj, 2);
        const unsigned fn = 0; std::memcpy(base + 2000 + 196, &fn, 4);
        const float pos[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(self + 16, pos, 12);
        const short heading = 777; std::memcpy(self + 56, &heading, 2);
        const unsigned flags = 0x100; std::memcpy(self + 12, &flags, 4);
        const unsigned listWord = 5; std::memcpy(self + 576, &listWord, 4);
        calls.clear();
    };
    auto stateAfter = [&]() { unsigned short s; std::memcpy(&s, self + 572, 2); return static_cast<int>(s); };
    const std::string anim = std::to_string(self_off + 288);
    auto T = [&](int offset, const char* rate, int flags) { return "trans(" + anim + "," + std::to_string(3000 + offset) + "," + rate + "," + std::to_string(flags) + ");"; };
    auto S = [&](int offset, const char* rate, int flags) { return "shot(" + anim + "," + std::to_string(3000 + offset) + "," + rate + "," + rate + "," + std::to_string(flags) + ");"; };
    const char* slow = "0.300000"; const char* fast = "0.100000";
    auto expectCalls = [&](const char* label, int state, const std::string& want, int wantState) {
        const std::string full = "proc(" + std::to_string(state) + ");" + want;
        if (calls != full || stateAfter() != wantState) { std::fprintf(stderr, "%s: state %d\n got  %s (state %d)\n want %s (state %d)\n", label, state, calls.c_str(), stateAfter(), full.c_str(), wantState); ++failures; }
    };
    rollValue = 1234567;
    const unsigned r120 = rollValue % 120u, r60 = rollValue % 60u;
    setup(7, 0); procResult = 0;
    setState__6Wraithi(self, 0);
    expectCalls("0", 0, "list(" + std::to_string(self_off + 576) + ");rand;" + T(76, slow, 3), 0);
    { unsigned w; std::memcpy(&w, self + 576, 4); unsigned w256; std::memcpy(&w256, self + 256, 4); if (w != 0 || w256 != 1000 + r120) { std::fprintf(stderr, "state 0 fields\n"); ++failures; } }
    setup(7, 0); setState__6Wraithi(self, 1);
    expectCalls("1", 1, "reset;smart(300);" + T(60, slow, 3) + "sync;", 1);
    { float v[3]; std::memcpy(v, self + 272, 12); float g; std::memcpy(&g, self + 176, 4); if (v[0] != 11 || v[1] != 22 || v[2] != 33 || g != 140.0f) { std::fprintf(stderr, "state 1 fields\n"); ++failures; } }
    for (int cur : {3, 6, 13, 7}) { setup(cur, 0); setState__6Wraithi(self, 3); expectCalls("3", 3, (cur == 3 || cur == 6 || cur == 13) ? "reset;" : "reset;" + T(60, slow, 3) + "sync;", 3); }
    setup(7, 0); setState__6Wraithi(self, 5); expectCalls("5", 5, T(84, fast, 3), 5);
    setup(7, 0); setState__6Wraithi(self, 6); expectCalls("6", 6, S(28, fast, 8), 6);
    setup(7, 0); setState__6Wraithi(self, 15); expectCalls("15", 15, S(44, fast, 8), 15);
    setup(7, 0); setState__6Wraithi(self, 13); expectCalls("13", 13, S(44, fast, 8) + "rand;", 13);
    { unsigned w; std::memcpy(&w, self + 964, 4); if (w != 1000 + r120 + 300) { std::fprintf(stderr, "state 13 field\n"); ++failures; } }
    setup(7, 0); setState__6Wraithi(self, 14); expectCalls("14", 14, T(84, fast, 3) + "rand;", 14);
    { unsigned w; std::memcpy(&w, self + 256, 4); if (w != 1000 + r60 + 120) { std::fprintf(stderr, "state 14 field\n"); ++failures; } }
    setup(7, 0); setState__6Wraithi(self, 9); expectCalls("9", 9, T(76, slow, 3), 9);
    for (int cur : {9, 6, 13, 7}) for (unsigned f : {0u, 8u, 0xf7u}) {
        setup(cur, f); setState__6Wraithi(self, 8);
        const bool keep = cur == 9 || ((cur == 6 || cur == 13) && (f & 8)); 
        expectCalls("8", 8, keep ? "" : S(68, fast, 16), keep ? cur : 8);
    }
    setup(7, 0); setState__6Wraithi(self, 10);
    expectCalls("10", 10, "treasure(1,2,3,777,36,2,2.80,0,3,0.25,0,0);list(" + std::to_string(self_off + 576) + ");" + T(36, slow, 6) + "slot(68);", 10);
    { unsigned w; std::memcpy(&w, self + 12, 4); if (w != (0x100u | 0x4000u)) { std::fprintf(stderr, "state 10 flags\n"); ++failures; } }
    for (int s : {2, 4, 7, 11, 12, 16, 17, 100, 65535, 65536, 0x1000a, -1}) { setup(7, 0); setState__6Wraithi(self, s); expectCalls("store only", s, "", s & 0xffff); }
    setup(7, 0); procResult = 1; setState__6Wraithi(self, 5);
    if (calls != "proc(5);" || stateAfter() != 7) { std::fprintf(stderr, "processing early return: %s\n", calls.c_str()); ++failures; }
    if (!failures) std::puts("Wraith setState ok");
    return failures != 0;
}
