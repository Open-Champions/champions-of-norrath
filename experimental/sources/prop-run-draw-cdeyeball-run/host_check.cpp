#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "msg_run.cpp"
using namespace vslots_b;
static int failures = 0, order_count = 0, order[8];
static int check_calls, check_distance, slot_a_calls, slot_b_calls, slot_b_value;
static void *check_self, *slot_a_self, *slot_b_self;
static unsigned char slot_b_point[12];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void objectActivationCheck__8Creaturei(void* self, int distance) {
    ++check_calls; order[order_count++] = 1; check_self = self; check_distance = distance;
}
extern "C" void slot_a(void* self) { ++slot_a_calls; order[order_count++] = 2; slot_a_self = self; }
extern "C" void slot_b(void* self, int value, const void* point) {
    ++slot_b_calls; order[order_count++] = 3; slot_b_self = self; slot_b_value = value;
    std::memcpy(slot_b_point, point, 12);
}
static unsigned char object[1000], table[400], target[16], inner[48];
static void fresh() {
    std::memset(object, 0, sizeof object);
    check_calls = slot_a_calls = slot_b_calls = order_count = 0;
    write_word(address(object) + 72u, address_bits(table) - 100u);
}
static unsigned facing() { return read_u16(address(object) + 56u); }
int main() {
    host_note(table - 100); host_note(target); host_note(inner);
    write_u16(address(table) + 68u, 0xfffcu);  // slot byte 168 is table - 100 + 168
    write_word(address(table) + 72u, address_bits(reinterpret_cast<void*>(slot_a)));
    write_u16(address(table) + 76u, 8u);       // slot byte 176
    write_word(address(table) + 80u, address_bits(reinterpret_cast<void*>(slot_b)));
    host_note(reinterpret_cast<void*>(slot_a));
    host_note(reinterpret_cast<void*>(slot_b));
    for (unsigned i = 0; i < 12u; ++i) inner[16 + i] = static_cast<unsigned char>(0x60 + i);
    write_word(address(target) + 12u, address_bits(inner));

    fresh();
    msg_run__9CdEyeball(object);
    expect(check_calls == 1 && check_self == object && check_distance == 600, "activation check with 600");
    expect(slot_a_calls == 1 && slot_a_self == object - 4, "slot 168 with the adjusted this");
    expect(slot_b_calls == 0, "no target, no second slot");
    expect(order_count == 2 && order[0] == 1 && order[1] == 2, "check then slot 168");

    fresh();
    write_word(address(object) + 576u, address_bits(target));
    msg_run__9CdEyeball(object);
    expect(slot_b_calls == 1 && slot_b_self == object + 8 && slot_b_value == 0, "slot 176 with adjustment and 0");
    expect(std::memcmp(slot_b_point, inner + 16, 12) == 0, "point comes from (target+12)+16");
    expect(order_count == 3 && order[2] == 3, "slot 176 last");

    // Facing limits: object+962 is the aim, object+56 the current angle.
    fresh();
    write_u16(address(object) + 962u, 10000u); write_u16(address(object) + 56u, 3999u);  // d = 6001
    msg_run__9CdEyeball(object);
    expect(facing() == 4000u, "d above 6000 stores aim - 6000");
    fresh();
    write_u16(address(object) + 962u, 10000u); write_u16(address(object) + 56u, 4000u);  // d = 6000
    msg_run__9CdEyeball(object);
    expect(facing() == 4000u, "d of 6000 is left alone");
    fresh();
    write_u16(address(object) + 962u, 10000u); write_u16(address(object) + 56u, 16001u);  // d = -6001
    msg_run__9CdEyeball(object);
    expect(facing() == 16000u, "d below -6000 stores aim + 6000");
    fresh();
    write_u16(address(object) + 962u, 10000u); write_u16(address(object) + 56u, 16000u);  // d = -6000
    msg_run__9CdEyeball(object);
    expect(facing() == 16000u, "d of -6000 is left alone");
    fresh();
    write_u16(address(object) + 962u, 100u); write_u16(address(object) + 56u, 65000u);  // d = 636 after 16-bit wrap
    msg_run__9CdEyeball(object);
    expect(facing() == 65000u, "difference wraps through 16 bits");
    fresh();
    write_u16(address(object) + 962u, 0x7000u); write_u16(address(object) + 56u, 0x9000u);  // d = -8192 as 16-bit
    msg_run__9CdEyeball(object);
    expect(facing() == 0x7000u + 6000u, "signed 16-bit difference below -6000");
    fresh();
    write_u16(address(object) + 962u, 100u); write_u16(address(object) + 56u, 0u);
    msg_run__9CdEyeball(object);
    expect(facing() == 0u, "small difference leaves the angle");
    fresh();
    write_u16(address(object) + 962u, 3000u); write_u16(address(object) + 56u, 0xe000u);  // d = 3000 - (-8192) = 11192
    msg_run__9CdEyeball(object);
    expect(facing() == 3000u - 6000u + 65536u, "aim - 6000 wraps in a halfword");
    return failures == 0 ? 0 : 1;
}
