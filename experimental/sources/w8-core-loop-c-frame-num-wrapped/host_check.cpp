// Host contract for frameNumWrapped. Not an EE plan input.
#include <cstring>
#include "frame_num_wrapped.cpp"

extern "C" {
unsigned char globalSystem[256] __attribute__((aligned(8)));
unsigned char PS[17 * 312] __attribute__((aligned(8)));
}

static const void* seen[40];
static int seenCount = 0;
extern "C" void frameNumWrappedPS__FP14ParticleSystem(ParticleSystem* system) {
    seen[seenCount++] = system;
}

static void setFlag(int index, unsigned value) {
    std::memcpy(PS + index * 312 + 304, &value, 4);
}

int main() {
    // Nothing active: only the global system is visited.
    frameNumWrapped__Fv();
    if (seenCount != 1 || seen[0] != globalSystem) return 1;
    // Active records are visited in order after it; others are skipped.
    seenCount = 0;
    setFlag(0, 1);
    setFlag(5, 0x80000000u);
    setFlag(15, 7);
    setFlag(16, 9);  // beyond the 16 walked records
    // A nonzero byte elsewhere in a record does not count.
    PS[3 * 312 + 300] = 1;
    PS[3 * 312 + 308] = 1;
    frameNumWrapped__Fv();
    if (seenCount != 4) return 2;
    if (seen[0] != globalSystem || seen[1] != PS || seen[2] != PS + 5 * 312 ||
        seen[3] != PS + 15 * 312)
        return 3;
    return 0;
}
