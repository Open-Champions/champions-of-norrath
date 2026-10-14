#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "get_inst_type.cpp"
using namespace vslots_b;
static int failures, lookups, calls, lookup_index[8], call_order[8];
static void* lookup_scene;
static void* call_scene[8];
static void* call_self[8];
static unsigned char sprites[4][80], tables[4][48], scene_obj[8];
static int answers[4];
static int shrink_at = -1;
static unsigned char* group_obj;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" void* Sprite__7VIScenei(void* scene, int index) {
    lookup_scene = scene; lookup_index[lookups++] = index;
    return sprites[index];
}
extern "C" int slot(void* self, void* scene) {
    int which = -1;
    for (int i = 0; i < 4; ++i) if (static_cast<unsigned char*>(self) == sprites[i] + (i == 2 ? 8 : 0)) which = i;
    call_self[calls] = self; call_scene[calls] = scene; call_order[calls++] = which;
    if (which == shrink_at) write_word(address(group_obj) + 64u, 1u);
    return answers[which];
}
static void run(int count, int expected, int expected_calls, const char* m) {
    unsigned char group[68 + 32 * 4];
    group_obj = group;
    std::memset(group, 0, sizeof group);
    write_word(address(group) + 64u, static_cast<unsigned>(count));
    const int idx[4] = {3, 1, 2, 0};
    for (int i = 0; i < 4; ++i) write_word(address(group) + 68u + 32u * i, static_cast<unsigned>(idx[i]));
    lookups = calls = 0;
    const int got = GetInstType__C13VIGroupSpriteR7VIScene(group, scene_obj);
    expect(got == expected, m);
    expect(calls == expected_calls, "number of member calls");
    expect(lookups == calls && (calls == 0 || lookup_scene == scene_obj), "one lookup per call, with the scene");
}
int main() {
    host_note(scene_obj);
    for (int i = 0; i < 4; ++i) {
        std::memset(sprites[i], 0, 80); std::memset(tables[i], 0, 48);
        host_note(tables[i]); host_note(reinterpret_cast<void*>(slot));
        write_word(address(sprites[i]) + 60u, address_bits(tables[i]));
        write_u16(address(tables[i]) + 16u, i == 2 ? 8u : 0u);
        write_word(address(tables[i]) + 20u, address_bits(reinterpret_cast<void*>(slot)));
    }
    run(0, 0, 0, "empty group returns 0");
    run(-5, 0, 0, "negative count returns 0");
    run(4, 0, 4, "no member answers 1: all four called");
    expect(lookup_index[0] == 3 && lookup_index[1] == 1 && lookup_index[2] == 2 && lookup_index[3] == 0, "member indices in order");
    expect(call_order[0] == 3 && call_order[1] == 1 && call_order[2] == 2 && call_order[3] == 0, "call order");
    expect(call_scene[0] == scene_obj, "scene passed to the slot");
    expect(call_self[2] == sprites[2] + 8, "halfword adjustment applied to this");
    answers[1] = 2; answers[3] = 0;
    run(4, 0, 4, "answers other than 1 do not stop");
    answers[1] = 1;
    run(4, 1, 2, "answer 1 stops at the second member");
    answers[1] = 0; answers[0] = 1;
    run(3, 0, 3, "count limits the members visited");
    run(4, 1, 4, "last member can answer 1");
    answers[0] = 0; answers[3] = 1;
    shrink_at = 3;
    run(4, 1, 1, "first member answer 1");
    answers[3] = 0;
    run(4, 0, 1, "count re-read after each member");
    return failures ? 1 : 0;
}
