// Host check only; not a build input.
#include "update_center_transform.cpp"
#include <cstring>
static int interp_ret, ncall; static char order[16]; static int dtors; static long long now = 0x123456789ll;
static float seen_centre[3], seen_neg[3], seen_scale, seen_q[4], seen_ts, seen_t[3], seen_frame_init[8];
static void* Mptr;
extern "C" {
int InterpTransAboutCenter__9VIHSpriteR7VISceneR7VIVect3R14VIHSpriteFrame(void*, void*, float* c, void* f) {
    float* fr = (float*)f;
    std::memcpy(seen_frame_init, fr, 32);
    if (c[0] != 0 || c[1] != 0 || c[2] != 0) interp_ret = 99;
    c[0] = 1; c[1] = 2; c[2] = 3; fr[3] = 0.5f; fr[4] = 2.0f; fr[5] = 7; fr[6] = 8; fr[7] = 9;
    order[ncall++] = 'I';
    return interp_ret;
}
void Scale__10VIMatrix44f(void* m, float s) { Mptr = m; seen_scale = s; order[ncall++] = 'S'; }
void Translate__10VIMatrix44RC7VIVect3(void*, const float* v) { std::memcpy(seen_neg, v, 12); order[ncall++] = 'T'; }
void Mul__10VIMatrix44RC10VIMatrix44T1(void*, const void*, const void*) { order[ncall++] = 'M'; }
void Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(void* m, const float* q, float s, const float* t) {
    if (m != Mptr) ncall = 99;
    std::memcpy(seen_q, q, 16); seen_ts = s; std::memcpy(seen_t, t, 12); order[ncall++] = 'X';
}
long long GetTime__7VIScene(void*) { order[ncall++] = 'G'; return now; }
void _$_10VIMatrix44(void*, int f) { if (f == 2) dtors++; order[ncall++] = 'D'; }
}
int main() {
    alignas(16) char sprite[512]; std::memset(sprite, 0, sizeof sprite);
    *(float*)(sprite + 208) = 1.25f;
    interp_ret = -3; 
    if (UpdateCenterTransform__9VIHSpriteR7VIScene(sprite, nullptr) != -1) return 1;
    if (std::memcmp(order, "IDD", 3) != 0 || ncall != 3 || dtors != 2 || *(long long*)(sprite + 192) != 0) return 2;
    float init[8] = {0, 0, 0, 1, 1, 0, 0, 0};
    if (std::memcmp(seen_frame_init, init, 32) != 0) return 3;
    ncall = 0; dtors = 0; interp_ret = 0;
    if (UpdateCenterTransform__9VIHSpriteR7VIScene(sprite, nullptr) != 0) return 4;
    if (std::memcmp(order, "ISTMXMTMGDD", 11) != 0 || ncall != 11 || dtors != 2) return 5;
    if (Mptr != sprite + 320 || seen_scale != 1.25f) return 6;
    if (seen_neg[0] != -1 || seen_neg[1] != -2 || seen_neg[2] != -3) return 7;
    if (seen_q[3] != 0.5f || seen_ts != 2.0f || seen_t[0] != 7 || seen_t[2] != 9) return 8;
    if (*(long long*)(sprite + 192) != now) return 9;
    return 0;
}
