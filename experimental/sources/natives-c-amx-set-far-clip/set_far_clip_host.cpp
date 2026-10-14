#include <cstdio>

extern "C" int amxSetFarClip__FP5__amxPi(void*, int*);

static float seen;
static int calls;

extern "C" void worldSetFarClip__Ff(float distance) {
    seen = distance;
    calls += 1;
}

int main() {
    int params[2] = {4, 100};
    if (amxSetFarClip__FP5__amxPi(nullptr, params) != 0) return 1;
    if (calls != 1 || seen != 100.f) return 2;
    params[1] = -4;
    if (amxSetFarClip__FP5__amxPi(nullptr, params) != 0) return 3;
    if (seen != -4.f) return 4;
    params[1] = 0;
    if (amxSetFarClip__FP5__amxPi(nullptr, params) != 0 || seen != 0.f) return 5;
    std::printf("set far clip passed\n");
    return 0;
}
