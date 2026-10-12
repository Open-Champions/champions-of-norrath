#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "msg_run.cpp"
using namespace vslots_b;
static int failures = 0, order_count = 0, order[16];
static int damage_calls, frame_calls, check_calls, advance_calls, slide_calls, move_calls, exit_calls, wander_calls, remove_calls, state_calls, state_value, a168_calls;
static void *damage_model, *frame_model, *state_self, *a168_self;
static int frame_result, check_distance, exit_result;
static float advance_amount, slide_amount, moved[3];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model) { (void)self; ++damage_calls; order[order_count++] = 1; damage_model = model; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void* self, void* model) { (void)self; ++frame_calls; order[order_count++] = 2; frame_model = model; return frame_result; }
extern "C" int objectActivationCheck__8Creaturei(void* self, int distance) { (void)self; ++check_calls; order[order_count++] = 3; check_distance = distance; return 0; }
extern "C" void creatureAdvanceAnimation__8Creaturef(void* self, float amount) { (void)self; ++advance_calls; order[order_count++] = 4; advance_amount = amount; }
extern "C" void slideBack__8Creaturef(void* self, float amount) { (void)self; ++slide_calls; order[order_count++] = 5; slide_amount = amount; }
extern "C" void move2__8CreatureG6Point3(void* self, const void* point) { (void)self; ++move_calls; order[order_count++] = 7; std::memcpy(moved, point, 12); }
extern "C" int bExitStatePersueIdle__8Creature(void* self) { (void)self; ++exit_calls; order[order_count++] = 8; return exit_result; }
extern "C" void smartWander__8Creature(void* self) { (void)self; ++wander_calls; order[order_count++] = 9; }
extern "C" void objectRemoveFromRunList__FP10GameObject(void* self) { (void)self; ++remove_calls; order[order_count++] = 10; }
extern "C" void slot_168(void* self) { ++a168_calls; order[order_count++] = 6; a168_self = self; }
extern "C" void set_state_stub(void* self, int value) { ++state_calls; order[order_count++] = 11; state_self = self; state_value = value; }
static unsigned char object[900], group[32], model[4], table[200];
static void fresh(int state) {
    std::memset(object, 0, sizeof object);
    damage_calls = frame_calls = check_calls = advance_calls = slide_calls = move_calls = exit_calls = wander_calls = remove_calls = state_calls = a168_calls = order_count = 0;
    frame_result = 0; exit_result = 0;
    write_word(address(object) + 36u, address_bits(group));
    write_word(address(object) + 72u, address_bits(table) - 168u);
    write_u16(address(object) + 572u, static_cast<unsigned>(state) & 0xffffu);
}
static void lt(bool less) {  // word 256 below word 808 or not
    write_word(address(object) + 256u, less ? 0xffffffffu : 5u);
    write_word(address(object) + 808u, less ? 0u : 5u);
}
int main() {
    host_note(group); host_note(model); host_note(table - 168);
    write_word(address(group) + 12u, address_bits(model));
    write_u16(address(table), 4u);  // slot 168
    write_word(address(table) + 4u, address_bits(reinterpret_cast<void*>(slot_168)));
    write_u16(address(table) + 16u, 8u);  // slot 184
    write_word(address(table) + 20u, address_bits(reinterpret_cast<void*>(set_state_stub)));
    host_note(reinterpret_cast<void*>(slot_168));
    host_note(reinterpret_cast<void*>(set_state_stub));

    fresh(2);
    msg_run__7Mermaid(object);
    expect(damage_model == model && frame_model == model, "handlers get the group +12 word");
    expect(check_distance == 600 && advance_amount == 1.0f, "600 and 1.0");
    expect(slide_amount > 0.19999f && slide_amount < 0.20001f, "slideBack gets 0.2");
    expect(a168_self == object + 4, "slot 168 with the adjusted this");
    expect(moved[0] == 0.0f && moved[1] == 0.0f && moved[2] == 0.0f && move_calls == 1, "move2 with a zero point");
    expect(order_count == 7 && order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4 && order[4] == 5 && order[5] == 6 && order[6] == 7, "common order");
    fresh(2); frame_result = 1;
    msg_run__7Mermaid(object);
    expect(order_count == 2, "non-zero everyFrame returns early");

    const int nothing[] = {2, 4, 6, 7, 9, 11, 12, -1, 100, -32768};
    for (int s : nothing) {
        fresh(s); lt(true); exit_result = 1; write_u16(address(object) + 294u, 0x90u);
        msg_run__7Mermaid(object);
        expect(order_count == 7 && state_calls == 0 && wander_calls == 0 && remove_calls == 0 && exit_calls == 0, "idle states do nothing");
    }
    fresh(0); lt(true);
    msg_run__7Mermaid(object);
    expect(state_calls == 1 && state_value == 1 && state_self == object + 8, "state 0 below: setState(1)");
    fresh(0); lt(false);
    msg_run__7Mermaid(object);
    expect(state_calls == 0 && wander_calls == 0, "state 0 not below: nothing");
    fresh(1); lt(true);
    msg_run__7Mermaid(object);
    expect(state_calls == 1 && state_value == 0 && wander_calls == 0, "state 1 below: setState(0)");
    fresh(1); lt(false);
    msg_run__7Mermaid(object);
    expect(state_calls == 0 && wander_calls == 1, "state 1 not below: smartWander");
    fresh(3); write_word(address(object) + 576u, 0u);
    msg_run__7Mermaid(object);
    expect(state_calls == 1 && state_value == 0, "state 3 with no target: setState(0)");
    fresh(3); write_word(address(object) + 576u, 0x1000u);
    msg_run__7Mermaid(object);
    expect(state_calls == 0, "state 3 with a target: nothing");
    fresh(5); exit_result = 0;
    msg_run__7Mermaid(object);
    expect(exit_calls == 1 && state_calls == 0, "state 5 stays");
    fresh(5); exit_result = -4;
    msg_run__7Mermaid(object);
    expect(exit_calls == 1 && state_calls == 1 && state_value == 3, "state 5 leaves to 3");
    fresh(8); write_u16(address(object) + 294u, 0x10u);
    msg_run__7Mermaid(object);
    expect(state_calls == 0, "state 8 with bit 0x10: nothing");
    fresh(8); write_u16(address(object) + 294u, 0xffefu);
    msg_run__7Mermaid(object);
    expect(state_calls == 1 && state_value == 0, "state 8 without bit 0x10: setState(0)");
    fresh(10); write_u16(address(object) + 294u, 0x7fu);
    msg_run__7Mermaid(object);
    expect(remove_calls == 0 && state_calls == 0, "state 10 without bit 0x80");
    fresh(10); write_u16(address(object) + 294u, 0x80u);
    msg_run__7Mermaid(object);
    expect(remove_calls == 1 && state_calls == 0, "state 10 with bit 0x80 removes");
    return failures == 0 ? 0 : 1;
}
