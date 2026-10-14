#define NORRATH_VSLOTS_B_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include "msg_run.cpp"
using namespace vslots_b;
static int failures = 0, order_count = 0, order[8];
static int remove_calls, action_calls, action_value, tag_calls;
static void *remove_object, *action_object;
static int on_result, off_result;
static char tags[4][16];
static int tag_occurrence[4];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-5f; }
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence) {
    (void)object;
    order[order_count++] = 2;
    if (tag_calls < 4) { std::strncpy(tags[tag_calls], tag, 15); tag_occurrence[tag_calls] = occurrence; }
    ++tag_calls;
    return std::strcmp(tag, "onOnly") == 0 ? on_result : std::strcmp(tag, "offOnly") == 0 ? off_result : -1000;
}
extern "C" void objectRemoveFromRunList__FP10GameObject(void* object) { ++remove_calls; remove_object = object; order[order_count++] = 1; }
extern "C" void objectDoMyActions__FP10GameObjecti(void* object, int value) {
    ++action_calls; action_object = object; action_value = value; order[order_count++] = 3;
}
static unsigned char object[100];
static void fresh(float progress, int direction) {
    std::memset(object, 0, sizeof object);
    write_single(address(object) + 76u, progress);
    write_u8(address(object) + 80u, static_cast<unsigned>(direction));
    write_u8(address(object) + 81u, 1u);
    write_word(address(object) + 12u, 0x5u);
    remove_calls = action_calls = tag_calls = order_count = 0;
    on_result = off_result = 0;
}
static float progress() { return read_single(address(object) + 76u); }
static unsigned flags() { return read_word(address(object) + 12u); }
int main() {
    fresh(0.5f, 1);
    write_u8(address(object) + 81u, 0u);
    msg_run__5Lever(object);
    expect(near(progress(), 0.5f) && remove_calls == 0 && read_u8(address(object) + 80u) == 1u, "idle lever does nothing");

    fresh(0.5f, 1);
    msg_run__5Lever(object);
    expect(near(progress(), 0.5f + 1.5f / 60.0f), "one step is 1.5/60 per direction unit");
    expect(remove_calls == 0 && action_calls == 0 && read_u8(address(object) + 81u) == 1u && read_u8(address(object) + 80u) == 1u, "mid-move changes nothing else");
    fresh(0.5f, -2);
    msg_run__5Lever(object);
    expect(near(progress(), 0.5f - 3.0f / 60.0f) && read_u8(address(object) + 80u) == 0xfeu, "direction is a signed byte");

    fresh(0.99f, 1);
    msg_run__5Lever(object);
    expect(progress() == 1.0f, "progress clamps at 1.0");
    expect(remove_calls == 1 && remove_object == object && read_u8(address(object) + 81u) == 0u, "removed and idle");
    expect(action_calls == 1 && action_object == object && action_value == 1, "action argument is true for direction 1");
    expect(read_u8(address(object) + 80u) == 0xffu, "direction negated");
    expect(flags() == (0x5u | 0x100000u), "no tag: bit 0x100000 set");
    expect(tag_calls == 2 && std::strcmp(tags[0], "onOnly") == 0 && std::strcmp(tags[1], "offOnly") == 0 && tag_occurrence[0] == 0 && tag_occurrence[1] == 0, "tags and occurrence");
    expect(order_count == 4 && order[0] == 1 && order[1] == 2 && order[2] == 2 && order[3] == 3, "remove, two tags, action");

    fresh(0.01f, -1);
    msg_run__5Lever(object);
    expect(progress() == 0.0f && remove_calls == 1 && action_value == 0, "clamps at 0.0, argument false for direction -1");
    expect(read_u8(address(object) + 80u) == 1u, "-1 negates to 1");

    fresh(0.99f, 1);
    on_result = 7;
    msg_run__5Lever(object);
    expect(action_value == 1 && flags() == 0x5u, "onOnly found: argument 1, flag untouched");
    fresh(0.01f, -1);
    on_result = 7;
    msg_run__5Lever(object);
    expect(action_value == 1 && flags() == 0x5u, "onOnly found forces argument 1");
    fresh(0.99f, 1);
    off_result = -3;
    msg_run__5Lever(object);
    expect(action_value == 0 && flags() == 0x5u, "offOnly found: argument 0, flag untouched");
    fresh(0.99f, 1);
    on_result = 1; off_result = 1;
    msg_run__5Lever(object);
    expect(action_value == 0 && flags() == 0x5u, "both found: offOnly wins");

    fresh(1.0f, 0);
    msg_run__5Lever(object);
    expect(remove_calls == 1 && action_value == 0 && read_u8(address(object) + 80u) == 0u, "exactly 1.0 counts as done");
    fresh(0.0f, 0);
    msg_run__5Lever(object);
    expect(remove_calls == 0, "exactly 0.0 is not below zero");
    fresh(0.5f, 0);
    write_u8(address(object) + 81u, 0x80u);  // negative signed byte is non-zero
    msg_run__5Lever(object);
    expect(near(progress(), 0.5f) && remove_calls == 0, "any non-zero byte at +81 runs the body");
    fresh(0.99f, 2);
    msg_run__5Lever(object);
    expect(action_value == 0, "direction 2 is not 1");
    return failures == 0 ? 0 : 1;
}
