#include "global_init.cpp"
#include <cstdio>
static int calls = 0, phase = -1;
static unsigned priority = 0;
extern "C" void norrath_sound_static_initialization(int p, unsigned q) { ++calls; phase = p; priority = q; }
int main() {
    _GLOBAL_$I$soundSfxVolume();
    if (calls != 1 || phase != 1 || priority != 0xffffu) return 1;
    std::puts("global_init passed");
    return 0;
}
