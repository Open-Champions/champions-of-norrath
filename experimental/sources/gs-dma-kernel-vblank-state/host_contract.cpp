#include "vblank_state.cpp"

#include <cstdio>

int main() {
    if (sizeof vblankCount != 4 || sizeof vblankHID != 4) return 1;
    if (sizeof vblankSemaphore != 4 || sizeof vblankSemaphore2 != 4) return 2;
    if (sizeof vblankField != 1) return 3;
    if (vblankCount != 0 || vblankField != 0 || vblankHID != 0) return 4;
    if (vblankSemaphore != 0 || vblankSemaphore2 != 0) return 5;
    if (&vblankCount == reinterpret_cast<int*>(&vblankField)) return 6;
    if (&vblankSemaphore == &vblankSemaphore2) return 7;
    std::puts("vblank state contract passed");
    return 0;
}
