#include "sound_restore.cpp"
#include <cstdio>
static int calls = 0;
static bool seen = false;
extern "C" void norrath_sound_restore(bool flag) { ++calls; seen = flag; }
int main() {
    soundRestore__Fb(true);
    if (calls != 1 || !seen) return 1;
    soundRestore__Fb(false);
    if (calls != 2 || seen) return 2;
    std::puts("sound_restore passed");
    return 0;
}
