#define NORRATH_VSLOTS_B_HOST 1
#include "msg_poke.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static char target_value[] = "Gate";
static bool has_tag = false;
static bool wrong_tag = false;
static unsigned fill = 0;
static void* fill_objects[4];
static int find_calls = 0;
static int fill_calls = 0;
static int fill_count = 0;
static char* fill_name = nullptr;
static unsigned char table[128];
static unsigned char objects[3][96];
static void* seen_object[4];
static int seen_value[4];
static int slot_calls = 0;
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int occurrence) {
    ++find_calls;
    if (std::strcmp(tag, "target") != 0 || occurrence != 0) wrong_tag = true;
    return has_tag ? target_value : nullptr;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int count) {
    ++fill_calls; fill_name = name; fill_count = count;
    for (unsigned i = 0; i < fill; ++i) list[i] = fill_objects[i];
    list[fill] = nullptr;
}
extern "C" void slot_stub(void* object, int value) {
    seen_object[slot_calls] = object; seen_value[slot_calls] = value; ++slot_calls;
}
int main() {
    unsigned char self[96];
    std::memset(self, 0, sizeof self);
    std::memset(table, 0, sizeof table);
    vslots_b::host_note(table);
    vslots_b::host_note(reinterpret_cast<void*>(slot_stub));
    for (auto& o : objects) {
        std::memset(o, 0, sizeof o);
        vslots_b::host_note(o);
        vslots_b::write_word(vslots_b::address(o) + 72u, vslots_b::address_bits(table));
    }
    vslots_b::write_u16(vslots_b::address(table) + 112u, 8u);
    vslots_b::write_word(vslots_b::address(table) + 116u, vslots_b::address_bits(reinterpret_cast<void*>(slot_stub)));
    fill_objects[0] = objects[0]; fill_objects[1] = objects[1]; fill_objects[2] = objects[2];

    has_tag = false;
    msg_poke__13PokeReflectori(self, 6);
    expect(fill_calls == 0 && slot_calls == 0, "no tag does nothing");

    has_tag = true;
    fill = 0;
    msg_poke__13PokeReflectori(self, 6);
    expect(fill_calls == 1 && fill_name == target_value && fill_count == 16, "list asked for 16 by name");
    expect(slot_calls == 0, "empty list calls nothing");

    fill = 3;
    fill_calls = 0;
    msg_poke__13PokeReflectori(self, 6);
    expect(slot_calls == 3, "three objects poked");
    for (int i = 0; i < 3; ++i) {
        expect(seen_object[i] == objects[i] + 8 && seen_value[i] == 6, "slot byte 112 on the adjusted object with the argument");
    }
    expect(!wrong_tag, "tag is target at 0");
    return failures ? 1 : 0;
}
