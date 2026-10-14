// Host check for dropGold__4itemG6Point3i. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -ffp-contract=off -fno-builtin host_check.cpp
// The expected candidate points and final generator states were obtained once from a private run of the
// original function with the same seed, point and callee results; they are pinned here as literals.
#include "drop_gold.cpp"

#include <cstdio>
#include <vector>

extern "C" {
unsigned syncRandState;
int worldObject;
void* world = &worldObject;
}

static std::vector<float> heights;
static unsigned heightCalls;
static float seenCandidate[16][3];
static float seenStep[16];
static int allocCalls, allocSize, goldCalls, goldAmount;
static float goldPoint[3];
static int blockObject;
static bool allocFails;

extern "C" {
void* blockAlloc__Fi(int size) { ++allocCalls; allocSize = size; return allocFails ? nullptr : &blockObject; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, const float* point, float step) {
    if (w != &worldObject) return -9999.0f;
    if (heightCalls < 16) {
        for (int i = 0; i < 3; ++i) seenCandidate[heightCalls][i] = point[i];
        seenStep[heightCalls] = step;
    }
    float h = heights[heightCalls % heights.size()];
    ++heightCalls;
    return h;
}
void __4GoldG6Point3i(void* self, const float* point, int amount) {
    if (self != &blockObject) return;
    ++goldCalls;
    goldAmount = amount;
    for (int i = 0; i < 3; ++i) goldPoint[i] = point[i];
}
}

static void reset(std::vector<float> h, bool fail = false) {
    heights = h;
    heightCalls = 0;
    allocCalls = goldCalls = 0;
    allocFails = fail;
    syncRandState = 0x12345678u;
}

int main() {
    const float origin[3] = {10.0f, 20.0f, 30.0f};
    reset({100.0f, 32.0f});
    dropGold__4itemG6Point3i(origin, 7);
    if (heightCalls != 2 || seenStep[0] != 12.0f || seenStep[1] != 12.0f) return 1;
    if (seenCandidate[0][0] != 6.9798583984375f || seenCandidate[0][1] != 41.708984375f || seenCandidate[0][2] != 30.0f) return 2;
    if (seenCandidate[1][0] != -4.80517578125f || seenCandidate[1][1] != 21.368896484375f || seenCandidate[1][2] != 30.0f) return 3;
    if (allocCalls != 1 || allocSize != 192 || goldCalls != 1 || goldAmount != 7) return 4;
    if (goldPoint[0] != -4.80517578125f || goldPoint[1] != 21.368896484375f || goldPoint[2] != 32.0f) return 5;
    if (syncRandState != 0xe2aa733eu) return 6;

    // ten rejected heights: the point is unchanged and the gold is still created.
    reset({100.0f});
    dropGold__4itemG6Point3i(origin, 7);
    if (heightCalls != 10) return 7;
    if (seenCandidate[9][0] != 32.010009765625f || seenCandidate[9][1] != 45.723388671875f) return 8;
    if (goldCalls != 1 || goldPoint[0] != 10.0f || goldPoint[1] != 20.0f || goldPoint[2] != 30.0f) return 9;
    if (syncRandState != 0x97cfc797u) return 10;

    // the height limit is strict: a difference of exactly 6 (either sign) is rejected, just under is accepted.
    reset({36.0f, 24.0f, 29.0f});
    dropGold__4itemG6Point3i(origin, 1);
    if (heightCalls != 3 || goldPoint[2] != 29.0f) return 11;

    // an allocation failure creates no gold; a non-positive amount does nothing at all.
    reset({30.0f}, true);
    dropGold__4itemG6Point3i(origin, 3);
    if (allocCalls != 1 || goldCalls != 0 || heightCalls != 1) return 12;
    reset({30.0f});
    dropGold__4itemG6Point3i(origin, 0);
    dropGold__4itemG6Point3i(origin, -4);
    if (allocCalls != 0 || heightCalls != 0 || syncRandState != 0x12345678u) return 13;
    std::puts("drop gold passed");
    return 0;
}
