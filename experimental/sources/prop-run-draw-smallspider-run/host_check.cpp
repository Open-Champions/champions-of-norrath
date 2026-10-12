#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "msg_run.cpp"
using namespace vslots_b;
static int failures = 0, order_count = 0, order[16];
static int damage_calls, frame_calls, check_calls, advance_calls, slide_calls, wander_calls, remove_calls, slot_calls, slot_value;
static void *damage_model, *frame_model, *slot_self;
static int frame_result, check_result, check_distance;
static float advance_amount, slide_amount;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model) { (void)self; ++damage_calls; order[order_count++] = 1; damage_model = model; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void* self, void* model) { (void)self; ++frame_calls; order[order_count++] = 2; frame_model = model; return frame_result; }
extern "C" int objectActivationCheck__8Creaturei(void* self, int distance) { (void)self; ++check_calls; order[order_count++] = 3; check_distance = distance; return check_result; }
extern "C" void creatureAdvanceAnimation__8Creaturef(void* self, float amount) { (void)self; ++advance_calls; order[order_count++] = 4; advance_amount = amount; }
extern "C" void slideBack__8Creaturef(void* self, float amount) { (void)self; ++slide_calls; order[order_count++] = 5; slide_amount = amount; }
extern "C" void smartWander__8Creature(void* self) { (void)self; ++wander_calls; order[order_count++] = 6; }
extern "C" void objectRemoveFromRunList__FP10GameObject(void* self) { (void)self; ++remove_calls; order[order_count++] = 7; }
extern "C" void set_state_stub(void* self, int value) { ++slot_calls; order[order_count++] = 8; slot_self = self; slot_value = value; }
static unsigned char object[900], group[32], model[4], table[200];
static void fresh(int state) {
    std::memset(object, 0, sizeof object);
    damage_calls = frame_calls = check_calls = advance_calls = slide_calls = wander_calls = remove_calls = slot_calls = order_count = 0;
    frame_result = 0; check_result = 1;
    write_word(address(object) + 36u, address_bits(group));
    write_word(address(object) + 72u, address_bits(table) - 184u);
    write_u16(address(object) + 572u, static_cast<unsigned>(state) & 0xffffu);
}
int main() {
    host_note(group); host_note(model); host_note(table - 184);
    write_word(address(group) + 12u, address_bits(model));
    write_u16(address(table), 16u);
    write_word(address(table) + 4u, address_bits(reinterpret_cast<void*>(set_state_stub)));
    host_note(reinterpret_cast<void*>(set_state_stub));

    fresh(99);
    msg_run__11SmallSpider(object);
    expect(damage_model == model && frame_model == model, "both handlers get the group +12 word");
    expect(check_distance == 600 && advance_amount == 1.0f && slide_amount == 1.0f, "600, 1.0, 1.0");
    expect(order_count == 5 && order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4 && order[4] == 5, "order, unknown state does nothing more");

    fresh(0);
    frame_result = 5;
    msg_run__11SmallSpider(object);
    expect(order_count == 2 && check_calls == 0, "non-zero everyFrame stops");
    fresh(0);
    check_result = 0;
    msg_run__11SmallSpider(object);
    expect(order_count == 3 && advance_calls == 0, "zero activation check stops");

    fresh(1);
    msg_run__11SmallSpider(object);
    expect(wander_calls == 1 && order[5] == 6 && order_count == 6 && slot_calls == 0, "state 1 wanders");

    fresh(0);
    write_word(address(object) + 256u, 3u); write_word(address(object) + 808u, 4u);
    msg_run__11SmallSpider(object);
    expect(slot_calls == 1 && slot_self == object + 16 && slot_value == 1, "state 0 with 256 < 808 calls slot 184 with 1");
    fresh(0);
    write_word(address(object) + 256u, 4u); write_word(address(object) + 808u, 4u);
    msg_run__11SmallSpider(object);
    expect(slot_calls == 0, "equal words do not call");
    fresh(0);
    write_word(address(object) + 256u, 0xffffffffu); write_word(address(object) + 808u, 0u);
    msg_run__11SmallSpider(object);
    expect(slot_calls == 1, "the compare is signed (-1 < 0)");
    fresh(0);
    write_word(address(object) + 256u, 0u); write_word(address(object) + 808u, 0xffffffffu);
    msg_run__11SmallSpider(object);
    expect(slot_calls == 0, "0 is not below -1");

    fresh(10);
    write_u16(address(object) + 294u, 0x7fu);
    msg_run__11SmallSpider(object);
    expect(remove_calls == 0, "state 10 without bit 0x80");
    fresh(10);
    write_u16(address(object) + 294u, 0x80u);
    msg_run__11SmallSpider(object);
    expect(remove_calls == 1 && order_count == 6, "state 10 with bit 0x80 leaves the run list");
    fresh(10);
    write_u16(address(object) + 294u, 0x100u);
    msg_run__11SmallSpider(object);
    expect(remove_calls == 0, "only bit 0x80 counts");
    fresh(-1);
    write_u16(address(object) + 294u, 0xffffu); write_word(address(object) + 808u, 9u);
    msg_run__11SmallSpider(object);
    expect(remove_calls == 0 && wander_calls == 0 && slot_calls == 0, "state -1 does nothing");
    fresh(2);
    write_word(address(object) + 808u, 9u); write_u16(address(object) + 294u, 0xffffu);
    msg_run__11SmallSpider(object);
    expect(remove_calls == 0 && wander_calls == 0 && slot_calls == 0, "state 2 does nothing");
    return failures == 0 ? 0 : 1;
}
