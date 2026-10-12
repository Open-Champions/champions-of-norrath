// Host contract for objectActivationCheck__8Creaturei. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct GameObject;
struct Creature {
    unsigned char before_flags[12];
    unsigned object_flags;
    unsigned char before_state[160 - 16];
    unsigned long long state_bits;
    unsigned char before_active[236 - 168];
    int active;
    int previous_active;
    unsigned char before_animation[288 - 244];
    unsigned char animation_state[1];
};
extern "C" int objectActivationCheck__8Creaturei(Creature*, int);

static int processing_result, processing_calls, seen_previous, seen_argument, seen_previous_copy;
static GameObject* seen_object;
static Creature* current;
static char* tag_result;
static const char* seen_tag;
static int tag_calls, tag_index, script_calls, reset_calls, order, processing_order, tag_order, script_order, reset_order;
static char* started;
static void* reset_target;
static int reset_changes_active = -1;
extern "C" int objectActiveProcessing__FP10GameObjectii(GameObject* o, int previous, int argument) {
    ++processing_calls; seen_object = o; seen_previous = previous; seen_argument = argument;
    seen_previous_copy = current->previous_active;
    processing_order = ++order;
    return processing_result;
}
extern "C" char* findTagString__10GameObjectPci(GameObject*, const char* tag, int index) { ++tag_calls; seen_tag = tag; tag_index = index; tag_order = ++order; return tag_result; }
extern "C" void scriptStart__FPc(char* script) { ++script_calls; started = script; script_order = ++order; }
extern "C" void animResetFramestates__FP14AnimationState(void* state) {
    ++reset_calls; reset_target = state; reset_order = ++order;
    if (reset_changes_active >= 0) current->active = reset_changes_active;
}

static void reset_counters() { processing_calls = tag_calls = script_calls = reset_calls = order = 0; seen_tag = nullptr; started = nullptr; }

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    static Creature c;
    current = &c;
    char script_name[] = "script";
    tag_result = script_name;

    // Becomes active with a follow flag: found script.
    std::memset(&c, 0, sizeof c);
    c.object_flags = 0x00400000; c.active = 0;
    processing_result = 7;
    reset_counters();
    int r = objectActivationCheck__8Creaturei(&c, 123);
    expect(r == 1 && c.active == 1, "any nonzero result becomes 1");
    expect(seen_object == reinterpret_cast<GameObject*>(&c) && seen_previous == 0 && seen_argument == 123, "processing gets (creature, previous, argument)");
    expect(seen_previous_copy == 0 && c.previous_active == 0, "previous copy stored before the call");
    expect(tag_calls == 1 && std::strcmp(seen_tag, "followFoundScript") == 0 && tag_index == 0, "activation looks up followFoundScript");
    expect(script_calls == 1 && started == script_name && script_order > tag_order && tag_order > processing_order, "script started after the lookup");
    expect(reset_calls == 0, "no animation reset while active");

    // Becomes inactive: lost script, then animation reset; the reset's effect on the flag is returned.
    c.active = 1; c.object_flags = 0x00800000;
    processing_result = 0;
    reset_counters();
    r = objectActivationCheck__8Creaturei(&c, 5);
    expect(r == 0 && c.previous_active == 1 && seen_previous == 1, "previous 1 is passed and copied");
    expect(std::strcmp(seen_tag, "followLostScript") == 0 && script_calls == 1, "deactivation looks up followLostScript");
    expect(reset_calls == 1 && reset_target == c.animation_state && reset_order > script_order, "animation state reset last");
    reset_changes_active = 1;
    c.active = 1; reset_counters();
    r = objectActivationCheck__8Creaturei(&c, 5);
    expect(r == 1, "return value is read after the reset");
    reset_changes_active = -1;

    // No follow flag: no tag lookup even when the state changes.
    c.object_flags = 0x003fffff | 0xff000000; c.active = 0; processing_result = 1; reset_counters();
    objectActivationCheck__8Creaturei(&c, 0);
    expect(tag_calls == 0 && script_calls == 0, "flag mask 0x00c00000 gates the scripts");
    // Unchanged state: no lookup.
    c.object_flags = 0x00c00000; c.active = 1; processing_result = 3; reset_counters();
    objectActivationCheck__8Creaturei(&c, 0);
    expect(tag_calls == 0, "no lookup when the flag did not change");
    // Missing tag: no script.
    c.active = 0; tag_result = nullptr; reset_counters();
    objectActivationCheck__8Creaturei(&c, 0);
    expect(tag_calls == 1 && script_calls == 0, "null lookup starts nothing");
    tag_result = script_name;
    // Bit 28 of the state word replaces the argument with 0xabe0.
    c.state_bits = 0x10000000ull; reset_counters();
    objectActivationCheck__8Creaturei(&c, 99);
    expect(seen_argument == 0xabe0, "bit 28 forces the argument to 0xabe0");
    c.state_bits = 0x0fffffffull | 0xffffffff00000000ull; reset_counters();
    objectActivationCheck__8Creaturei(&c, 99);
    expect(seen_argument == 99, "other bits, including the high word, leave it alone");
    c.state_bits = 0x100000000ull << 3; reset_counters();
    objectActivationCheck__8Creaturei(&c, 41);
    expect(seen_argument == 41, "only bit 28 of the low word counts");
    if (failures) return 1;
    std::printf("objectActivationCheck contract passed\n");
    return 0;
}
