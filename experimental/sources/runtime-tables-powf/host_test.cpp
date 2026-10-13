// Host comparison only. Not an EE build input.
#include "powf.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

// Stand-in for the separately reconstructed 2016-byte body. It records the
// forwarded arguments and returns a sentinel the wrapper must pass through.
static float seen_x = 0.0f;
static float seen_y = 0.0f;
static int calls = 0;

float norrath_math_ieee754_powf(float x, float y) {
    seen_x = x;
    seen_y = y;
    ++calls;
    unsigned bits = 0x12345678u;
    float sentinel;
    std::memcpy(&sentinel, &bits, 4);
    return sentinel;
}

static unsigned bits_of(float value) {
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    return bits;
}

static int check(float x, float y) {
    calls = 0;
    // Volatile pointer so the host compiler cannot fold this into its own
    // powf builtin; the authored wrapper must really run and forward.
    static float (*volatile call_powf)(float, float) = powf;
    const float got = call_powf(x, y);
    if (calls != 1) {
        std::printf("powf did not forward once\n");
        return 1;
    }
    if (bits_of(seen_x) != bits_of(x) || bits_of(seen_y) != bits_of(y)) {
        std::printf("powf changed its arguments\n");
        return 1;
    }
    if (bits_of(got) != 0x12345678u) {
        std::printf("powf changed the result bits %08x\n", bits_of(got));
        return 1;
    }
    return 0;
}

int main() {
    if (check(2.0f, 10.0f)) return 1;
    if (check(1.5f, -3.0f)) return 2;
    if (check(0.0f, 0.0f)) return 3;
    unsigned nan_bits = 0x7fc00000u;
    float nan;
    std::memcpy(&nan, &nan_bits, 4);
    if (check(nan, 1.0f)) return 4;
    if (check(10.0f, 308.0f)) return 5;
    return 0;
}
