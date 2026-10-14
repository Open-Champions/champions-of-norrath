#define NORRATH_VSLOTS_B_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include "msg_run.cpp"
using namespace vslots_b;
static int failures = 0;
static int clip_calls, stand_calls, grid_calls, remove_calls, slot_calls;
static void *clip_world, *stand_world, *grid_object, *remove_object, *slot_self;
static float clip_start[3], clip_motion[3], clip_extent, stand_point[3], stand_extent;
static int clip_flag, slot_value;
static float clip_fraction = 1.0f, clip_normal[3] = {0, 0, 0}, stand_height = 0.0f;
static unsigned char world_target[8];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (1.0f + std::fabs(b)); }
extern "C" unsigned char norrath_actors_b_view_4844a0[4] = {0, 0, 0, 0};
extern "C" float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(
    void* world, const void* start, const void* motion, float extent, int flag, void* output) {
    ++clip_calls; clip_world = world; clip_extent = extent; clip_flag = flag;
    std::memcpy(clip_start, start, 12); std::memcpy(clip_motion, motion, 12);
    std::memcpy(output, clip_normal, 12);
    return clip_fraction;
}
extern "C" float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, const void* point, float extent) {
    ++stand_calls; stand_world = world; stand_extent = extent; std::memcpy(stand_point, point, 12);
    return stand_height;
}
extern "C" void objectUpdateInGrid__FP10GameObject(void* object) { ++grid_calls; grid_object = object; }
extern "C" void objectRemoveFromRunList__FP10GameObject(void* object) { ++remove_calls; remove_object = object; }
extern "C" void slot_stub(void* self, int value) { ++slot_calls; slot_self = self; slot_value = value; }

static unsigned char object[400];
static unsigned char table[32];
static void put(unsigned offset, float value) { write_single(address(object) + offset, value); }
static float get(unsigned offset) { return read_single(address(object) + offset); }
static void fresh() {
    std::memset(object, 0, sizeof object);
    clip_calls = stand_calls = grid_calls = remove_calls = slot_calls = 0;
    clip_fraction = 1.0f; stand_height = 0.0f;
    std::memset(clip_normal, 0, sizeof clip_normal);
    put(16, 1.0f); put(20, 2.0f); put(24, 3.0f); put(28, 0.75f);
    write_word(address(object) + 72u, address_bits(table));
    write_u16(address(object) + 270u, 50u);
}
int main() {
    host_note(table);
    host_note(world_target);
    write_word(address(norrath_actors_b_view_4844a0), address_bits(world_target));
    write_u16(address(table) + 8u, 16u);
    write_word(address(table) + 12u, address_bits(reinterpret_cast<void*>(slot_stub)));
    host_note(reinterpret_cast<void*>(slot_stub));

    // Moving without a hit: the full 1/60 scaled motion is added to the point.
    fresh();
    put(252, 60.0f); put(256, 0.0f); put(264, 0.5f); put(48, 27.0f); stand_height = 10.0f;
    msg_run__4Prop(object);
    expect(clip_calls == 1 && grid_calls == 1 && stand_calls == 1, "one clip, grid update and height query");
    expect(clip_world == world_target && stand_world == world_target && clip_flag == 1, "world word, flag 1");
    expect(clip_start[0] == 1.0f && clip_start[1] == 2.0f && clip_start[2] == 3.0f, "clip receives the old point");
    expect(near(clip_motion[0], 1.0f) && clip_motion[1] == 0.0f && clip_motion[2] == 0.0f, "motion is velocity / 60 with z 0");
    expect(clip_extent == 0.75f && stand_extent == 0.75f && grid_object == object, "extent word and grid object");
    expect(near(get(16), 2.0f) && get(20) == 2.0f, "point moved by the motion");
    expect(near(stand_point[0], 2.0f), "height query uses the moved point");
    expect(near(get(252), 60.0f), "no hit leaves the velocity");
    expect(near(get(48), 27.0f - 26.999998f) && get(24) > 2.99f && get(24) < 3.01f, "falling above the floor keeps going");

    // A partial hit moves part way and removes velocity along the normal.
    fresh();
    put(252, 60.0f); put(48, 27.0f); clip_fraction = 0.5f; clip_normal[0] = -1.0f; stand_height = -100.0f;
    msg_run__4Prop(object);
    expect(near(get(16), 1.5f), "half the motion is applied");
    expect(near(get(252), 60.0f - 1.0f * 60.001f * 1.5f) && get(256) == 0.0f, "x velocity gains normal * depth * 1.5");
    fresh();
    put(252, 60.0f); put(48, 27.0f); clip_fraction = 0.5f; clip_normal[0] = 1.0f; stand_height = -100.0f;
    msg_run__4Prop(object);
    expect(near(get(252), 60.0f), "a normal along the velocity (depth below zero) changes nothing");
    fresh();
    put(252, 30.0f); put(256, 40.0f); put(48, 27.0f); clip_fraction = 0.25f; clip_normal[1] = -0.5f; stand_height = -100.0f;
    msg_run__4Prop(object);
    {
        const float depth = 0.001f + 0.5f * 40.0f;
        expect(near(get(256), 40.0f + -0.5f * depth * 1.5f) && near(get(252), 30.0f), "y component uses the y normal");
        expect(near(get(16), 1.0f + 30.0f / 60.0f * 0.25f) && near(get(20), 2.0f + 40.0f / 60.0f * 0.25f), "quarter motion");
    }
    // A hit at fraction exactly 1.0 does not run the normal arm.
    fresh();
    put(252, 60.0f); put(48, 27.0f); clip_fraction = 1.0f; clip_normal[0] = -1.0f; stand_height = -100.0f;
    msg_run__4Prop(object);
    expect(near(get(252), 60.0f), "fraction 1.0 skips the normal arm");

    // Too slow to move: no clip, no grid update, but the height pass runs.
    fresh();
    put(252, 1.0f); put(256, 1.0f); put(48, 27.0f); stand_height = -100.0f;
    msg_run__4Prop(object);
    expect(clip_calls == 0 && grid_calls == 0 && stand_calls == 1, "slow object only queries height");
    fresh();
    put(252, 2.0f); put(256, 0.0f);  // (2/60)^2 = 0.00111 is above 0.001
    stand_height = -100.0f; put(48, 27.0f);
    msg_run__4Prop(object);
    expect(clip_calls == 1, "threshold is above 0.001");
    fresh();
    put(252, 1.8f);  // (1.8/60)^2 = 0.0009 is below 0.001
    stand_height = -100.0f; put(48, 27.0f);
    msg_run__4Prop(object);
    expect(clip_calls == 0, "just under the threshold");

    // Landing: at or below the floor with a non-positive fall.
    fresh();
    put(24, 10.0f); put(264, 0.5f); put(252, 0.0f); put(48, 0.0f); put(252, 10.0f); put(256, -5.0f); stand_height = 10.0f;
    clip_fraction = 1.0f;
    msg_run__4Prop(object);
    expect(near(get(24), 10.5f), "z set to the floor");
    expect(near(get(252), 10.0f * 0.98f) && near(get(256), -5.0f * 0.98f), "velocity scaled by 0.98");
    expect(near(get(48), 26.999998f * 0.4f), "hard landing rebounds at 0.4 of the fall");
    fresh();
    put(24, 10.0f); put(264, 0.5f); put(48, 26.95f); stand_height = 10.0f;
    msg_run__4Prop(object);
    expect(near(get(24), 10.5f) && get(48) == 0.0f, "soft landing (fall above -0.1) stops the fall");
    fresh();
    put(24, 10.0f); put(264, 0.5f); put(48, 27.5f); stand_height = 10.0f;  // fall > 0 rising
    msg_run__4Prop(object);
    expect(get(24) > 10.0f && near(get(48), 0.500002f), "rising object is not clamped");
    fresh();
    put(24, 20.0f); put(264, 0.5f); put(48, 0.0f); stand_height = 10.0f;  // above the floor
    msg_run__4Prop(object);
    expect(near(get(24), 20.0f - 26.999998f / 60.0f) && near(get(48), -26.999998f), "above the floor keeps falling");
    fresh();
    put(24, 10.5f); put(264, 0.5f); put(48, 26.999998f); stand_height = 10.0f;  // fall 0 and z - 0 == floor
    put(252, 10.0f);
    msg_run__4Prop(object);
    expect(near(get(24), 10.5f) && near(get(252), 9.8f) && get(48) == 0.0f, "z equal to the floor with zero fall lands");

    // Halfwords 56, 268.
    fresh();
    write_u16(address(object) + 56u, 0xfff0u); write_u16(address(object) + 268u, 25u);
    msg_run__4Prop(object);
    expect(read_u16(address(object) + 56u) == 9u && read_u16(address(object) + 268u) == 15u, "56 gains 268 (wraps), 268 loses 10");
    fresh();
    write_u16(address(object) + 268u, 5u);
    msg_run__4Prop(object);
    expect(read_u16(address(object) + 56u) == 5u && read_u16(address(object) + 268u) == 0u, "268 clamps at zero");
    fresh();
    write_u16(address(object) + 268u, 10u);
    msg_run__4Prop(object);
    expect(read_u16(address(object) + 268u) == 0u, "268 of 10 becomes 0");
    fresh();
    write_u16(address(object) + 268u, 0xfff6u);  // as signed -10; 0xfff6 - 10 = 0xffec negative
    msg_run__4Prop(object);
    expect(read_u16(address(object) + 268u) == 0u, "negative result clamps");
    fresh();
    write_u16(address(object) + 268u, 0x8005u);  // 0x7ffb, positive
    msg_run__4Prop(object);
    expect(read_u16(address(object) + 268u) == 0x7ffbu, "positive wrap result kept");

    // Bit 0 set skips the whole motion half.
    fresh();
    write_u16(address(object) + 320u, 1u); put(252, 60.0f); write_u16(address(object) + 268u, 25u);
    msg_run__4Prop(object);
    expect(clip_calls == 0 && stand_calls == 0 && read_u16(address(object) + 268u) == 25u && get(24) == 3.0f, "bit 0 skips motion");
    expect(read_u16(address(object) + 270u) == 49u, "counter still counts down");
    fresh();
    write_u16(address(object) + 320u, 0xfffeu);  // other bits do not skip
    msg_run__4Prop(object);
    expect(stand_calls == 1, "only bit 0 selects the skip");

    // Counter.
    fresh();
    write_u16(address(object) + 320u, 1u); write_u16(address(object) + 270u, 1u);
    msg_run__4Prop(object);
    expect(remove_calls == 0 && read_u16(address(object) + 270u) == 0u && read_u16(address(object) + 320u) == 1u, "counter 0 with bit 0 already set: no removal");
    fresh();
    write_u16(address(object) + 320u, 0u); write_u16(address(object) + 270u, 1u);
    msg_run__4Prop(object);
    expect(remove_calls == 1 && remove_object == object && read_u16(address(object) + 320u) == 1u, "counter 0, bits clear: set bit 0 and remove");
    fresh();
    write_u16(address(object) + 320u, 2u); write_u16(address(object) + 270u, 1u);
    msg_run__4Prop(object);
    expect(remove_calls == 0 && read_u16(address(object) + 320u) == 3u, "bit 1 set: bit 0 is set but no removal");
    fresh();
    write_u16(address(object) + 320u, 0u); write_u16(address(object) + 270u, 5u);
    msg_run__4Prop(object);
    expect(remove_calls == 0 && read_u16(address(object) + 320u) == 0u && read_u16(address(object) + 270u) == 4u, "positive counter changes nothing else");
    fresh();
    write_u16(address(object) + 320u, 1u); write_u16(address(object) + 270u, 0xffc5u);  // -59 -> -60
    put(316, 5.0f);
    msg_run__4Prop(object);
    expect(get(24) == 3.0f && get(316) == 5.0f, "-60 is not below -60");
    fresh();
    write_u16(address(object) + 320u, 1u); write_u16(address(object) + 270u, 0xffc4u);  // -60 -> -61
    put(316, 5.0f);
    msg_run__4Prop(object);
    expect(near(get(24), 3.0f - 0.1f) && near(get(316), 5.0f - 0.1f) && slot_calls == 0, "below -60 sinks by 0.1");
    fresh();
    write_u16(address(object) + 320u, 1u); write_u16(address(object) + 270u, 0xfed5u);  // -299 -> -300
    msg_run__4Prop(object);
    expect(slot_calls == 0, "-300 is not below -300");
    fresh();
    write_u16(address(object) + 320u, 1u); write_u16(address(object) + 270u, 0xfed4u);  // -300 -> -301
    msg_run__4Prop(object);
    expect(slot_calls == 1 && slot_self == object + 16 && slot_value == 3, "below -300 calls vtable byte 8 with 3");
    fresh();
    write_u16(address(object) + 320u, 0u); write_u16(address(object) + 270u, 0xfed4u);  // removal wins first
    msg_run__4Prop(object);
    expect(remove_calls == 1 && slot_calls == 0 && get(24) < 3.5f, "removal returns before the sink");
    return failures == 0 ? 0 : 1;
}
