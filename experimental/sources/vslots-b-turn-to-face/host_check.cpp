#define NORRATH_VSLOTS_B_HOST 1
#include "turn_to_face.cpp"
#include <cstring>
static int calls = 0;
static float seen_first = 0.0f;
static float seen_second = 0.0f;
static int atan_result = 0;
extern "C" int iatan2__Fff(float first, float second) {
    ++calls;
    seen_first = first;
    seen_second = second;
    return atan_result;
}
static unsigned half_at(unsigned char* object, unsigned offset) {
    return vslots_b::read_u16(vslots_b::address(object) + offset);
}
static int run(int flag, int atan, unsigned facing, unsigned old_turn, float rate, unsigned speed,
               unsigned expect_turn, unsigned expect_facing) {
    unsigned char object[808];
    unsigned char point[12];
    std::memset(object, 0x5a, sizeof object);
    std::memset(point, 0, sizeof point);
    vslots_b::write_single(vslots_b::address(object) + 16u, 10.0f);
    vslots_b::write_single(vslots_b::address(object) + 20u, 20.0f);
    vslots_b::write_single(vslots_b::address(point), 13.0f);
    vslots_b::write_single(vslots_b::address(point) + 4u, 24.0f);
    vslots_b::write_single(vslots_b::address(object) + 168u, rate);
    vslots_b::write_u16(vslots_b::address(object) + 56u, facing);
    vslots_b::write_u16(vslots_b::address(object) + 284u, old_turn);
    vslots_b::write_u8(vslots_b::address(object) + 800u, speed);
    calls = 0;
    atan_result = atan;
    turnToFace__8CreatureiG6Point3(object, flag, point);
    if (calls != 1 || seen_first != 4.0f || seen_second != 3.0f) return 1;
    if (half_at(object, 284u) != expect_turn || half_at(object, 56u) != expect_facing) return 2;
    if (object[55] != 0x5a || object[58] != 0x5a || object[283] != 0x5a || object[286] != 0x5a) return 3;
    if (object[799] != 0x5a || object[801] != 0x5a) return 3;
    return 0;
}
int main() {
    if (run(0, 1000, 10u, 0u, 1.0f, 0u, 27u, 37u) != 0) return 1;
    if (run(1, 1000, 10u, 0u, 1.0f, 0u, 64658u, 64668u) != 0) return 2;
    if (run(0, 1000, 10u, 100u, 1.0f, 30u, 82u, 51u) != 0) return 3;
    if (run(0, 1000, 10u, 0u, 1.0f, 30u, 27u, 23u) != 0) return 4;
    if (run(0, -500, 20u, 4000u, 2.0f, 1u, 2185u, 2169u) != 0) return 5;
    if (run(0, 0, 0u, 0u, 0.0f, 61u, 0u, 0u) != 0) return 6;
    if (run(1, 1000, 10u, 0u, 1.0f, 30u, 64658u, 65107u) != 0) return 7;
    return 0;
}
