// Host contract for animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps.
// Not an EE plan input. Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#define NORRATH_HOST_ARENA 1
extern "C" {
int drawFrameNm;
unsigned char soundLimit[512];
signed char soundGroupLimit[256];
static int soundCalls, updateCalls;
void playSoundGroup__FP15AnimationHeaderiiP6Point3(unsigned char*, int, int, unsigned char*) { ++soundCalls; }
void updateAnimation__FP13animComponent(unsigned char*) { ++updateCalls; }
int iatan2__Fff(float, float) { return 0; }
}
#include "anim_advance_time.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* header = arena_at(0x1000);
    unsigned char* link = arena_at(0x2000);
    unsigned char* result = arena_at(0x4000);
    float motion[2] = {5.0f, 6.0f};
    short turn = 9;
    std::memset(state, 0, 8 + 32 * 4);
    std::memset(header, 0, 256);
    sw_(header + 4, 100);                          // length
    sf_(header + 20, 0.02f); header[24] = 0x21;   // trigger at 0.02 (1.2 frames)
    sf_(header + 28, 0.5f);  header[32] = 0x04;   // trigger at 0.5 (30 frames)
    sw_(state, norrath_host_address(result));
    sw_(result + 1552, 77);
    state[4] = 1;
    unsigned char* c = state + 8;
    sw_(c, norrath_host_address(header));
    sw_(c + 4, norrath_host_address(link));
    sf_(c + 8, 1.0f); sf_(c + 16, 0.5f); sf_(c + 12, 0.25f); sf_(c + 20, -0.25f);
    sf_(c + 24, 0.0f);
    sh_(c + 28, 0x0200);
    // Step of 2 frames: time passes 1.2 (but not 30), weights change by rate * dt.
    int r = animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(state, 2.0f, nullptr, motion, &turn);
    if (r != 0x21) { std::printf("return %d\n", r); return 1; }
    if (hw(state) != 0 || hw(result + 1552) != 0) return 2;
    if (motion[0] != 0.0f || motion[1] != 0.0f || turn != 0) return 3;
    if (state[4] != 1) return 4;
    float t = lf_(c + 24);
    if (t < 1.99f || t > 2.0f) { std::printf("time %f\n", t); return 5; }
    if (lf_(c + 8) != 1.5f || lf_(c + 16) != 0.0f) { std::printf("weights %f %f\n", lf_(c + 8), lf_(c + 16)); return 6; }
    if ((state[6] | (state[7] << 8)) != 0x0200) return 7;
    // The second weight is zero and flag 0x200 keeps the component; without the flag it is removed.
    sh_(c + 28, 0x0000); sf_(c + 16, 0.25f); sf_(c + 20, -0.25f);
    r = animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(state, 1.0f, nullptr, motion, &turn);
    if (state[4] != 0) return 8;
    if (hw(link + 20) != 0) return 9;
    if (soundCalls != 0 || updateCalls != 0) return 10;
    std::puts("animAdvanceAnimationTime contract passed");
    return 0;
}
