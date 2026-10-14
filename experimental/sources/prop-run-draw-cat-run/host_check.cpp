#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include <vector>
#include "msg_run.cpp"
using namespace vslots_b;
static int failures = 0;
// Event codes: 1 damage, 2 frame, 3 check, 4 advance, 5 slide, 6 addTurn, 7 subTurn, 8 ready, 9 rand,
// 10 walk, 11 run, 12 wander, 13 oneshot, 100+n setState(n).
static std::vector<int> events;
static std::vector<unsigned> rands;
static unsigned rand_index;
static int ready_result, frame_result, check_distance;
static void *damage_model, *frame_model, *state_self, *shot_state, *shot_header;
static float advance_amount, slide_amount, shot_first, shot_second;
static int shot_flags, shot_count;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model) { (void)self; events.push_back(1); damage_model = model; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void* self, void* model) { (void)self; events.push_back(2); frame_model = model; return frame_result; }
extern "C" int objectActivationCheck__8Creaturei(void* self, int distance) { (void)self; events.push_back(3); check_distance = distance; return 0; }
extern "C" void creatureAdvanceAnimation__8Creaturef(void* self, float amount) { (void)self; events.push_back(4); advance_amount = amount; }
extern "C" void slideBack__8Creaturef(void* self, float amount) { (void)self; events.push_back(5); slide_amount = amount; }
extern "C" void addTurnAnimation__3Cat(void* self) { (void)self; events.push_back(6); }
extern "C" void subTurnAnimation__3Cat(void* self) { (void)self; events.push_back(7); }
extern "C" int readyToSwitch__3Cat(void* self) { (void)self; events.push_back(8); return ready_result; }
extern "C" unsigned eRand__10GameObject(void* self) { (void)self; events.push_back(9); return rand_index < rands.size() ? rands[rand_index++] : 0xdeadu; }
extern "C" void switchToWalk__3Cat(void* self) { (void)self; events.push_back(10); }
extern "C" void switchToRun__3Cat(void* self) { (void)self; events.push_back(11); }
extern "C" void smartWander__8Creature(void* self) { (void)self; events.push_back(12); }
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float first, float second, int flags) {
    events.push_back(13); shot_state = state; shot_header = header; shot_first = first; shot_second = second; shot_flags = flags; ++shot_count;
}
extern "C" void set_state_stub(void* self, int value) { events.push_back(100 + value); state_self = self; }
static unsigned char object[1000], group[200], model[4], table[200], headers[8][4];
static void fresh(int state) {
    std::memset(object, 0, sizeof object);
    events.clear(); rands.clear(); rand_index = 0; shot_count = 0;
    ready_result = 0; frame_result = 0;
    write_word(address(object) + 36u, address_bits(group));
    write_word(address(object) + 72u, address_bits(table) - 184u);
    write_u16(address(object) + 572u, static_cast<unsigned>(state) & 0xffffu);
}
static void words(int a, int b, int c, int d) {
    write_word(address(object) + 968u, static_cast<unsigned>(a));
    write_word(address(object) + 808u, static_cast<unsigned>(b));
    write_word(address(object) + 972u, static_cast<unsigned>(c));
    write_word(address(object) + 964u, static_cast<unsigned>(d));
}
static std::vector<int> tail() { return std::vector<int>(events.begin() + 5 + 1, events.end()); }  // after the 5 common events and the turn call
static bool tail_is(std::initializer_list<int> want) { return tail() == std::vector<int>(want); }
static unsigned word(unsigned offset) { return read_word(address(object) + offset); }
int main() {
    host_note(group); host_note(model); host_note(table - 184);
    for (int i = 0; i < 8; ++i) host_note(headers[i]);
    write_word(address(group) + 12u, address_bits(model));
    write_word(address(group) + 124u, address_bits(headers[0]));
    write_word(address(group) + 140u, address_bits(headers[1]));
    write_word(address(group) + 148u, address_bits(headers[2]));
    write_u16(address(table), 8u);
    write_word(address(table) + 4u, address_bits(reinterpret_cast<void*>(set_state_stub)));
    host_note(reinterpret_cast<void*>(set_state_stub));

    fresh(5);
    msg_run__3Cat(object);
    expect(damage_model == model && frame_model == model && check_distance == 600 && advance_amount == 1.0f && slide_amount == 1.0f, "common calls");
    expect(events.size() == 6 && events[0] == 1 && events[1] == 2 && events[2] == 3 && events[3] == 4 && events[4] == 5 && events[5] == 7, "state 5: common calls and subTurn only");
    fresh(5); frame_result = 3;
    msg_run__3Cat(object);
    expect(events.size() == 2, "non-zero everyFrame returns early");
    const int add_states[] = {1, 3, 7};
    for (int s : add_states) {
        fresh(s); words(9, 1, 9, 0);
        msg_run__3Cat(object);
        expect(events[5] == 6, "states 1, 3, 7 use addTurnAnimation");
    }
    const int sub_states[] = {0, 2, 4, 5, 6, 8, 13, -1, 14, 100};
    for (int s : sub_states) {
        fresh(s); words(9, 1, 9, 0);
        msg_run__3Cat(object);
        expect(events[5] == 7, "other states use subTurnAnimation");
    }
    const int idle_states[] = {2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, -1, 1000, -32768};
    for (int s : idle_states) {
        fresh(s); words(0, 100, 0, 0); ready_result = 1; rands = {0, 0, 0};
        msg_run__3Cat(object);
        expect(events.size() == 6, "unlisted states stop after the turn call");
    }

    // State 0.
    fresh(0); words(1, 2, 100, 0); ready_result = 1; rands = {0};
    msg_run__3Cat(object);
    expect(tail_is({8, 9, 10, 101}) && state_self == object + 8, "state 0 ready, bit 0: walk then setState(1)");
    fresh(0); words(1, 2, 100, 0); ready_result = 1; rands = {3};
    msg_run__3Cat(object);
    expect(tail_is({8, 9, 11, 103}), "state 0 ready, bit 1: run then setState(3)");
    fresh(0); words(1, 2, 100, 0); ready_result = 0;
    msg_run__3Cat(object);
    expect(tail_is({8}) && shot_count == 0, "state 0 not ready and C not below B: only the ready call");
    fresh(0); words(2, 2, 100, 0); ready_result = 1;
    msg_run__3Cat(object);
    expect(tail_is({}), "state 0 with A == B: nothing, readyToSwitch not called");
    fresh(0); words(2, 2, 1, 0); rands = {7};
    msg_run__3Cat(object);
    expect(tail_is({9, 13}) && word(972) == 2u + 7u + 120u, "state 0 timer: C = B + r % 300 + 120");
    expect(shot_state == object + 288 && shot_header == headers[0] && shot_first == shot_second && shot_flags == 32, "state 0 one-shot arguments");
    expect(shot_first > 0.00999f && shot_first < 0.01001f, "K0 is 0.01");
    fresh(0); words(2, 2, 1, 0); rands = {305};
    msg_run__3Cat(object);
    expect(word(972) == 2u + 5u + 120u, "remainder is mod 300");
    fresh(0); words(2, 2, 1, 0); rands = {0xfffffffeu};
    msg_run__3Cat(object);
    expect(word(972) == 2u + (0xfffffffeu % 300u) + 120u, "remainder is unsigned");
    fresh(0); words(2, 2, 1, 0); write_u16(address(object) + 294u, 0x20u); rands = {7};
    msg_run__3Cat(object);
    expect(tail_is({}) && word(972) == 1u, "bit 0x20 blocks the timer");
    fresh(0); words(2, 2, 1, 0); write_u16(address(object) + 294u, 0xffdfu); rands = {7};
    msg_run__3Cat(object);
    expect(tail_is({9, 13}), "only bit 0x20 blocks");
    fresh(0); words(2, 2, 2, 0); rands = {7};
    msg_run__3Cat(object);
    expect(tail_is({}), "C == B does not fire");
    fresh(0); words(1, 2, 1, 0); ready_result = 0; rands = {7};
    msg_run__3Cat(object);
    expect(tail_is({8, 9, 13}), "not ready falls through to the timer");
    fresh(0); words(-5, 2, -3, 0); ready_result = 0; rands = {7};
    msg_run__3Cat(object);
    expect(tail_is({8, 9, 13}), "signed compares");

    // State 13.
    fresh(13); words(1, 2, 100, 0); ready_result = 1; rands = {2};
    msg_run__3Cat(object);
    expect(tail_is({8, 9, 10, 101}), "state 13 ready: walk, setState(1)");
    fresh(13); words(1, 2, 100, 0); ready_result = 1; rands = {1};
    msg_run__3Cat(object);
    expect(tail_is({8, 9, 11, 103}), "state 13 ready: run, setState(3)");
    fresh(13); words(1, 2, 100, 5); ready_result = 1;
    msg_run__3Cat(object);
    expect(tail_is({}), "state 13 with D non-zero skips readyToSwitch and the timer");
    fresh(13); words(1, 2, 100, 0); ready_result = 1; write_u16(address(object) + 294u, 0x8u);
    msg_run__3Cat(object);
    expect(tail_is({8}), "state 13 with bit 0x8 stops after readyToSwitch");
    fresh(13); words(1, 2, 100, 0); ready_result = 0;
    msg_run__3Cat(object);
    expect(tail_is({8}), "state 13 not ready");
    fresh(13); words(3, 2, 1, 0); rands = {481, 0};
    msg_run__3Cat(object);
    expect(tail_is({9, 9, 13}) && word(972) == 2u + 1u + 240u, "state 13 timer: C = B + r % 480 + 240");
    expect(shot_header == headers[1] && shot_state == object + 288 && shot_flags == 32, "bit 0 clear: group +140");
    expect(shot_first > 0.19999f && shot_first < 0.20001f && shot_second > 0.29999f && shot_second < 0.30001f, "K1 0.2 and K3 0.3");
    fresh(13); words(3, 2, 1, 5); rands = {0, 1};
    msg_run__3Cat(object);
    expect(tail_is({9, 9, 13}) && shot_header == headers[2], "bit 0 set: group +148");
    expect(shot_first > 0.19999f && shot_first < 0.20001f && shot_second > 0.29999f && shot_second < 0.30001f, "K2 and K3");
    fresh(13); words(3, 2, 1, 0); write_u16(address(object) + 294u, 0x20u); rands = {1, 1};
    msg_run__3Cat(object);
    expect(tail_is({}), "state 13: bit 0x20 blocks the timer");
    fresh(13); words(3, 2, 2, 0); rands = {1, 1};
    msg_run__3Cat(object);
    expect(tail_is({}), "state 13: C == B does not fire");
    fresh(13); words(3, 2, 1, 0); write_u16(address(object) + 294u, 0x8u); rands = {1, 1};
    msg_run__3Cat(object);
    expect(tail_is({9, 9, 13}), "state 13: bit 0x8 does not block the timer");

    // States 1 and 3 share a body.
    const int pair[] = {1, 3};
    for (int s : pair) {
        fresh(s); words(1, 2, 0, 0); rands = {0};
        msg_run__3Cat(object);
        expect(tail_is({9, 113}) && state_self == object + 8, "A < B, bit 0 clear: setState(13)");
        fresh(s); words(1, 2, 0, 0); rands = {1};
        msg_run__3Cat(object);
        expect(tail_is({9, 100}), "A < B, bit 0 set: setState(0)");
        fresh(s); words(2, 2, 0, 0); rands = {0};
        msg_run__3Cat(object);
        expect(tail_is({12}), "A == B: smartWander");
        fresh(s); words(-1, -2, 0, 0); rands = {0};
        msg_run__3Cat(object);
        expect(tail_is({12}), "signed: -1 is not below -2");
        fresh(s); words(-3, -2, 0, 0); rands = {0};
        msg_run__3Cat(object);
        expect(tail_is({9, 113}), "signed: -3 is below -2");
    }
    return failures == 0 ? 0 : 1;
}
