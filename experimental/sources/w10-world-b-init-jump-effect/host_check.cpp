// Host contract for initJumpEffect__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include <cstdio>
#include <cstring>
#include <cmath>
struct Point3H { float x, y, z; };
static Point3H canned = {1.5f, -2.5f, 3.5f};
static void* seen_result; static void* seen_camera; static int calls, order_start_set;
#include "init_jump_effect.cpp"
extern "C" void* GetLastTarget__6Camera(void* result, void* camera) {
    ++calls; seen_result = result; seen_camera = camera;
    // The start vector and the timer are already written when the camera is asked.
    order_start_set = startPos.x == 11.f && startPos.y == 12.f && startPos.z == 13.f && jumpGateTime == 0.f;
    std::memcpy(result, &canned, 12);
    return result;
}
extern "C" { void* gameCam = reinterpret_cast<void*>(0x1234); }
Point3 worldCameraPos;
float jumpGateTime = 7.f;
Point3 startPos = {9, 9, 9}, endPos = {8, 8, 8};
int main() {
    worldCameraPos = {11.f, 12.f, 13.f};
    initJumpEffect__Fv();
    if (startPos.x != 11.f || startPos.y != 12.f || startPos.z != 13.f) return 1;
    if (jumpGateTime != 0.f) return 2;
    if (calls != 1 || seen_camera != gameCam || seen_result == nullptr) return 3;
    if (!order_start_set) return 4;
    if (endPos.x != 1.5f || endPos.y != -2.5f || endPos.z != 93.5f) return 5;
    // x gains a positive zero (negative zero becomes positive), y is copied exactly.
    canned = {-0.0f, -0.0f, -90.0f};
    initJumpEffect__Fv();
    if (std::signbit(endPos.x) || !std::signbit(endPos.y) || endPos.z != 0.f) return 6;
    std::puts("initJumpEffect check passed");
    return 0;
}
