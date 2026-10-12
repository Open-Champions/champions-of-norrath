#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include <vector>
#include "msg_run.cpp"
using namespace vslots_b;
static int failures = 0;
// 1 damage, 2 frame, 3 check, 4 advance, 5 slide, 6 slot168, 7 farFromHome, 8 steer, 9 exit, 10 remove, 100+n setState(n)
static std::vector<int> events;
static int frame_result, check_result, far_result, steer_result, exit_result, check_distance;
static void *damage_model, *frame_model, *a168_self, *state_self;
static float advance_amount, slide_amount;
static unsigned char steer_point[12];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model) { (void)self; events.push_back(1); damage_model = model; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void* self, void* model) { (void)self; events.push_back(2); frame_model = model; return frame_result; }
extern "C" int objectActivationCheck__8Creaturei(void* self, int distance) { (void)self; events.push_back(3); check_distance = distance; return check_result; }
extern "C" void creatureAdvanceAnimation__8Creaturef(void* self, float amount) { (void)self; events.push_back(4); advance_amount = amount; }
extern "C" void slideBack__8Creaturef(void* self, float amount) { (void)self; events.push_back(5); slide_amount = amount; }
extern "C" int bFarFromHome__8Creature(void* self) { (void)self; events.push_back(7); return far_result; }
extern "C" int steerAndRouteToPosition__8CreatureG6Point3(void* self, const void* point) { (void)self; events.push_back(8); std::memcpy(steer_point, point, 12); return steer_result; }
extern "C" int bExitStatePersueIdle__8Creature(void* self) { (void)self; events.push_back(9); return exit_result; }
extern "C" void objectRemoveFromRunList__FP10GameObject(void* self) { (void)self; events.push_back(10); }
extern "C" void slot_168(void* self) { events.push_back(6); a168_self = self; }
extern "C" void set_state_stub(void* self, int value) { events.push_back(100 + value); state_self = self; }
static unsigned char object[900], group[32], model[4], table[200];
static void fresh(int state) {
    std::memset(object, 0, sizeof object);
    events.clear();
    frame_result = 0; check_result = 1; far_result = 0; steer_result = 0; exit_result = 0;
    write_word(address(object) + 36u, address_bits(group));
    write_word(address(object) + 72u, address_bits(table) - 168u);
    write_u16(address(object) + 572u, static_cast<unsigned>(state) & 0xffffu);
}
static void lt(unsigned offset_a, unsigned offset_b, bool less) {
    write_word(address(object) + offset_a, less ? 0xffffffffu : 5u);
    write_word(address(object) + offset_b, less ? 0u : 5u);
}
static bool tail_is(std::initializer_list<int> want) {
    return std::vector<int>(events.begin() + 6, events.end()) == std::vector<int>(want);
}
int main() {
    host_note(group); host_note(model); host_note(table - 168);
    write_word(address(group) + 12u, address_bits(model));
    write_u16(address(table), 4u);
    write_word(address(table) + 4u, address_bits(reinterpret_cast<void*>(slot_168)));
    write_u16(address(table) + 16u, 8u);
    write_word(address(table) + 20u, address_bits(reinterpret_cast<void*>(set_state_stub)));
    host_note(reinterpret_cast<void*>(slot_168));
    host_note(reinterpret_cast<void*>(set_state_stub));

    fresh(2);
    msg_run__14GnomeNavigator(object);
    expect(damage_model == model && frame_model == model && check_distance == 600 && advance_amount == 1.0f && slide_amount == 1.0f, "common calls");
    expect(a168_self == object + 4 && events.size() == 6, "slot 168 with the adjusted this, then nothing");
    fresh(2); frame_result = 1;
    msg_run__14GnomeNavigator(object);
    expect(events.size() == 2, "non-zero everyFrame returns");
    fresh(2); check_result = 0;
    msg_run__14GnomeNavigator(object);
    expect(events.size() == 3, "zero activation check returns");
    const int idle[] = {2, 4, 6, 7, 8, 11, 12, -1, 100, -32768};
    for (int s : idle) {
        fresh(s); lt(256, 808, true); lt(804, 808, true); far_result = 1; steer_result = 1; exit_result = 1;
        write_u64(address(object) + 160u, 0xc00000ull); write_u16(address(object) + 294u, 0x80u);
        msg_run__14GnomeNavigator(object);
        expect(events.size() == 6, "idle states do nothing");
    }

    // State 0.
    fresh(0); lt(256, 808, true);
    msg_run__14GnomeNavigator(object);
    expect(events.size() == 6, "state 0 without the 0xc00000 bits returns");
    fresh(0); lt(256, 808, true); write_u64(address(object) + 160u, 0x400000ull);
    msg_run__14GnomeNavigator(object);
    expect(tail_is({101}) && state_self == object + 8, "bit 0x400000 is enough; below: setState(1)");
    fresh(0); lt(256, 808, true); write_u64(address(object) + 160u, 0x800000ull);
    msg_run__14GnomeNavigator(object);
    expect(tail_is({101}), "bit 0x800000 is enough");
    fresh(0); lt(256, 808, true); write_u64(address(object) + 160u, 0xffffffffff3fffffull);
    msg_run__14GnomeNavigator(object);
    expect(events.size() == 6, "other bits do not count");
    fresh(0); lt(256, 808, false); write_u64(address(object) + 160u, 0xc00000ull); far_result = 0;
    msg_run__14GnomeNavigator(object);
    expect(tail_is({7}), "not below and not far: nothing");
    fresh(0); lt(256, 808, false); write_u64(address(object) + 160u, 0xc00000ull); far_result = 1;
    msg_run__14GnomeNavigator(object);
    expect(tail_is({7, 101}), "not below but far: setState(1)");
    fresh(0); lt(256, 808, true); write_u64(address(object) + 160u, 0xc00000ull); far_result = 1;
    msg_run__14GnomeNavigator(object);
    expect(tail_is({101}), "below skips bFarFromHome");

    // State 1.
    fresh(1);
    for (unsigned i = 0; i < 12u; ++i) object[272 + i] = static_cast<unsigned char>(0x30 + i);
    lt(256, 808, false); steer_result = 1;
    msg_run__14GnomeNavigator(object);
    expect(tail_is({8, 100}) && std::memcmp(steer_point, object + 272, 12) == 0, "state 1 steering non-zero: setState(0), point from +272");
    fresh(1); lt(256, 808, true); steer_result = 0;
    msg_run__14GnomeNavigator(object);
    expect(tail_is({8, 100}), "steering zero, +256 below +808: setState(0)");
    fresh(1); lt(256, 808, false); steer_result = 0;
    msg_run__14GnomeNavigator(object);
    expect(tail_is({8}), "steering zero, not below: nothing");
    fresh(1); lt(256, 808, true); steer_result = -3;
    msg_run__14GnomeNavigator(object);
    expect(tail_is({8, 100}), "negative steering result is non-zero");

    // State 3, 5, 9, 10.
    fresh(3); write_word(address(object) + 576u, 0u);
    msg_run__14GnomeNavigator(object);
    expect(tail_is({100}), "state 3, no target: setState(0)");
    fresh(3); write_word(address(object) + 576u, 4u);
    msg_run__14GnomeNavigator(object);
    expect(events.size() == 6, "state 3 with a target: nothing");
    fresh(5); exit_result = 0;
    msg_run__14GnomeNavigator(object);
    expect(tail_is({9}), "state 5 stays");
    fresh(5); exit_result = 1;
    msg_run__14GnomeNavigator(object);
    expect(tail_is({9, 103}), "state 5 leaves to 3");
    fresh(9); lt(804, 808, true);
    msg_run__14GnomeNavigator(object);
    expect(tail_is({103}), "state 9 below: setState(3)");
    fresh(9); lt(804, 808, false);
    msg_run__14GnomeNavigator(object);
    expect(events.size() == 6, "state 9 not below: nothing");
    fresh(9); write_word(address(object) + 804u, 0u); write_word(address(object) + 808u, 0xffffffffu);
    msg_run__14GnomeNavigator(object);
    expect(events.size() == 6, "state 9 compare is signed");
    fresh(10); write_u16(address(object) + 294u, 0x80u);
    msg_run__14GnomeNavigator(object);
    expect(tail_is({10}), "state 10 with bit 0x80 removes");
    fresh(10); write_u16(address(object) + 294u, 0xff7fu);
    msg_run__14GnomeNavigator(object);
    expect(events.size() == 6, "state 10 without bit 0x80");
    return failures == 0 ? 0 : 1;
}
