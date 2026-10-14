#define NORRATH_VSLOTS_B_HOST 1
#include "msg_get_poke_name.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static char target_value[] = "Gate";
static bool has_tag = false;
static bool wrong_tag = false;
static int find_calls = 0;
static unsigned char other[96];
static unsigned char table[128];
static bool object_exists = true;
static char* looked_up = nullptr;
static int slot_calls = 0;
static void* slot_object = nullptr;
static unsigned short* slot_buffer = nullptr;
static int slot_value = 0;
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int occurrence) {
    ++find_calls;
    if (std::strcmp(tag, "target") != 0 || occurrence != 0) wrong_tag = true;
    return has_tag ? target_value : nullptr;
}
extern "C" unsigned char* objectFindObject__FPc(char* name) { looked_up = name; return object_exists ? other : nullptr; }
extern "C" int slot_stub(void* object, unsigned short* buffer, int value) {
    ++slot_calls; slot_object = object; slot_buffer = buffer; slot_value = value;
    return 77;
}
int main() {
    unsigned char object[96];
    unsigned short buffer[4] = {0x1111, 0x2222, 0x3333, 0x4444};
    std::memset(object, 0, sizeof object);
    std::memset(other, 0, sizeof other);
    std::memset(table, 0, sizeof table);
    vslots_b::host_note(other);
    vslots_b::host_note(table);
    vslots_b::host_note(reinterpret_cast<void*>(slot_stub));
    vslots_b::write_u16(vslots_b::address(table) + 104u, 16u);
    vslots_b::write_word(vslots_b::address(table) + 108u, vslots_b::address_bits(reinterpret_cast<void*>(slot_stub)));
    vslots_b::write_word(vslots_b::address(other) + 72u, vslots_b::address_bits(table));

    has_tag = false;
    expect(msg_getPokeName__13PokeReflectorPUsi(object, buffer, 4) == 0, "no tag returns zero");
    expect(buffer[0] == 0 && buffer[1] == 0x2222, "first halfword cleared only");
    expect(slot_calls == 0 && looked_up == nullptr, "no lookup without a tag");

    has_tag = true;
    object_exists = false;
    buffer[0] = 0x1111;
    expect(msg_getPokeName__13PokeReflectorPUsi(object, buffer, 4) == 0, "missing object returns zero");
    expect(looked_up == target_value && buffer[0] == 0 && slot_calls == 0, "named lookup, buffer cleared, no call");

    object_exists = true;
    buffer[0] = 0x1111;
    expect(msg_getPokeName__13PokeReflectorPUsi(object, buffer, 4) == 77, "returns the slot result");
    expect(slot_calls == 1 && slot_object == other + 16 && slot_buffer == buffer && slot_value == 4, "slot byte 104 on the adjusted object");
    expect(buffer[0] == 0 && !wrong_tag, "buffer cleared before the call, tag is target at 0");
    return failures ? 1 : 0;
}
