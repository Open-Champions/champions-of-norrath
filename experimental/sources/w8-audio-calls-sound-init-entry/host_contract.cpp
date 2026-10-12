#include "sound_init.cpp"
#include <cstdio>
static int calls = 0;
static float seen = 0;
extern "C" void norrath_sound_initialize(float gain) { ++calls; seen = gain; }
int main() {
    soundInit__Ff(0.375f);
    if (calls != 1 || seen != 0.375f) return 1;
    soundInit__Ff(-2.0f);
    if (calls != 2 || seen != -2.0f) return 2;
    std::puts("sound_init passed");
    return 0;
}
