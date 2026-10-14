#define NORRATH_W8C_HOST 1
#include "set_state.cpp"
#include "../w8-actor-virtuals-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static int model_calls = 0, add_calls = 0, end_calls = 0, slot_calls = 0;
static void* seen_model = nullptr;
static int seen_which = 0;
static void* seen_add_state = nullptr;
static void* seen_add_header = nullptr;
static int seen_add_count = 0;
static void* seen_slot_self = nullptr;
static int seen_slot_arg = -1;
static unsigned char* g_object = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which) {
    ++model_calls; seen_model = model; seen_which = which;
    expect(g_object[156] == 0x44, "byte 156 is cleared after the model call");
    return 0x8877665544332211ull;
}
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int count) {
    ++add_calls; seen_add_state = state; seen_add_header = header; seen_add_count = count;
}
extern "C" void norrath_animation_set_to_end(void* state) { ++end_calls; expect(state == seen_add_state, "end on the same state"); }
static void slot(void* self, int arg) { ++slot_calls; seen_slot_self = self; seen_slot_arg = arg; }

static void run(int state, unsigned limit, unsigned header, unsigned flags) {
    unsigned char object[600];
    unsigned char table[256];
    std::memset(object, 0x44, sizeof object);
    std::memset(table, 0, sizeof table);
    g_object = object;
    w8c::host_install_slot(object, table, 152u, 16, reinterpret_cast<void*>(slot));
    const w8c::Address a = w8c::address(object);
    w8c::write_word(a + 288u, limit);
    w8c::write_word(a + 144u, 0x1000u);
    w8c::host_note(reinterpret_cast<void*>(0x1000u));
    const int idx = state;
    w8c::write_word(a + 296u + 8u * static_cast<unsigned>(idx), header);
    w8c::write_word(a + 300u + 8u * static_cast<unsigned>(idx), flags);
    model_calls = add_calls = end_calls = slot_calls = 0;
    setState__8AutoPropi(object, state);
    if (!(state < static_cast<int>(limit))) {
        expect(model_calls == 0 && slot_calls == 0, "out-of-range state does nothing");
        expect(w8c::read_word(a + 376u) == 0x44444444u, "no store");
        return;
    }
    expect(model_calls == 1 && slot_calls == 1, "model and slot called once");
    expect(w8c::read_word(a + 376u) == static_cast<unsigned>(state) && w8c::read_word(a + 380u) == 0u, "state stored, counter zero");
    unsigned long long mask = 0;
    if (state > 0) mask = state >= 31 ? ~0ull : ((1ull << (state + 1)) - 1);
    expect(w8c::read_u64(a + 384u) == mask, "mask");
    expect(seen_which == static_cast<int>(mask & 0xffffffffu), "low word of mask passed");
    expect(w8c::read_u64(a + 200u) == 0x8877665544332211ull, "64-bit chunk result stored");
    expect(object[156] == 0, "byte 156 cleared");
    if (header != 0) {
        expect(add_calls == 1 && end_calls == 1 && seen_add_state == object + 152, "animation added to object+152");
        expect(seen_add_count == ((flags & 2u) ? 3 : 6), "count follows bit 2");
    } else {
        expect(add_calls == 0 && end_calls == 0, "no animation without a header");
    }
    expect(seen_slot_self == object + 16 && seen_slot_arg == static_cast<int>(flags & 1u), "slot receives adjusted object and bit 0");
}
int main() {
    run(0, 5u, 0u, 0u);
    run(0, 5u, 0x2000u, 1u);
    run(2, 5u, 0x2000u, 2u);
    run(4, 5u, 0x2000u, 3u);
    run(5, 5u, 0x2000u, 3u);
    run(6, 5u, 0x2000u, 3u);
    run(30, 31u, 0x2000u, 0u);
    run(31, 33u, 0x2000u, 0u);
    run(40, 50u, 0x2000u, 0u);
    return failures ? 1 : 0;
}
