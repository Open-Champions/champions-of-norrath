// Host check for the animation storage extents and zero start. Not an EE plan input.
#include <cstdio>
#include "anim_state_data.cpp"
int main() {
    int failures = 0;
    auto zero = [&](const void* p, unsigned n) {
        const unsigned char* b = static_cast<const unsigned char*>(p);
        for (unsigned i = 0; i < n; ++i) if (b[i]) return false;
        return true;
    };
    if (sizeof animState != 264) { std::fprintf(stderr, "fail: animState extent\n"); ++failures; }
    if (!zero(animState, sizeof animState) || animStatePtr != nullptr || animUseBuffer != 0 || animationLoading != 0 ||
        animationLoadPos != 0) { std::fprintf(stderr, "fail: storage is not zero\n"); ++failures; }
    if (sizeof animUseBuffer != 1 || sizeof animationLoading != 1 || sizeof animationLoadPos != 4 ||
        sizeof animStatePtr != sizeof(void*)) { std::fprintf(stderr, "fail: scalar widths\n"); ++failures; }
    animUseBuffer = -1;   // signed: a negative value stays negative
    if (!(animUseBuffer < 0)) { std::fprintf(stderr, "fail: animUseBuffer is not signed\n"); ++failures; }
    if (failures) return 1;
    std::printf("animation storage check passed\n");
    return 0;
}
