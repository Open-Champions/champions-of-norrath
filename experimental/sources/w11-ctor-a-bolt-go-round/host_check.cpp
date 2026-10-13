// Host contract for the BoltGoRound constructor; not an EE input. The GameObject stub fills the
// object with 0x21 and writes a position; the player table, the alive list and their object
// positions are set up so some players are inside the 500-unit range and some are not. Stubs log the
// calls with arguments; the lightning stub returns a numbered handle.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
extern "C" {
int nmPlayersAlive;
int playersAlive[4];
unsigned char player[23984 * 4];
}
#include "ctor.cpp"
#include <cstdio>
unsigned char vt_BoltGoRound[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static int lightning_calls;
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static std::string pt(Point3 p) { return std::to_string(p.x) + "/" + std::to_string(p.y) + "/" + std::to_string(p.z); }
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls() += "go(" + std::to_string(self == the_obj) + "," + pt(p) + "," + std::to_string(map == nullptr) + "," + std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 512);
    w11::f32(self, 16) = 400.0f; w11::f32(self, 20) = 128.0f; w11::f32(self, 24) = -128.0f;
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls() += "snd(" + std::to_string(w11::word(h)) + "," + std::to_string(o == the_obj) + "," + std::to_string(v) + ");";
    return 0x55aa;
}
extern "C" void objectAddToRunList__FP10GameObject(void* o) {
    calls() += "run" + std::to_string(o == the_obj) + "[" + std::to_string(w11::u32(the_obj, 12)) + "," + std::to_string(w11::u32(the_obj, 80)) + "," +
               std::to_string(w11::u32(the_obj, 84)) + "," + std::to_string(w11::i32(the_obj, 152)) + "," + std::to_string(w11::u16(the_obj, 76)) + "];";
}
extern "C" int P_StartLightning__FG6Point3T0iiffUi(Point3 a, Point3 b, int c, int d, float e, float f, unsigned g) {
    calls() += "bolt(" + pt(a) + "," + pt(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(f) + "," +
               std::to_string(g) + ");";
    return 0x7000 + ++lightning_calls;
}
int main() {
    the_obj = at(0x100);
    int bad = 0;
    // x positions of the four players; the object itself sits at x = 400 (y 128, z -104 after the raise)
    const float xs[4] = {410.0f, 5000.0f, 800.0f, 600.0f};
    struct Variant { int order[4]; int alive; bool header; };
    const Variant variants[] = {
        {{2, 0, 3, 1}, 0, true},   // nobody alive
        {{2, 0, 3, 1}, 1, true},   // one in range
        {{2, 0, 3, 1}, 4, true},   // three in range, one out
        {{1, 2, 0, 3}, 4, false},  // the out-of-range player first, so the bolt target slot index differs from the count
    };
    for (const Variant& v : variants) {
        std::memset(arena, 0, sizeof arena);
        std::memset(player, 0, sizeof player);
        calls().clear();
        lightning_calls = 0;
        frameNm = 70;
        for (int p = 0; p < 4; ++p) {
            unsigned char* obj = at(0x1000 + 0x400 * p);
            w11::f32(obj, 16) = xs[p]; w11::f32(obj, 20) = 128.0f; w11::f32(obj, 24) = -128.0f;
            w11::u32(player + 23984 * p, 21872) = w11::word(obj);
        }
        for (int i = 0; i < 4; ++i) playersAlive[i] = v.order[i];
        nmPlayersAlive = v.alive;
        void* const header = v.header ? at(0xa00) : nullptr;
        __11BoltGoRoundG6Point3siP15AnimationHeader(the_obj, Point3{1.0f, -2.0f, 3.5f}, 9, 25, header);

        std::string want = "go(1,1.000000/-2.000000/3.500000,1,1);";
        if (header) want += "snd(" + std::to_string(w11::word(header)) + ",1,1.000000);";
        want += "run1[" + std::to_string(0x21212121u | 0xb) + ",95,0," + std::to_string(header ? 0x55aa : -1) + ",8481];";
        // Model of the slots: slot[k] holds the position stored for the k-th accepted player.
        Point3 slot[4] = {};
        int count = 0, handle[4] = {};
        for (int i = 0; i < v.alive; ++i) {
            const float x = xs[v.order[i]];
            const float dx = x - 400.0f, dz = -128.0f - -104.0f;
            if (dx * dx + dz * dz < 250000.0f) {
                slot[count] = Point3{x, 128.0f, -128.0f};
                want += "bolt(" + pt(Point3{400.0f, 128.0f, -104.0f}) + "," + pt(slot[i]) + ",200,10,8.000000,16.500000," + std::to_string(0x5e808080u) + ");";
                handle[count] = 0x7000 + count + 1;
                ++count;
            }
        }
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        if (w11::u16(the_obj, 76) != count) { std::puts("count"); bad = 1; }
        for (int n = 0; n < count; ++n) {
            if (w11::i32(the_obj, 88 + 4 * n) != handle[n]) { std::puts("handles"); bad = 1; }
            if (fl(the_obj + 104 + 12 * n) != slot[n].x) { std::puts("slots"); bad = 1; }
        }
        if (fl(the_obj + 24) != -104.0f || w11::u32(the_obj, 72) != w11::word(vt_BoltGoRound) || w11::u32(the_obj, 84) != 0 || the_obj[300] != 0x21) { std::puts("fields"); bad = 1; }
    }
    return bad;
}
