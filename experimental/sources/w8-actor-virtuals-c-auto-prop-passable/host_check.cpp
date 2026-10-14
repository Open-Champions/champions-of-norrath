#define NORRATH_W8C_HOST 1
#include "set_passable_state.cpp"
#include <cstdio>
#include <cstring>
unsigned char norrath_actors_b_view_4844a0[4] = {0x78, 0x56, 0x34, 0x12};
static int failures = 0;
static int collision_calls = 0, move_calls = 0;
static int seen_index[2], seen_flag;
static float seen_point[3];
static short seen_extra = -1;
static unsigned seen_world[2];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" int worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int flag) {
    seen_world[0] = static_cast<unsigned>(reinterpret_cast<w8c::Address>(world));
    ++collision_calls; seen_index[0] = index; seen_flag = flag; return 0;
}
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, const void* point, short extra) {
    seen_world[1] = static_cast<unsigned>(reinterpret_cast<w8c::Address>(world));
    ++move_calls; seen_index[1] = index; std::memcpy(seen_point, point, 12); seen_extra = extra;
    expect(collision_calls == move_calls, "collision call precedes the move");
    return 0;
}
static void run(int passable, unsigned gate, unsigned flags, bool calls, unsigned expected_flags) {
    unsigned char object[600];
    std::memset(object, 0x33, sizeof object);
    w8c::write_word(w8c::address(object) + 284u, gate);
    w8c::write_word(w8c::address(object) + 396u, 0xfffffffeu);
    w8c::write_word(w8c::address(object) + 12u, flags);
    collision_calls = move_calls = 0;
    setPassableState__8AutoPropb(object, passable);
    expect(collision_calls == (calls ? 1 : 0) && move_calls == (calls ? 1 : 0), "door calls follow the gate bit");
    expect(w8c::read_word(w8c::address(object) + 12u) == expected_flags, "flag word");
    if (calls) {
        expect(seen_index[0] == -2 && seen_index[1] == -2, "door index is the signed word at 396");
        expect(seen_extra == 0, "extra is zero");
        if (passable) {
            expect(seen_flag == 0 && seen_point[0] == 0.0f && seen_point[1] == 0.0f && seen_point[2] == -256.0f, "passable arguments");
        } else {
            expect(seen_flag == 1 && seen_point[0] == 0.0f && seen_point[1] == 0.0f && seen_point[2] == 0.0f, "blocked arguments");
        }
    }
    for (unsigned i = 0; i < sizeof object; ++i)
        if (!(i >= 12 && i < 16) && !(i >= 284 && i < 288) && !(i >= 396 && i < 400) && object[i] != 0x33) {
            std::fprintf(stderr, "fail: stray write at %u\n", i); ++failures; }
}
int main() {
    run(1, 1u, 0x10u, true, 0x11u);
    run(1, 0u, 0x10u, false, 0x11u);
    run(1, 2u, 0x10u, false, 0x11u);
    run(0, 3u, 0x11u, true, 0x10u);
    run(0, 0u, 0xffffffffu, false, 0xfffffffeu);
    run(5, 1u, 0u, true, 1u);
    return failures ? 1 : 0;
}
