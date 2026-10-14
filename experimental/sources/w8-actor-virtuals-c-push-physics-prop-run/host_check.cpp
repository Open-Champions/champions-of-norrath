#define NORRATH_W8C_HOST 1
#include "run.cpp"
#include "../w8-actor-virtuals-c-bytes/host_vcall.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
unsigned char norrath_actors_b_view_4844a0[4];
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static int untransforms = 0, velocities = 0, add_forces = 0, impulses = 0, apply_forces = 0, clips = 0, grids = 0, finds = 0, stands = 0;
static int contacts = 0;
static void* contact_args[4][2];
static float velocity_answer[3] = {0.0f, 0.0f, 0.0f};
static float clip_answer = 1.0f, stand_answer = 0.0f;
static float spring_seen = 0.0f, drag_seen_x = 0.0f;
static float apply_scale = 0.0f, impulse_scale = 0.0f;
static void* listed[4];
static int type_a = 0;
extern "C" void matUntransform__FP6Point3P8Matrix33T0(const void* in, const void*, void* out) { ++untransforms; std::memmove(out, in, 12); }
extern "C" void bodyGetVelocity__FP5_bodyP6Point3T1(void*, const void*, void* out) { ++velocities; std::memcpy(out, velocity_answer, 12); }
extern "C" void bodyAddForce__FP5_bodyP6Point3N31(void*, const void*, const void* force, void*, void*) {
    ++add_forces; float f[3]; std::memcpy(f, force, 12);
    if (f[2] != 0.0f) spring_seen = f[2]; else drag_seen_x = f[0];
}
extern "C" void bodyApplyImpulse__FP5_bodyG6Point3T1f(void*, const void*, const void*, float scale) { ++impulses; impulse_scale = scale; }
extern "C" void bodyApplyForce__FP5_bodyG6Point3T1f(void*, const void*, const void*, float scale) { ++apply_forces; apply_scale = scale; }
extern "C" float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(void*, const void*, const void*, float, int flags, void* normal) {
    ++clips; expect(flags == 1, "clip flags 1");
    if (normal) { float n[3] = {0.0f, 1.0f, 0.0f}; std::memcpy(normal, n, 12); }
    return clip_answer;
}
extern "C" float worldFindStandHeight__FP12_worldHeaderG6Point3f(void*, const void*, float) { ++stands; return stand_answer; }
extern "C" void objectUpdateInGrid__FP10GameObject(void*) { ++grids; }
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void*, const void*, void** out, int capacity) {
    ++finds; expect(capacity == 16, "capacity 16");
    for (int i = 0; i < 4; ++i) out[i] = listed[i];
}
extern "C" void* __tf15PushPhysicsProp() { return &type_a; }
extern "C" int __eq__C9type_infoRC9type_info(void* l, void* r) { return l == r; }
extern "C" void __throw_bad_typeid() { expect(false, "typeid of a null object"); }
static void* type_a_fn() { return &type_a; }
static void contact(void* self, void* other) { contact_args[contacts % 4][0] = self; contact_args[contacts % 4][1] = other; ++contacts; }

static unsigned char object[400], other[400], tables[2][64], self_table[64];
static void setup() {
    using namespace w8c;
    std::memset(object, 0, sizeof object); std::memset(other, 0, sizeof other);
    std::memset(tables, 0, sizeof tables); std::memset(self_table, 0, sizeof self_table);
    host_install_slot(object, self_table, 48u, 8, reinterpret_cast<void*>(contact));
    host_install_slot(other, tables[0], 48u, 16, reinterpret_cast<void*>(contact));
    write_word(address(tables[0]) + 4u, address_bits(reinterpret_cast<void*>(type_a_fn)));
    host_note(reinterpret_cast<void*>(type_a_fn));
    write_word(address(object) + 252u, 3u);
    write_single(address(object) + 80u, 2.0f);
    write_single(address(object) + 256u, 1.0f); write_single(address(object) + 28u, 2.0f); write_single(address(object) + 32u, 4.0f);
    write_single(address(object) + 76u, -50.0f);
    for (int i = 0; i < 3; ++i) { write_single(address(object) + 16u + 4u * i, 0.0f); write_single(address(object) + 96u + 4u * i, 0.0f); write_single(address(object) + 232u + 4u * i, 0.0f); }
    write_single(address(object) + 104u, 10.0f); write_single(address(object) + 24u, 10.0f);
    write_single(address(other) + 28u, 2.0f); write_single(address(other) + 32u, 4.0f);
    write_single(address(other) + 16u, 1.0f);
    untransforms = velocities = add_forces = impulses = apply_forces = clips = grids = finds = stands = contacts = 0;
    velocity_answer[0] = velocity_answer[1] = velocity_answer[2] = 0.0f;
    clip_answer = 1.0f; stand_answer = -60.0f;
    listed[0] = nullptr; listed[1] = nullptr;
}
int main() {
    using namespace w8c;
    setup(); write_word(address(object) + 252u, 0u);
    msg_run__15PushPhysicsProp(object);
    expect(untransforms == 0 && grids == 0, "zero +252 ends the call");
    // quiet case: all eight corners above the floor, nothing nearby
    setup();
    msg_run__15PushPhysicsProp(object);
    expect(read_word(address(object) + 252u) == 2u, "countdown decremented");
    expect(untransforms == 8 && velocities == 0 && add_forces == 0, "eight corners, none below the floor");
    expect(apply_forces == 1 && apply_scale == 1.0f && finds == 1 && stands == 1 && grids == 1, "single apply, one query, one grid update");
    expect(std::fabs(read_single(address(object) + 132u) - (0.0f - 0.18f / 2.0f)) < 1e-6f, "gravity step 0.18 / mass");
    expect(read_single(address(object) + 76u) == -60.0f && read_single(address(object) + 24u) == 10.0f - 1.0f, "floor from the query; z drops by half extent");
    // corners below the floor: drag, impulse on downward velocity, spring only when positive
    setup(); write_single(address(object) + 76u, 100.0f); velocity_answer[0] = 5.0f; velocity_answer[2] = -2.0f;
    msg_run__15PushPhysicsProp(object);
    expect(velocities == 16 && impulses == 8 && add_forces == 16, "each corner: velocity, impulse, drag + spring");
    expect(std::fabs(impulse_scale + 0.2f) < 1e-6f && std::fabs(drag_seen_x + 0.5f) < 1e-6f, "impulse scale and drag");
    expect(spring_seen > 0.0f && std::fabs(spring_seen - (4.0f * 0.01f - (-2.0f) * 0.01f)) < 1e-5f, "spring push uses the depth capped at 4");
    // a distant corner marks the object as moved
    setup(); write_single(address(object) + 104u, 10.0f);
    write_single(address(object) + 232u, 50.0f);
    msg_run__15PushPhysicsProp(object);
    expect(read_word(address(object) + 248u) == 30u || read_word(address(object) + 248u) == 29u, "far corner sets +248");
    expect(read_word(address(object) + 244u) == 59u, "+244 starts at 60 and ticks once");
    // support object (bit 0x10) within reach and 6 below: its top becomes the floor
    setup(); write_word(address(other) + 12u, 0x10u); write_single(address(other) + 24u, 7.0f); write_single(address(object) + 24u, 10.0f);
    listed[0] = other; stand_answer = 0.0f;
    msg_run__15PushPhysicsProp(object);
    expect(read_single(address(object) + 76u) == 11.0f && contacts == 0, "support top used as the floor, no contact calls");
    // plain contact: pushed apart, both virtual slots called
    setup(); listed[0] = object; listed[1] = other;
    msg_run__15PushPhysicsProp(object);
    expect(contacts == 2 && contact_args[0][0] == other + 16 && contact_args[0][1] == object, "other's slot first with its adjusted self and this object");
    expect(contact_args[1][0] == object + 8 && contact_args[1][1] == other, "then this object's slot");
    expect(clips == 1, "the push is clipped once");
    // same-kind object with nonzero +248 skips the clip
    setup(); listed[0] = other; write_word(address(other) + 248u, 4u);
    msg_run__15PushPhysicsProp(object);
    expect(clips == 0 && contacts == 2, "locked other object skips the clip");
    // bit 0x1 objects are ignored
    setup(); listed[0] = other; write_word(address(other) + 12u, 1u);
    msg_run__15PushPhysicsProp(object);
    expect(contacts == 0 && clips == 0, "flag 1 objects ignored");
    return failures ? 1 : 0;
}
