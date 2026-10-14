#pragma once
// Host-check body shared by the HateInto and HateGate checks (not a plan input).
// The includer defines NORRATH_W8C_HOST, includes its run.cpp, then this file,
// and calls hate_check(entry, mask, step, definition).
#include <cstdio>
#include <cstring>
int frameNm = 0;
static int failures = 0;
static int autoprop_calls = 0, shadow_calls = 0, particle_calls = 0, nm_answer = 0, rand_calls = 0;
static int shadow_indices[64], shadow_angles[64];
static void* seen_definition[64];
static float seen_position[64][3];
static float seen_velocity[64][3];
static float seen_age[64];
static unsigned rand_values[3] = {0x8000u, 0x10000u, 0u};
static unsigned char* g_self = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void msg_run__8AutoProp(void* self) { ++autoprop_calls; expect(shadow_calls == 0, "AutoProp runs first"); g_self = static_cast<unsigned char*>(self); }
extern "C" int modelGetNmShadowPoints__FP12_modelHeader(void*) { return nm_answer; }
extern "C" int modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void*, void* state, const void* point, short angle, int index, void* out, float* scratch, void* matrix, float scale) {
    float p[3];
    std::memcpy(p, point, 12);
    expect(state == g_self + 152 && p[0] == 3.0f && p[1] == 4.0f && p[2] == 5.0f, "state and position copy");
    expect(scratch != nullptr && matrix == nullptr && scale == 1.0f, "scratch, null matrix, unit scale");
    shadow_indices[shadow_calls] = index;
    shadow_angles[shadow_calls] = angle;
    float shadow[3] = {10.0f * (shadow_calls + 1), 20.0f, 30.0f};
    std::memcpy(out, shadow, 12);
    ++shadow_calls;
    return 0;
}
extern "C" unsigned norrath_actor_random(void* object) {
    expect(object == g_self, "random takes the object");
    return rand_values[rand_calls++ % 3];
}
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float age) {
    seen_definition[particle_calls] = definition;
    std::memcpy(seen_position[particle_calls], position, 12);
    std::memcpy(seen_velocity[particle_calls], velocity, 12);
    seen_age[particle_calls] = age;
    ++particle_calls;
}
static void hate_check(void (*entry)(void*), unsigned mask, int step, void* definition) {
    unsigned char object[400];
    std::memset(object, 0, sizeof object);
    w8c::write_single(w8c::address(object) + 16u, 3.0f);
    w8c::write_single(w8c::address(object) + 20u, 4.0f);
    w8c::write_single(w8c::address(object) + 24u, 5.0f);
    w8c::write_u16(w8c::address(object) + 56u, 0xfff0u);
    for (int start = 0; start < 12; ++start) {
        frameNm = start;
        autoprop_calls = shadow_calls = particle_calls = rand_calls = 0;
        nm_answer = 20;
        entry(object);
        const int first = start & static_cast<int>(mask);
        const int expected = first < 20 ? (20 - first + step - 1) / step : 0;
        expect(autoprop_calls == 1, "AutoProp once");
        expect(shadow_calls == expected && particle_calls == expected && rand_calls == 3 * expected, "pass count");
        for (int k = 0; k < shadow_calls; ++k) {
            expect(shadow_indices[k] == first + k * step && shadow_angles[k] == -16, "index and angle");
            expect(seen_definition[k] == definition && seen_age[k] == 0.0f, "definition and age");
            expect(seen_velocity[k][0] == 0.0f && seen_velocity[k][1] == 0.0f && seen_velocity[k][2] == 0.0f, "zero velocity every pass");
            // rand 0x8000 -> 0.5 -> 0 -> 0; rand 0x10000 -> 1.0 -> 0.5 -> 1; rand 0 -> 0 -> -0.5 -> -1
            expect(seen_position[k][0] == 10.0f * (k + 1) && seen_position[k][1] == 21.0f && seen_position[k][2] == 29.0f, "position plus offsets");
        }
    }
    frameNm = 5; nm_answer = -4; shadow_calls = 0;
    entry(object);
    expect(shadow_calls == 0, "negative count does nothing");
}
