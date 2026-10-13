// Host contract for msg_alert__13FemaleDarkElfifG6Point3P10GameObject. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_alert.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
using namespace vslots_b;
static int atan_calls = 0, heading = 0, slot_calls = 0, slot_value = 0; static float atan_y, atan_x; static void* slot_self;
extern "C" int iatan2__Fff(float y, float x) { ++atan_calls; atan_y = y; atan_x = x; return heading; }
static void slot(void* self, int value) { ++slot_calls; slot_self = self; slot_value = value; }
static unsigned char object[700], table[400], source[64], point[12];
static void fresh() {
    std::memset(object, 0, sizeof object); std::memset(source, 0, sizeof source);
    atan_calls = slot_calls = 0;
    write_word(address(object) + 72u, address_bits(table) - 184u);
    write_word(address(object) + 1104u, 1u);
    write_u16(address(object) + 572u, 3u);
    write_single(address(object) + 16u, 10.0f); write_single(address(object) + 20u, 20.0f);
    write_single(address(source) + 16u, 13.5f); write_single(address(source) + 20u, 17.0f);
    write_u16(address(object) + 56u, 100u);
    heading = 100;
}
int main() {
    host_note(table - 184);
    write_u16(address(table), 0xfff8u);  // slot byte 184: adjustment -8
    write_word(address(table) + 4u, address_bits(reinterpret_cast<void*>(slot)));
    host_note(reinterpret_cast<void*>(slot));
    fresh();
    msg_alert__13FemaleDarkElfifG6Point3P10GameObject(object, 2, 0.0f, point, source);
    if (atan_calls != 1 || atan_y != -3.0f || atan_x != 3.5f) return 1;
    if (slot_calls != 1 || slot_self != object - 8 || slot_value != 14) return 2;
    // gate checks
    const struct { int kind; unsigned word; unsigned half; bool src; int head; bool fires; } rows[] = {
        {2, 1, 3, true, 100, true}, {2, 1, 5, true, 100, true}, {2, 1, 4, true, 100, false}, {2, 1, 6, true, 100, false},
        {1, 1, 3, true, 100, false}, {3, 1, 3, true, 100, false}, {2, 0, 3, true, 100, false}, {2, 1, 3, false, 100, false},
        {2, 1, 3, true, 100 + 9999, true}, {2, 1, 3, true, 100 + 10000, false}, {2, 1, 3, true, 100 - 9999, true},
        {2, 1, 3, true, 100 - 10000, false}, {2, 1, 3, true, 100 + 9998, true}, {2, 1, 3, true, -30000, false}};
    for (const auto& r : rows) {
        fresh();
        write_word(address(object) + 1104u, r.word); write_u16(address(object) + 572u, r.half); heading = r.head;
        msg_alert__13FemaleDarkElfifG6Point3P10GameObject(object, r.kind, 0.0f, point, r.src ? source : nullptr);
        if ((slot_calls == 1) != r.fires || slot_calls > 1) { std::printf("row kind %d head %d\n", r.kind, r.head); return 3; }
    }
    // the halfword at +56 is signed
    fresh(); write_u16(address(object) + 56u, 0xfff0u); heading = -16 + 9999;
    msg_alert__13FemaleDarkElfifG6Point3P10GameObject(object, 2, 0.0f, point, source);
    if (slot_calls != 1) return 4;
    fresh(); write_u16(address(object) + 56u, 0xfff0u); heading = -16 + 10000;
    msg_alert__13FemaleDarkElfifG6Point3P10GameObject(object, 2, 0.0f, point, source);
    if (slot_calls != 0) return 5;
    return 0;
}
