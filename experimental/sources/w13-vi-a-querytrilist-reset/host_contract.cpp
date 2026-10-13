#include "reset.cpp"
#include <string.h>

static int vol_ret, vol_calls; static void *vl, *vb; static float cos_arg; static int cos_calls;
extern "C" int Volume__14VIQueryTriListRC6VIBBox(void* l, const void* b) { ++vol_calls; vl = l; vb = (void*)b; return vol_ret; }
extern "C" float cosf(float x) { ++cos_calls; cos_arg = x; return 0.25f; }

int main() {
    alignas(4) unsigned char l[64]; int box = 0; float axis[3] = {1.5f, -2.f, 3.25f};
    memset(l, 0x66, sizeof l); *(int*)l = 9;
    vol_ret = -4;
    if (Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f(l, &box, axis, 90.f) != -1) return 1;
    if (*(int*)l != 0 || vl != l || vb != &box || cos_calls != 0 || l[28] != 0x66 || l[40] != 0x66) return 2;
    vol_ret = 0;
    if (Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f(l, &box, axis, 90.f) != 0 || cos_calls != 1) return 3;
    if (*(float*)(l + 28) != 1.5f || *(float*)(l + 32) != -2.f || *(float*)(l + 36) != 3.25f) return 4;
    if (*(float*)(l + 40) != 0.25f) return 5;
    float want = 90.f * 0.017453292f;
    if (cos_arg != want) return 6;
    for (int i = 4; i < 64; ++i) if ((i < 28 || i >= 44) && l[i] != 0x66) return 7;
    return 0;
}
