#include "update_adjustments.cpp"
#include <cstdio>
static int calls = 0;
extern "C" void norrath_sound_update_adjustments() { ++calls; }
int main() {
    soundUpdateAdjustments__Fv();
    soundUpdateAdjustments__Fv();
    if (calls != 2) return 1;
    std::puts("update_adjustments passed");
    return 0;
}
