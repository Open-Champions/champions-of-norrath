// Host contract for msg_run__8RollDoor; not an EE input. Collision, run-list and trig calls are
// logged; the trig stubs return dyadic values so the expected floats are exact.
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
extern "C" { void* world = reinterpret_cast<void*>(0x1234); }
extern "C" void worldSetDoorCollision__FP12_worldHeaderii(void* w, int a, int b) { calls += "door(" + std::to_string(w == world) + "," + std::to_string(a) + "," + std::to_string(b) + ");"; }
extern "C" void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
extern "C" float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
extern "C" float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return -0.25f; }
#define W10_ACTOR_L_HOST 1
unsigned char* w10_actor_l_host_base;
unsigned w10_actor_l_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
#include "msg_run.cpp"
alignas(16) static unsigned char o[128], ref[128];
static void putw(int at, unsigned v) { std::memcpy(o + at, &v, 4); }
static void putf(int at, float v) { std::memcpy(o + at, &v, 4); }
static void puth(int at, short v) { std::memcpy(o + at, &v, 2); }
static float getf(int at) { float v; std::memcpy(&v, o + at, 4); return v; }
static int geti(int at) { int v; std::memcpy(&v, o + at, 4); return v; }
static short geth(int at) { short v; std::memcpy(&v, o + at, 2); return v; }
int main() {
    int bad = 0;
    for (int c : {0, 120, 121, 150, 151, 152, -5}) {
        std::memset(o, 0x11, sizeof o); calls.clear();
        putw(84, static_cast<unsigned>(c)); puth(76, 7); puth(56, -300); putf(16, 10.0f); putf(20, 20.0f);
        putf(80, 2.0f); puth(88, 1000); putw(12, 0x40);
        std::memcpy(ref, o, sizeof o);
        msg_run__8RollDoor(o);
        std::string want;
        if (c >= 121) want += "door(1,7,0);";
        if (geti(84) != c + 1) bad = 1;
        if (c >= 151) {
            want += "remove;";
            std::memcpy(ref + 84, &(const int&)(c + 1), 4);
            const unsigned fl = 0x42; std::memcpy(ref + 12, &fl, 4);
        } else {
            want += "cos(" + std::to_string(-300 + 16384) + ");sin(" + std::to_string(-300 + 16384) + ");";
            const float x = 10.0f - 0.5f * 2.0f, y = 20.0f - (-0.25f) * 2.0f, sp = 2.0f + 0.01f;
            std::memcpy(ref + 16, &x, 4); std::memcpy(ref + 20, &y, 4); std::memcpy(ref + 80, &sp, 4);
            const short h = static_cast<short>(1000 - 400); std::memcpy(ref + 88, &h, 2);
            std::memcpy(ref + 84, &(const int&)(c + 1), 4);
        }
        if (calls != want) bad = 1;
        if (std::memcmp(o, ref, sizeof o) != 0) bad = 1;
    }
    return bad;
}
