#include <cstdio>
extern "C" float streamVolume[3] = {0.25f, 0.5f, 0.25f};
static int updates = 0;
extern "C" void soundUpdateAdjustments__Fv() { updates += 1; }
extern "C" void soundSetStreamVolume__Fif(int, float);
int main() {
    soundSetStreamVolume__Fif(1, 0.5f);
    if (updates != 0) return 1;
    soundSetStreamVolume__Fif(0, -2.f);
    if (updates != 1 || streamVolume[0] != 0.f) return 2;
    soundSetStreamVolume__Fif(2, 4.f);
    if (updates != 2 || streamVolume[2] != 1.f) return 3;
    std::puts("stream volume contract passed");
    return 0;
}
