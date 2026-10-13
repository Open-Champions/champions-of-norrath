// Host contract for animGetBoneMatrix__FP14AnimationStateiP8Matrix34. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" {
unsigned char identityResult[1600];
static int evalCalls;
static unsigned evalWord;
void evaluateAnimation__FP14AnimationStateb(unsigned char* state, bool flag) {
    ++evalCalls;
    if (flag) evalCalls += 100;
    if (evalWord) { state[0] = evalWord; state[1] = evalWord >> 8; state[2] = evalWord >> 16; state[3] = evalWord >> 24; }
}
}
#include "anim_get_bone_matrix.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
static void fill(unsigned char* rec, int bones, int seed) {
    sh_(rec + 8, bones);
    for (int b = 0; b != bones; ++b)
        for (int k = 0; k != 24; ++k) rec[b * 24 + 16 + k] = 0;
    for (int b = 0; b != bones; ++b)
        for (int h = 0; h != 12; ++h) sh_(rec + b * 24 + 16 + 2 * h, (seed + b * 100 + h * 7 - 40) & 0xffff);
}
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* rec = arena_at(0x2000);
    float out[14];
    auto halfword = [&](int b, int h) { int v = rec[b * 24 + 16 + 2 * h] | (rec[b * 24 + 17 + 2 * h] << 8); return v >= 0x8000 ? v - 0x10000 : v; };
    sw_(state, norrath_host_address(rec));
    fill(rec, 4, 30);
    for (int i = 0; i != 14; ++i) out[i] = 777.0f;
    animGetBoneMatrix__FP14AnimationStateiP8Matrix34(state, 2, out);
    if (evalCalls != 0) return 1;
    for (int e = 0; e != 3; ++e)
        for (int j = 0; j != 3; ++j)
            if (out[e * 3 + j] != halfword(2, 4 * j + e) / 4096.0f) { std::printf("rot %d %d\n", e, j); return 2; }
    if (out[9] != halfword(2, 3) / 64.0f || out[10] != halfword(2, 7) / 64.0f || out[11] != halfword(2, 11) / 64.0f) return 3;
    if (out[12] != 777.0f || out[13] != 777.0f) return 4;
    // Bone clamps: negative goes to 0, beyond the count goes to count-1.
    animGetBoneMatrix__FP14AnimationStateiP8Matrix34(state, -9, out);
    if (out[0] != halfword(0, 0) / 4096.0f || out[9] != halfword(0, 3) / 64.0f) return 5;
    animGetBoneMatrix__FP14AnimationStateiP8Matrix34(state, 99, out);
    if (out[0] != halfword(3, 0) / 4096.0f || out[11] != halfword(3, 11) / 64.0f) return 6;
    // Missing result record: evaluateAnimation is called once with a false flag.
    sw_(state, 0); evalWord = norrath_host_address(rec);
    animGetBoneMatrix__FP14AnimationStateiP8Matrix34(state, 1, out);
    if (evalCalls != 1 || out[1] != halfword(1, 4) / 4096.0f) return 7;
    // Still missing afterwards: the identity record is used.
    sw_(state, 0); evalWord = 0; evalCalls = 0;
    fill(identityResult, 2, 5);
    animGetBoneMatrix__FP14AnimationStateiP8Matrix34(state, 1, out);
    if (evalCalls != 1) return 8;
    {
        int v = identityResult[24 + 16 + 16] | (identityResult[24 + 16 + 17] << 8);
        if (v >= 0x8000) v -= 0x10000;
        if (out[2] != v / 4096.0f) return 9;
    }
    std::puts("animGetBoneMatrix contract passed");
    return 0;
}
