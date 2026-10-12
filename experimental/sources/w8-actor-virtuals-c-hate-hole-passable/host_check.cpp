#define NORRATH_W8C_HOST 1
#include "set_passable_state.cpp"
#include <cstdio>
#include <cstring>
unsigned char norrath_actors_b_view_4844a0[4] = {0x78, 0x56, 0x34, 0x12};
static int failures = 0;
static int moves = 0, heights = 0;
static float seen_height = 0.0f, seen_point[3];
static int seen_index = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void*, int index, const void* point, short extra) {
    ++moves; seen_index = index; std::memcpy(seen_point, point, 12); expect(extra == 0, "extra zero"); return 0;
}
extern "C" int worldSetDoorHeight__FP12_worldHeaderif(void*, int index, float height) {
    ++heights; seen_index = index; seen_height = height; return 0;
}
static void run(unsigned state, int expect_moves, int expect_heights, float height) {
    unsigned char object[600];
    std::memset(object, 0x21, sizeof object);
    w8c::write_word(w8c::address(object) + 376u, state);
    w8c::write_word(w8c::address(object) + 396u, 0xfffffffdu);
    w8c::write_single(w8c::address(object) + 408u, 1000.0f);
    moves = heights = 0;
    setPassableState__8HateHoleb(object, 1);
    expect(moves == expect_moves && heights == expect_heights, "call counts");
    if (heights) { expect(seen_height == height && seen_index == -3, "height and signed index"); }
    if (moves) expect(seen_point[0] == 0.0f && seen_point[1] == 0.0f && seen_point[2] == -350.0f, "move point");
    expect(object[12] == 0x21 && object[600 - 1] == 0x21, "no stray write");
}
int main() {
    run(1u, 1, 1, 650.0f);
    run(2u, 0, 1, 1256.0f);
    run(0u, 0, 0, 0.0f);
    run(3u, 0, 0, 0.0f);
    run(0xffffffffu, 0, 0, 0.0f);
    return failures ? 1 : 0;
}
