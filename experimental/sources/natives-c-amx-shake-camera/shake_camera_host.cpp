#include <cstdio>

extern "C" int amxShakeCamera__FP5__amxPi(void*, int*);

static float amount;
static int held;
static int calls;

extern "C" void gameSetCameraShake__Ffb(float value, int flag) {
    amount = value;
    held = flag;
    calls += 1;
}

int main() {
    int params[2] = {4, 12};
    if (amxShakeCamera__FP5__amxPi(nullptr, params) != 0) return 1;
    if (calls != 1 || amount != 12.f || held != 0) return 2;
    params[1] = -8;
    if (amxShakeCamera__FP5__amxPi(nullptr, params) != 0) return 3;
    if (amount != -8.f || held != 0) return 4;
    std::printf("shake camera passed\n");
    return 0;
}
