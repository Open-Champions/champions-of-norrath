#define NORRATH_VSLOTS_B_HOST 1
#include "msg_poke.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static int actions = 0;
static void* action_object = nullptr;
static int action_value = -1;
static int slot_calls = 0;
static void* slot_object = nullptr;
static int slot_value = -1;
static int order = 0;
static int action_order = 0;
static int slot_order = 0;
extern "C" void objectDoMyActions__FP10GameObjecti(void* object, int value) {
    ++actions; action_object = object; action_value = value; action_order = ++order;
}
extern "C" void slot_stub(void* object, int value) {
    ++slot_calls; slot_object = object; slot_value = value; slot_order = ++order;
}
int main() {
    unsigned char object[96];
    unsigned char table[32];
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    vslots_b::host_note(object);
    vslots_b::host_note(table);
    vslots_b::host_note(reinterpret_cast<void*>(slot_stub));
    vslots_b::write_u16(vslots_b::address(table) + 8u, 12u);
    vslots_b::write_word(vslots_b::address(table) + 12u, vslots_b::address_bits(reinterpret_cast<void*>(slot_stub)));
    vslots_b::write_word(vslots_b::address(object) + 72u, vslots_b::address_bits(table));
    msg_poke__13UserParamPropi(object, 9);
    expect(actions == 1 && action_object == object && action_value == 1, "actions run with 1");
    expect(slot_calls == 1 && slot_object == object + 12 && slot_value == 3, "slot byte 8 called with 3 on the adjusted object");
    expect(action_order < slot_order, "actions before the slot call");
    actions = 0; slot_calls = 0;
    msg_poke__13UserParamPropi(nullptr, 9);
    expect(actions == 1 && action_object == nullptr && slot_calls == 0, "null object runs actions only");
    return failures ? 1 : 0;
}
