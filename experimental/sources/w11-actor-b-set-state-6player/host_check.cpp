// Host contract for setState__6Playeri; not an EE input. Includes the source.
#define W11_ACTOR_B_HOST 1
#include "set_state.cpp"
#include <cstring>
#include <initializer_list>
#include <string>
static unsigned char arena[0x20000];
unsigned char* w11_actor_b_host_base = arena;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
unsigned char player[4 * 23984];
unsigned levelFlags;
static std::string calls;
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* a, void* h, float blend, int mode) {
    calls += "T(" + std::to_string(static_cast<unsigned char*>(a) - arena) + "," + std::to_string(static_cast<unsigned char*>(h) - arena) + "," +
           std::to_string(blend) + "," + std::to_string(mode) + ");";
}
extern "C" void animGlobalSync__FP14AnimationState(void* a) { calls += "S(" + std::to_string(static_cast<unsigned char*>(a) - arena) + ");"; }
extern "C" void resetRouting__8Creature(void* s) { calls += "R(" + std::to_string(static_cast<unsigned char*>(s) - arena) + ");"; }
static void setw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned getw(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
int main() {
    const unsigned self = 0x400, res = 0x2000;
    for (int rec = 0; rec < 4; ++rec)
    for (signed char kind : {0, 1, 2, -1}) {
        for (unsigned lf : {0u, 0x10000000u, 0xefffffffu, 0xffffffffu})
        for (int state : {-1, 0, 1, 2, 9})
        for (unsigned cur : {0u, 1u, 9u}) {
            std::memset(arena, 0x33, sizeof arena);
            std::memset(player, 0, sizeof player);
            player[rec * 23984 + 23582] = static_cast<unsigned char>(kind);
            levelFlags = lf;
            setw(self + 960, rec); setw(self + 36, res); setw(self + 1400, cur);
            int m = kind * 13;
            setw(res + 36, 0x7000); setw(res + m * 8 + 68, 0x7100); setw(res + m * 8 + 76, 0x7200);
            unsigned char before[sizeof arena]; std::memcpy(before, arena, sizeof arena);
            calls.clear();
            setState__6Playeri(arena + self, state);
            std::string want;
            if (state == 0) {
                want = "T(" + std::to_string(self + 288) + "," + std::to_string((lf & 0x10000000u) ? 0x7000 : 0x7100) + "," + std::to_string(0.3f) + ",3);";
            } else if (state == 1) {
                want = "R(" + std::to_string(self) + ");";
                if (cur != 1) want += "T(" + std::to_string(self + 288) + ",29184," + std::to_string(0.1f) + ",3);";
                want += "S(" + std::to_string(self + 288) + ");";
            }
            if (calls != want) return 1;
            unsigned wantCur = state == 0 ? 0 : state == 1 ? 1 : cur;
            if (getw(self + 1400) != wantCur) return 2;
            float f; std::memcpy(&f, arena + self + 176, 4);
            if (state == 1 ? f != 80.0f : std::memcmp(before + self + 176, arena + self + 176, 4) != 0) return 3;
            for (unsigned i = 0; i < sizeof arena; ++i) {
                bool ok = (i >= self + 1400 && i < self + 1404) || (state == 1 && i >= self + 176 && i < self + 180);
                if (!ok && before[i] != arena[i]) return 4;
            }
        }
    }
    return 0;
}
